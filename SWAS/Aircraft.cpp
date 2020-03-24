//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"
#include "InputInterface.h"

int Aircraft::_nextID = 0;

Aircraft::Aircraft()
{
	_aircraftID = ++_nextID;

}

//void Aircraft::CopyAircraftInfo(const Aircraft& mapAircraft)
Aircraft::Aircraft(const Aircraft& mapAircraft)
{
	_aircraftType = mapAircraft._aircraftType;						//	Aircraft name: i.e. F35
	_priority = mapAircraft._priority;								//	Aircraft priority: helps determine first to be served in queues
	_numCalEvents = mapAircraft._numCalEvents;
	_length = mapAircraft._length;									//	Aircraft size x dimension  ***make a tuple or struct later
	_wingspan = mapAircraft._wingspan;								//	Aircraft size y dimension
	_repairJobName = mapAircraft._repairJobName;
	_iatUnplanned = mapAircraft._iatUnplanned;						//	Distribution for random aircraft interarrival times (iat) 

	//recurIat vector, calobj and others populated after this copy
}

void Aircraft::CopyMyJobList(string aircraftType)
{
	_aircraftType = aircraftType;

	//cout << "Master map's aircraft version all repair map size" << InputReader::GetAircraft(aircraftType)->GetAllRJMapSize() << endl;

	//iterate through all repair jobs in _allrepairJobs map in mastermap
	map<string, RepairJob*>::iterator iter = InputReader::GetAircraft(_aircraftType)->GetRJMapBegin();

	while (InputReader::GetAircraft(_aircraftType)->IsMapEnd(iter))
	{
		//for all repair jobs in allrepairjobs in mastermap with schedule type "random"
		if (iter->second->GetSchedType() == "Unplanned") {

			//	cout << "got unplanned job" << endl;

				//roll dice on unplanned probability to see if we're going to schedule it
			if (iter->second->WillSchedule() == true)
			{
				cout << endl;
				cout << _aircraftType << "	will have a Random Repair Job called: " << iter->second->GetName() << endl;

				//make a copy of this repair job
				RepairJob* currJob = new RepairJob();
				currJob->CopyRepairJob(*iter->second);

				//give this copy to this new aircraft's myrepairjobs list
				_myRepairJobs.insert(pair<string, RepairJob*>(iter->second->GetName(), currJob));

				//			cout << "NEW my own repair job map size:	" << GetMyRJMapSize() << endl;
				//			cout << "Copied Job Is:	" << currJob->GetName() << endl;
				cout << "Vec Step Size Is	" << currJob->GetStepVecSize() << endl;

				map<string, RepairJob*>::const_iterator myJobIter = _myRepairJobs.begin();
				while (myJobIter != _myRepairJobs.end())
				{
					//////	schedule first step of this repair job
					Step* firstStep = myJobIter->second->GetStep(1);
					myJobIter->second->GetStep(1)->ScheduleFirstStep(firstStep, this);
					myJobIter++;
				}


			}

			else
				cout << _aircraftType << "	will not have a Random Repair Job called: " << iter->second->GetName() << endl;
		}

		//for all repair jobs with schedule type "recurring"
		else if (iter->second->GetSchedType() == "Recurring") {

			cout << endl;
			cout << _aircraftType << "	has a Recurring Repair Job called: " << iter->second->GetName() << endl;

			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*iter->second);
			//give this copy to this new aircraft's myrepairjobs list
			_myRepairJobs.insert(pair<string, RepairJob*>(iter->second->GetName(), currJob));

			//			cout << "NEW my own repair job map size:	" << GetMyRJMapSize() << endl;
//			cout << "Copied Recurring Job Is:	" << currJob->GetName() << endl;
			cout << "Vec Step Size Is	" << currJob->GetStepVecSize() << endl;
			cout << endl;

			map<string, RepairJob*>::const_iterator myJobIter = _myRepairJobs.begin();
			while (myJobIter != _myRepairJobs.end())
			{
				//////	schedule first step of this repair job
				Step* firstStep = myJobIter->second->GetStep(1);
				myJobIter->second->GetStep(1)->ScheduleFirstStep(firstStep, this);
				myJobIter++;
			}
		}

		//for all repair jobs with schedule type "calendar"
		else if (iter->second->GetSchedType() == "Calendar") {

			cout << endl;
			cout << _aircraftType << "	has a Calendar Repair Job called: " << iter->second->GetName() << endl;

			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*iter->second);
			//give this copy to this new aircraft's myrepairjobs list
			_myRepairJobs.insert(pair<string, RepairJob*>(iter->second->GetName(), currJob));
			
			SetCalendarObj(currJob->GetCalendarDate());
			//			cout << "NEW my own repair job map size:	" << GetMyRJMapSize() << endl;
//			cout << "Copied Recurring Job Is:	" << currJob->GetName() << endl;
			//cout << "Vec Step Size Is	" << currJob->GetStepVecSize() << endl;
			//cout << endl;

			map<string, RepairJob*>::const_iterator myJobIter = _myRepairJobs.begin();
			while (myJobIter != _myRepairJobs.end())
			{
				//	schedule first step of this repair job
				Step* firstStep = myJobIter->second->GetStep(1);
				myJobIter->second->GetStep(1)->ScheduleFirstStep(firstStep, this);
				myJobIter++;
			}
			
		}
		iter++;
	}
}

void Aircraft::SetCalendarObj(string date)
{
	Time month;
	Time day;
	int year;

	cout << "STRING " << date << endl;
	istringstream calDate(date);
	//calDate >> month >> delim >> day >> delim >> 

	string first;
	string sec;
	string third;

	getline(calDate, first, '-');
	getline(calDate, sec, '-');
	getline(calDate, third, '-');

	cout << "*********************" << endl;
	cout << "FIRST " << first << endl;
	cout << "SEC " << sec << endl;
	cout << "THIRD " << third << endl;
	cout << endl;
	cout << endl;
	cout << endl;

	//istringstream num1(first);
	//istringstream num2(sec);
	//istringstream num3(third);

	//num1 >> month;
	//num2 >> day;
	//num3 >> year;


	//_myCalObj->_months.push_back(month);
	//_myCalObj->_days.push_back(day);
	//_myCalObj->_year.push_back(year);
	//_myCalObj->_timeOfDays.push_back(0.0);
}


void Aircraft::SetNextStep(Aircraft* currAir, RepairJob* currJob, int stepID)
{
	
}

bool Aircraft::IsMapEnd(map<string, RepairJob*>::const_iterator iter)
{
	if (iter != _allRepairJobsMap.end())
		return true;
	return false;
}

bool Aircraft::AreMoreJobs()
{
	map<string, RepairJob*>::const_iterator iter = _myRepairJobs.begin();
	if (iter != _myRepairJobs.end())
		return true;
	return false;
}

bool Aircraft::AreMoreSteps()
{
//	iter = _myRepairJobs.begin();
//	for (int i = 0; i < iter->second->GetStepVecSize(); i++)
//	
//		if (i != iter->second->GetStepVecSize()-1)
			return true;
//		return false;
//	}
//
}

void Aircraft::SetNumCalEvents(int numCalEvents)
{
	_numCalEvents = numCalEvents;
}

CalendarObj* Aircraft::GetCalendarObj()
{
	return _myCalObj;
}

RepairJob* Aircraft::GetNextRepairJob(string rjName)
{
	RepairJob* nextJob = this->GetRepairJobObj(rjName);
//
//	int highPriority = INT_MAX;
//	int myPriority = this->GetRepairJobObj(rjName)->GetPriority();
//
//	map<string, RepairJob*>::const_iterator iter = _myRepairJobs.begin();
//		while (iter != _myRepairJobs.end())
//		{
//
//			if (iter->second->GetPriority() > myPriority)
//			{
//				if (iter->second->GetPriority() < highPriority)
//				{
//					highPriority = iter->second->GetPriority();
//
//					nextJob = iter->second;
//				}
//			}
//
//			iter++;
//		}
//
		return nextJob;// need to set
		
	/*aircraft function recieves rpeiajob name
		get the repair jobobject
		get its priority
		iterate through mymap
		find next repairjob with lowest priority
		outside loop declar varfaible to track priority
		keep track of variable at each loop iterateion*/
}

Step* Aircraft::GetNextStep()
{
	//need to take arguments of aircraft, repairjob and stepid 
		
		//if stepid + 1 > container size
			//get next step
		//else 
			//check if there are more repair jobs? (bool in aircraft for more jobs
				//if yes, get next repair job
					//get first step
	return _nextStep;
}

void Aircraft::SetSource(int sourceID)
{
	_sourceID = sourceID;
}

int Aircraft::GetSource()
{
	return _sourceID;
}

int Aircraft::GetAircraftID()
{
	return _aircraftID;
}

int Aircraft::GetNextAircraftID()
{
	return _nextID;
}

Aircraft* Aircraft::New()
{
	return new Aircraft(); // add appropriate parameters
}

void Aircraft::SetAircraftFootprint(double length, double wingspan)
{
	_length = length;
	_wingspan = wingspan;
}

double Aircraft::GetAircraftFootprint()
{
	return _length, _wingspan;
}

void Aircraft::SetAircraftType(string aircraftType)
{
	_aircraftType = aircraftType;
	//	cout << "Aircraft Type: " << aircraftType << endl;
}


string Aircraft::GetAircraftType()
{
	return _aircraftType;
}

vector<Distribution*> Aircraft::GetRecurringIATs()
{
	return _recurIatVec;
}


void Aircraft::AddRepairJob(RepairJob* repairJob, string repairJobName)
{
	_allRepairJobsMap[repairJobName] = repairJob;
}

void Aircraft::AddRandRepairJob()
{
}

void Aircraft::AddSchedRepairJob()
{
}

int Aircraft::GetNumCalEvents()
{
	return _numCalEvents;
}


RepairJob* Aircraft::GetRepairJobObj(string name)
{
	map<string, RepairJob*>::iterator it = _allRepairJobsMap.find(name);
	if (it == _allRepairJobsMap.end())
		return nullptr;
	return it->second;
}

void Aircraft::SetAircraftPriority(int priority)
{
	_priority = priority;
	//	cout << "priority: " << priority << endl;
}

int Aircraft::GetAircraftPriority()
{
	return _priority;
}

map<string, RepairJob*>::iterator Aircraft::GetRJMapBegin()
{
	return _allRepairJobsMap.begin();
}

map<string, RepairJob*>::iterator  Aircraft::GetRJMapEnd()
{
	return _allRepairJobsMap.end();
}

int Aircraft::GetAllRJMapSize()
{
	return _allRepairJobsMap.size();
}

int Aircraft::GetMyRJMapSize()
{
	return _myRepairJobs.size();
}

void Aircraft::SetAircraftIAT(string iatUnplanned)
{
	istringstream iatUn(iatUnplanned);
	string firstHalf;
	string secHalf;

	getline(iatUn, firstHalf, '(');
	getline(iatUn, secHalf, ')');

	istringstream nums(secHalf);
	if (firstHalf == "Triangular" || firstHalf == "Tri")
	{
		double min, expected, max;
		nums >> min;
		nums >> expected;
		nums >> max;
		_iatUnplanned = new Triangular(min, expected, max);
	}

	else if (firstHalf == "Exponential")
	{
		double mean;
		nums >> mean;

		_iatUnplanned = new Exponential(mean);
	}

	else if (firstHalf == "Uniform")
	{
		double min, max;
		nums >> min >> max;

		_iatUnplanned = new Uniform(min, max);
	}

	else if (firstHalf == "Normal")
	{
		double mean, stdev;
		nums >> mean >> stdev;

		_iatUnplanned = new Normal(mean, stdev);
	}

	else if (firstHalf == "Poisson")
	{
		double mean;
		nums >> mean;

		_iatUnplanned = new Poisson(mean);
	}

	else if (firstHalf == "Constant")
	{
		double mean;
		nums >> mean;

		_iatUnplanned = new Constant(mean);
	}

	else if (firstHalf == "Weibull")
	{
		double scale, shape;
		nums >> scale >> shape;

		_iatUnplanned = new Weibull(scale, shape);
	}

	//Determines correct distribution and prints
//	_iatUnplanned->PrintDistribution();
}

Distribution* Aircraft::GetAircraftIAT()
{
	return _iatUnplanned; //check if this works?
	_iatUnplanned->PrintDistribution();
}

void Aircraft::PrintProperties()
{
	cout << "AIRCRAFT:" << endl;
	cout << "Aircraft Type: " << _aircraftType << endl;
	//cout << "Aircraft Priority: " << _priority << endl;
	//cout << "Aircraft Length: " << _length << endl;
	//cout << "Aircraft Wingspan: " << _wingspan << endl;
	//cout << endl;
	//cout << "Repair Jobs:" << endl;
	cout << endl;

	map<string, RepairJob*>::iterator it = _allRepairJobsMap.begin();
	//cout << "After creating the iterator to the map " << std::endl; 
	while (it != _allRepairJobsMap.end())
	{
		//cout << "in the loop \n";

		it->second->PrintJobProperties();
		it++;
	}
}