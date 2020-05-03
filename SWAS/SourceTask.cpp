#include "SourceTask.h"
//#include "SimExec.h"

class SourceBlock::ScheduleNextCalendarAircraftEA : public EventAction {
public:
	ScheduleNextCalendarAircraftEA(SourceBlock* source, RepairJob* repairJob, CalendarObj* calObj) {
		_source = source ;
		_repairJob = repairJob ;
		_calobj = calObj ;
	}
	void Execute() {
		_source->ScheduleNextCalendarAircraftEM(_repairJob, _calobj) ;
	}
private:
	SourceBlock* _source ;
	RepairJob* _repairJob ;
	CalendarObj* _calobj ;
} ;

//class SourceBlock::ScheduleNextRecurringAircraftEA : public EventAction {
//public:
//	ScheduleNextRecurringAircraftEA(SourceBlock* source) {
//		_source = source ;
//	}
//
//	void Execute() {
//		_source->ScheduleNextRecurringAircraftEM() ;
//	}
//private:
//	SourceBlock* _source ;
//} ;

class SourceBlock::ScheduleNextRecurringAircraftEA : public EventAction {
public:
	ScheduleNextRecurringAircraftEA(SourceBlock* source, Distribution* recurringiAT, RepairJob* repairJob) {
		_source = source ;
		_repairJob = repairJob ;
		_recurringIAT = recurringiAT ;
	}
	void Execute() {
		_source->ScheduleNextRecurringAircraftEM(_recurringIAT, _repairJob) ;
	}
private:
	SourceBlock* _source ;
	RepairJob* _repairJob ;
	Distribution* _recurringIAT ;
} ;

class SourceBlock::ScheduleNextUnplannedAircraftEA : public EventAction {
public:
	ScheduleNextUnplannedAircraftEA(SourceBlock* source, RepairJob* repairJob) {
		_source = source ;
		_repairJob = repairJob ;
	}

	void Execute() {
		_source->ScheduleNextUnplannedAircraftEM(_repairJob) ;
		//cout << "********IN NEXT UNPLANNED EA. REPAIR JOB IS" << _repairJob->GetName() << endl ;
	}

private:
	SourceBlock* _source ;
	RepairJob* _repairJob ;
} ;

/*Constructor for Unplanned*/
SourceBlock::SourceBlock(Distribution* iat, string aircraftType, Aircraft* aircraft, string name, RepairJob* repairJob,
	int numberOfAircraftToGenerate) : Task(name)
{
	//	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl ;
	_name = name ;

	//cout << "Initializing Aircraft Distributions" << endl ;
	_interarrivalTimeRND = iat->CopyThis() ;

	//cout << "Printing Aircraft Distributions" << endl ;
	//_interarrivalTimeRND->PrintDistribution() ;

	_aircraftType = aircraftType ;
	_aircraft = aircraft ;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate ;
	_numberGenerated = 0 ;

	//cout << "Scheduling First Unplanned Aircraft Arrival" << endl ;
	SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new ScheduleNextUnplannedAircraftEA(this, repairJob),
		_interarrivalTimeRND->GetRV(), "ScheduleNextUnplannedAircraftEA") ;
}

/*Constructor for Recurring*/
//SourceBlock::SourceBlock(vector<Distribution*> recurringIAT, string aircraftType, Aircraft* aircraft, string name,
//	int numberOfAircraftToGenerate) : Task(name)
//{
//	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl ;
//	_name = name ;
//
//	cout << "Initializing Aircraft Distributions" << endl ;
//	/*need to take the map of recurring IATs and set multiple*/
//	//_interarrivalTimeRecurring = 
//	_interarrivalTimeRecurring = recurringIAT ;
//
//	cout << "Printing Aircraft Distributions" << endl ;
//
//	for (int i = 0 ; i < _interarrivalTimeRecurring.size() ; ++i) {
//		_interarrivalTimeRecurring[i]->PrintDistribution() ;
//	}
//
//	_aircraftType = aircraftType ;
//	_aircraft = aircraft ;
//	_numberOfAircraftToGenerate = numberOfAircraftToGenerate ;
//	_numberGenerated = 0 ;
//
//	cout << "Scheduling first Recurring Aircraft Arrival(s)" << endl ;
//	for (int i = 0 ; i < _interarrivalTimeRecurring.size() ; ++i) {
//		SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this),
//			_interarrivalTimeRecurring[i]->GetRV(), "ScheduleNextRecurringAircraftEA") ;
//	}
//}



//recurring constructor
SourceBlock::SourceBlock(map<string, Distribution*> recurringIATS, string aircraftType, Aircraft* aircraft, string name,
	int numberOfAircraftToGenerate) : Task(name)
{
	//	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl ;
	_name = name ;

	//	cout << "Initializing Aircraft Distributions" << endl ;
		/*need to take the map of recurring IATs and set multiple*/
	_recurringIATs = recurringIATS ;

	//	cout << "Printing Aircraft Distributions" << endl ;
	map<string, Distribution*>::const_iterator it = _recurringIATs.begin() ;
	//while (it != _recurringIATs.end()) {
	//	it->second->PrintDistribution() ;
	//	it++ ;
	//}

	_aircraftType = aircraftType ;
	_aircraft = aircraft ;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate ;
	_numberGenerated = 0 ;

	it = _recurringIATs.begin() ;
	//cout << "Scheduling first Recurring Aircraft Arrival(s)" << endl ;
	//for (int i = 0 ; i < _interarrivalTimeRecurring.size() ; ++i) {
	while (it != _recurringIATs.end()) {
		//map<string, RepairJob*>::const_iterator iter = aircraft->GetMyUnplannedRJMapBegin() ;
		/*while (iter != aircraft->GetMyUnplannedRJMapEnd()) {
			if (it->first == iter->first)
				break ;
			else {
				iter++ ;
			}
		}*/
		RepairJob* newptr = aircraft->GetMyRepairJobObj(it->first) ;
		SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this, it->second, newptr),
			it->second->GetRV(), "ScheduleNextRecurringAircraftEA") ;
		it++ ;
		//delete newptr ;
	}
}

/*Constructor for Calendar*/
//scheduling aircraft arrival
SourceBlock::SourceBlock(string aircraftType, Aircraft* aircraft, string name,
	/*int numCalEventsToSched,*/ CalendarObj* calobj, RepairJob* repairJob) : Task(name)
{
	_aircraft = aircraft ;
	_name = name ;
	//cout << "Scheduling Calendar Arrival" << endl ;
	//for (int i = 0 ; i < numCalEventsToSched ; ++i) {
	SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextCalendarAircraftEA(this, repairJob, calobj), 0.0, "ScheduleNextCalendarAircraftEA") ;
	//cout << "Calendar Event Scheduled" << endl ;
	//}
}

/*Old Constructor*/
//SourceBlock::SourceBlock(Distribution* interarrivalTimeRecurring, string aircraftType, Aircraft* aircraft, string name,
//	int numOfCalendarEventsToSchedule, CalendarObj* calobj, int numberOfAircraftToGenerate) : Task(name) {
//	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl ;
//	_name = name ;
//	cout << "Initializing Aircraft Distributions" << endl ;
//	_interarrivalTimeRecurring = interarrivalTimeRecurring ;
//	_interarrivalTimeRND = aircraft->GetAircraftIAT() ;
//	cout << "Printing Aircraft Distributions" << endl ;
//	_interarrivalTimeRecurring->PrintDistribution() ;
//	_interarrivalTimeRND->PrintDistribution() ;
//	_aircraftType = aircraftType ;
//	_aircraft = aircraft ;
//	_numberOfAircraftToGenerate = numberOfAircraftToGenerate ;
//	_numberGenerated = 0 ;
//	cout << "Scheduling Calendar Events" << endl ;
//	for (int i = 0 ; i < numOfCalendarEventsToSchedule ; ++i) {
//		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _aircraft->GetAircraftPriority(), new ScheduleNextCalendarAircraftEA(this), "ScheduleNextCalendarAircraftEA") ;
//		cout << "Calendar Event Scheduled" << endl ;
//	}
//	SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this), _interarrivalTimeRecurring->GetRV(), "ScheduleNextRecurringAircraftEA") ;
//}

string SourceBlock::GetAircraftType() {
	return _aircraftType ;
}

void  SourceBlock::SetAircraftType(string aircraftType) {
	_aircraftType = aircraftType ;
}

string SourceBlock::GetName() {
	return _name ;
}

int SourceBlock::GetNumberGenerated() {
	return _numberGenerated ;
}

void SourceBlock::ScheduleNextCalendarAircraftEM(RepairJob* repairJob, CalendarObj* calObj) {
	//cout << "****************** IN CALENDAR EM" << endl ;
	Aircraft* newAircraft = _aircraft->New() ;
	InputReader::CalAirFix() ;

	map<string, RepairJob*>::iterator it = newAircraft->GetMyRJMapBegin() ;
	//cout << "FOR AICRAFT "<< this->GetAircraftType() << " ID " << this->GetAircraftID() << endl ;
	while (it != newAircraft->GetMyRJMapEnd())
	{
		if (it->second->GetSchedType() == "Calendar")
		{
			RepairJob* currJob = new RepairJob() ;
			currJob->CopyRepairJob(*it->second) ;
			newAircraft->AddMyRepairJob(currJob->GetName(), currJob) ;

			//cout << "..................ADDED CAL " << it->second->GetName() << endl ;
			it++ ;
		}

		else if (it->second->GetSchedType() != "Calendar")
		{
			newAircraft->DeleteJob(it->first) ;
			it = newAircraft->GetMyRJMapBegin() ;
		}
		else
			it++ ;
	}
	//cout << newAircraft->GetAircraftType() << " ----------------CALENDAR MAP SIZE " << newAircraft->GetMyRJMapSize() << endl ;
	it = newAircraft->GetMyRJMapBegin() ;
	//cout << "Scheduling Calendar Aircraft to Arrive" << endl ;
	
	for (int i = 0 ; i < calObj->GetNumEvents() / 2 ; ++i) {
		Aircraft* firstStep = new Aircraft(*newAircraft) ;
		firstStep->UpdateList(*newAircraft) ;
		map<string, RepairJob*>::const_iterator iter = newAircraft->GetMyRJMapBegin() ;
		while (iter != newAircraft->GetMyRJMapEnd()) {
			if (iter->first == it->first) {}
			else
			{
				firstStep->DeleteJob(iter->first) ;
			}
			iter++ ;
		}
		repairJob = it->second ;
		InputReader::AddAirCount() ;
		//repairJob->GetFirstStep()->ScheduleCalendarStep(repairJob->GetFirstStep(), newAircraft, calObj) ;
		repairJob->GetFirstStep()->ScheduleCalendarStep(repairJob->GetFirstStep(), firstStep, calObj, i) ;
		_numberGenerated++ ;
		
		it++ ;
		Scribe::RecordAircraft(_aircraft->GetAircraftType()) ;
	}
	

	//}
}


void SourceBlock::ScheduleNextUnplannedAircraftEM(RepairJob* repairJob)
{
	if (_numberGenerated != _numberOfAircraftToGenerate)
	{
		//cout << "--------------------Scheduling unplanned arrival for " << _aircraft->GetAircraftType() << endl ;

		//cout << "Unplanned Aircraft has arrived, " ;
		//cout << "Scheduling next Unplanned Aircraft Arrival" << endl ;
		SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextUnplannedAircraftEA(this, repairJob), _interarrivalTimeRND->GetRV(), "ScheduleNextUnplannedAircraftEA") ;
		//cout << "Departing Unplanned Arrival" << endl ;
		Aircraft* newAircraft = _aircraft->New() ;
		newAircraft->ClearMyMap() ;
		//cout << "------------------CLEARED UNPLANNED MAP SIZE IS " << newAircraft->GetMyRJMapSize() << endl ;

		//newAircraft->GetMyRepairJobObj(newAircraft->GetAircraftType())->GetFirstStep()->ScheduleFirstStep(newAircraft->GetRepairJobObj(newAircraft->GetAircraftType())->GetFirstStep(), newAircraft) ;
		//_aircraft->GetMyRepairJobObj(_aircraft->GetAircraftType())->GetFirstStep(1)->ScheduleFirstStep(_aircraft->GetAircraftType()->GetFirstStep(1)->ScheduleFirstStep(), _aircraft->New()) ;
		//_aircraft->New()->CopyMyJobList(_aircraft->GetAircraftType()) ;

		int jobCounter = 0 ;
		string job ;
		/*For all unplanned repair jobs*/
		map<string, RepairJob*>::const_iterator iter = newAircraft->GetMyUnplannedMapBegin() ;
		 
		while (iter != newAircraft->GetMyUnplannedMapEnd())
		{

			if ((iter->second->WillSchedule() == false))
			{
				iter++ ;

				continue ;
			}

			////TESTING
			//////////////////////////////////
			/*Roll the dice*/
			else if (iter->second->WillSchedule() == true)
			{  
				//cout << "************* " << newAircraft->GetAircraftType() << " WILL SCHEDULE " << iter->first << endl ;

				//if its a job we're going to schedule, put it in a map based on priority
				AddToPriorityMap(iter->second->GetPriority(), iter->first) ;
				//cout << "*********** ADDING " << iter->first << " IT TO THE MAP " << endl ;

				RepairJob* currJob = new RepairJob() ;
				currJob->CopyRepairJob(*iter->second) ;
				newAircraft->AddMyRepairJob(currJob->GetName(), currJob) ;
				Scribe::TallyUnplanned(1) ;
				 
				//cout << "..................ADDED UNPLANNED " << iter->second->GetName() << endl ;
			}

			iter++ ;

		}

		//job is the job with highest priority
		job = _jobPriority.begin()->second ;

		//schedule that high priority job
		
		newAircraft->GetMyJobsMap().find(job)->second->GetFirstStep()->
		ScheduleFirstStep(newAircraft->GetMyJobsMap().find(job)->second->GetFirstStep(), newAircraft) ;
		//cout << " THIS IS AIR ID " << newAircraft->GetAircraftID() << " scheduling first step of " << job << endl ;
		/*if (newAircraft->GetAircraftID() == 384)
			cout << "Bleh" << endl ;*/

		jobCounter++ ;

		

		//cout << "i think i am here \n" ;
		//cout <<newAircraft->GetAircraftType() << " ----------------UNPLANNED MAP SIZE " << newAircraft->GetMyRJMapSize() << endl ;

		/*If no jobs are scheduled, must choose one randomly*/
		if (jobCounter == 0)
		{
			while (iter != newAircraft->GetMyUnplannedMapEnd())
			{
				/*Add name of every repair job in the list to vector for random selection*/
				newAircraft->InsertJobName(iter->first) ;

				//cout << "vector size is " << newAircraft->GetUnplanVecSize() ;
				iter++ ;
			}


			/*Pick a random element from the vector*/
			string selection = newAircraft->GetRandomElement() ;
			cout << "INDEX CHOSEN IS " << selection << endl ;

			/*Schedule this job*/
			map<string, RepairJob*>::iterator it = newAircraft->GetUnplanJobMap().find(selection) ;
			it->second->GetFirstStep()->ScheduleFirstStep(it->second->GetFirstStep(), newAircraft) ;

			RepairJob* currJob = new RepairJob() ;
			currJob->CopyRepairJob(*it->second) ;
			newAircraft->AddMyRepairJob(currJob->GetName(), currJob) ;
			

			//cout << "..................ADDED UNPLANNED RANDOMLY " << iter->second->GetName() << endl ;
		}

		
		//repairJob->GetFirstStep()->ScheduleFirstStep(repairJob->GetFirstStep(), newAircraft) ;
		//Depart(_aircraft->New()) ;
		_numberGenerated++ ;
		//****Issue with scribe call
		Scribe::RecordAircraft(_aircraft->GetAircraftType()) ;
		//cout << newAircraft->GetAircraftType() << " " << newAircraft->GetAircraftID() << " ./././././.NUMBER OF JOBS IS " << newAircraft->GetMyRJMapSize() << endl ;

	}
}


void SourceBlock::AddToPriorityMap(int priority, string job)
{
	_jobPriority[priority] = job ;
}

void SourceBlock::ScheduleNextRecurringAircraftEM(Distribution* recurringIAT, RepairJob* repairJob) {

	if (_numberGenerated != _numberOfAircraftToGenerate) {
		//Scheduling recurring aircrafts
	//	cout << "Recurring Aircraft has arrived, " ;
	//	cout << "Scheduling next Recurring Aircraft Arrival: " << repairJob->GetName() << endl ;
		SimExec::ScheduleEventAtRecurring(_aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this, recurringIAT, repairJob), recurringIAT->GetRV(), "ScheduleNextRecurringAircraftEA") ;
		Aircraft* newAircraft = _aircraft->New() ;

		//cout << "************IN RECUR EM AIRCRAFT " << endl ;
		map<string, RepairJob*>::iterator it = newAircraft->GetMyRJMapBegin() ;
		while (it != newAircraft->GetMyRJMapEnd())
		{
			//cout << "JOB IS " << it->first << endl ;
			if (it->second->GetSchedType() == "Recurring") {
				RepairJob* currJob = new RepairJob() ;
				currJob->CopyRepairJob(*it->second) ;
				newAircraft->AddMyRepairJob(currJob->GetName(), currJob) ;

				//cout << "..................ADDED Recur " << it->second->GetName() << endl ;
				it++ ;
			}
			else if (it->second->GetSchedType() != "Recurring")
			{
				newAircraft->DeleteJob(it->first) ;
				it = newAircraft->GetMyRJMapBegin() ;
			}
			else
				it++ ;
			//it++ ;
		}
		//cout << newAircraft->GetAircraftType() << " ----------------RECUR MAP SIZE " << newAircraft->GetMyRJMapSize() << endl ;

		//newAircraft->GetMyRepairJobObj(newAircraft->GetAircraftType())->GetFirstStep()->ScheduleFirstStep(newAircraft->GetRepairJobObj(newAircraft->GetAircraftType())->GetFirstStep(), newAircraft) ;
		repairJob->GetFirstStep()->ScheduleFirstRecurringStep(repairJob->GetFirstStep(), newAircraft) ;
		//No longer doing this!
		//cout << "Scheduling next Random Aircraft arrival" << endl ;
		//SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomAircraftEA") ;
		//cout << "Departing new Aircraft" << endl ;
		//Depart(_aircraft->New()) ;
		_numberGenerated++ ;
		Scribe::RecordAircraft(_aircraft->GetAircraftType()) ;
	}
}

void SourceBlock::Execute(Aircraft* aircraft) {
	//This is just a declaration of the virtual function doesn't do anything
}