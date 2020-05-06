//SourceTask.cpp: Jordan Smith and Andrea Robey

#include "SourceTask.h"

class SourceBlock::ScheduleNextCalendarAircraftEA : public EventAction {
public:
	ScheduleNextCalendarAircraftEA(SourceBlock* source, RepairJob* repairJob, CalendarObj* calObj) {
		_source = source;
		_repairJob = repairJob;
		_calobj = calObj;
	}
	void Execute() {
		_source->ScheduleNextCalendarAircraftEM(_repairJob, _calobj);
	}
private:
	SourceBlock* _source;
	RepairJob* _repairJob;
	CalendarObj* _calobj;
};

class SourceBlock::ScheduleNextRecurringAircraftEA : public EventAction {
public:
	ScheduleNextRecurringAircraftEA(SourceBlock* source, Distribution* recurringiAT, RepairJob* repairJob) {
		_source = source;
		_repairJob = repairJob;
		_recurringIAT = recurringiAT;
	}
	void Execute() {
		_source->ScheduleNextRecurringAircraftEM(_recurringIAT, _repairJob);
	}
private:
	SourceBlock* _source;
	RepairJob* _repairJob;
	Distribution* _recurringIAT;
};

class SourceBlock::ScheduleNextUnplannedAircraftEA : public EventAction {
public:
	ScheduleNextUnplannedAircraftEA(SourceBlock* source, RepairJob* repairJob) {
		_source = source;
		_repairJob = repairJob;
	}

	void Execute() {
		_source->ScheduleNextUnplannedAircraftEM(_repairJob);
	}

private:
	SourceBlock* _source;
	RepairJob* _repairJob;
};

/*Constructor for Unplanned*/
SourceBlock::SourceBlock(Distribution* iat, string aircraftType, Aircraft* aircraft, string name, RepairJob* repairJob,
	int numberOfAircraftToGenerate) : Task(name)
{
	_name = name;
	_interarrivalTimeRND = iat->CopyThis();
	_aircraftType = aircraftType;
	_aircraft = aircraft;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
	_numberGenerated = 0;

	SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new ScheduleNextUnplannedAircraftEA(this, repairJob),
		_interarrivalTimeRND->GetRV(), "ScheduleNextUnplannedAircraftEA");
}

/*Constructor for Recurring*/
SourceBlock::SourceBlock(map<string, Distribution*> recurringIATS, string aircraftType, Aircraft* aircraft, string name,
	int numberOfAircraftToGenerate) : Task(name)
{
	_name = name;
	_recurringIATs = recurringIATS;
	_aircraftType = aircraftType;
	_aircraft = aircraft;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
	_numberGenerated = 0;

	map<string, Distribution*>::const_iterator it = _recurringIATs.begin();
	it = _recurringIATs.begin();

	while (it != _recurringIATs.end()) 
	{
		RepairJob* newptr = aircraft->GetMyRepairJobObj(it->first);
		SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this, it->second, newptr),
			it->second->GetRV(), "ScheduleNextRecurringAircraftEA");
		it++;
	}
}

/*Constructor for Calendar*/
//scheduling aircraft arrival
SourceBlock::SourceBlock(string aircraftType, Aircraft* aircraft, string name,
	CalendarObj* calobj, RepairJob* repairJob) : Task(name)
{
	_aircraft = aircraft;
	_name = name;
	SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextCalendarAircraftEA(this, repairJob, calobj), 0.0, "ScheduleNextCalendarAircraftEA");
}

string SourceBlock::GetAircraftType() {
	return _aircraftType;
}

void  SourceBlock::SetAircraftType(string aircraftType) {
	_aircraftType = aircraftType;
}

string SourceBlock::GetName() {
	return _name;
}

int SourceBlock::GetNumberGenerated() {
	return _numberGenerated;
}

void SourceBlock::ScheduleNextCalendarAircraftEM(RepairJob* repairJob, CalendarObj* calObj) {

	Aircraft* newAircraft = _aircraft->New();
	InputReader::CalAirFix();

	map<string, RepairJob*>::iterator it = newAircraft->GetMyRJMapBegin();
	while (it != newAircraft->GetMyRJMapEnd())
	{
		if (it->second->GetSchedType() == "Calendar")
		{
			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*it->second);
			newAircraft->AddMyRepairJob(currJob->GetName(), currJob);

			it++;
		}

		else if (it->second->GetSchedType() != "Calendar")
		{
			newAircraft->DeleteJob(it->first);
			it = newAircraft->GetMyRJMapBegin();
		}
		else
			it++;
	}

	it = newAircraft->GetMyRJMapBegin();
	
	for (int i = 0; i < calObj->GetNumEvents() / 2; ++i) 
	{
		Aircraft* firstStep = new Aircraft(*newAircraft);
		firstStep->UpdateList(*newAircraft);
	
		map<string, RepairJob*>::const_iterator iter = newAircraft->GetMyRJMapBegin();
		while (iter != newAircraft->GetMyRJMapEnd()) {
			
			if (iter->first == it->first) {}
			
			else
			{
				firstStep->DeleteJob(iter->first);
			}

			iter++;
		}
		repairJob = it->second;
		InputReader::AddAirCount();
		repairJob->GetFirstStep()->ScheduleCalendarStep(repairJob->GetFirstStep(), firstStep, calObj, i);
		_numberGenerated++;
		
		it++;
		Scribe::RecordAircraft(_aircraft->GetAircraftType());
	}
	

}


void SourceBlock::ScheduleNextUnplannedAircraftEM(RepairJob* repairJob)
{
	if (_numberGenerated != _numberOfAircraftToGenerate)
	{
		SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextUnplannedAircraftEA(this, repairJob), _interarrivalTimeRND->GetRV(), "ScheduleNextUnplannedAircraftEA");
		Aircraft* newAircraft = _aircraft->New();
	
		//clear _myrepairmap to specify my specific job list
		newAircraft->ClearMyMap();

		int jobCounter = 0;
		string job;

		//For all unplanned repair jobs
		map<string, RepairJob*>::const_iterator iter = newAircraft->GetMyUnplannedMapBegin();
		 
		while (iter != newAircraft->GetMyUnplannedMapEnd())
		{
			bool willsched = iter->second->WillSchedule();

			//roll the dice - if we aren't going to schedule it, go to the next job
			if ((willsched == false))
			{

				iter++;

				continue;
			}

			//Roll the dice do see which repair jobs will be required
			else if (willsched == true)
			{  
				//if its a job we're going to schedule, put it in a map based on priority
				AddToPriorityMap(iter->second->GetPriority(), iter->first);

				Scribe::TallyUnplanned(1); 
			}

			iter++;
		}

		//if any jobs are going to be scheduled
		if (_jobPriority.size() > 0)
		{
			//job is the job with highest priority
			job = _jobPriority.begin()->second;

			//add this job to the cleared _myrepairjobs
			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*InputReader::FindMasterRJ(newAircraft->GetAircraftType(),job)->second);
			newAircraft->AddMyRepairJob(currJob->GetName(), currJob);

			//Add bay requirement to aircraft's required resources
			newAircraft->AddBayReqToRes(newAircraft);

			//schedule that high priority job from master map because mymap was cleared	
 			InputReader::FindMasterRJ(newAircraft->GetAircraftType(),job)->second->GetStep(1)->
			ScheduleFirstStep(InputReader::FindMasterRJ(newAircraft->GetAircraftType(),job)->second->GetStep(1), newAircraft);
			jobCounter++;
		}

		else if (jobCounter == 0)
		{
			map<string, RepairJob*>::iterator insertIter = newAircraft->GetMyUnplannedMapBegin();
			while (insertIter != newAircraft->GetMyUnplannedMapEnd())
			{
				//Add name of every repair job in the list to vector for random selection
				newAircraft->InsertJobName(insertIter->first);
				insertIter++;
			}

			//Pick a random element from the vector
			string selection = newAircraft->GetRandomElement();

			//Schedule this job
			map<string, RepairJob*>::iterator addRJIt = newAircraft->FindMyUnRepairJob(selection);
			addRJIt->second->GetStep(1)->ScheduleFirstStep(addRJIt->second->GetFirstStep(), newAircraft);
			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*addRJIt->second);
			newAircraft->AddMyRepairJob(currJob->GetName(), currJob);
		}

		_numberGenerated++;
		Scribe::RecordAircraft(_aircraft->GetAircraftType());
	}
}


void SourceBlock::AddToPriorityMap(int priority, string job)
{
	_jobPriority[priority] = job;
}

void SourceBlock::ScheduleNextRecurringAircraftEM(Distribution* recurringIAT, RepairJob* repairJob) {

	if (_numberGenerated != _numberOfAircraftToGenerate) 
	{
		
		//Scheduling recurring aircrafts
		SimExec::ScheduleEventAtRecurring(_aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this, recurringIAT, repairJob), recurringIAT->GetRV(), "ScheduleNextRecurringAircraftEA");
		Aircraft* newAircraft = _aircraft->New();

		map<string, RepairJob*>::iterator it = newAircraft->GetMyRJMapBegin();
		while (it != newAircraft->GetMyRJMapEnd())
		{
			if (it->second->GetSchedType() == "Recurring")
			{
				RepairJob* currJob = new RepairJob();
				currJob->CopyRepairJob(*it->second);
				newAircraft->AddMyRepairJob(currJob->GetName(), currJob);

				it++;
			}
			else if (it->second->GetSchedType() != "Recurring")
			{
				newAircraft->DeleteJob(it->first);
				it = newAircraft->GetMyRJMapBegin();
			}
			else
				it++;
		}
		repairJob->GetFirstStep()->ScheduleFirstRecurringStep(repairJob->GetStep(1), newAircraft);
		_numberGenerated++;
		Scribe::RecordAircraft(_aircraft->GetAircraftType());
	}
}

void SourceBlock::Execute(Aircraft* aircraft) {
	//This is just a declaration of the virtual function doesn't do anything
}