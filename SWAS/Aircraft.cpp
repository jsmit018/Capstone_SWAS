//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"

int Aircraft::_nextID = 0;

Aircraft::Aircraft()
{

}

//void Aircraft::CopyAircraftInfo(const Aircraft& mapAircraft)
Aircraft::Aircraft(const Aircraft& mapAircraft)
{
	_aircraftID = ++_nextID;

	_aircraftType = mapAircraft._aircraftType;						//	Aircraft name: i.e. F35
	_priority = mapAircraft._priority;								//	Aircraft priority: helps determine first to be served in queues
	_numCalEvents = mapAircraft._numCalEvents;
	_length = mapAircraft._length;									//	Aircraft size x dimension  ***make a tuple or struct later
	_wingspan = mapAircraft._wingspan;								//	Aircraft size y dimension
	_repairJobName = mapAircraft._repairJobName;
	_baySizeReq = mapAircraft._baySizeReq;
	//cout << "MY UNPLANNED IAT " << _aircraftType << endl; 
	//mapAircraft._iatUnplanned->PrintDistribution();
	_iatUnplanned = mapAircraft._iatUnplanned->CopyThis();
	_myUnplannedJobsMap = mapAircraft._myUnplannedJobsMap;
	
	//Initialize CalendarObj
	_myCalObj = new CalendarObj();


	//iterating through old recurring iat map, inserting its first and second into the new recurring iat map
	map<string, Distribution*>::const_iterator recurIter = mapAircraft._myRecurIATmap.begin();
	while (recurIter != mapAircraft._myRecurIATmap.end())
	{
		Distribution* newDist = recurIter->second->CopyThis();
		_myRecurIATmap.insert(pair<string, Distribution*>(recurIter->first, newDist));

		recurIter++;
	}


	//cout << endl;
	//cout << endl;
	//cout << _aircraftType << " BAYSIZE REQ " << _baySizeReq << endl;
	//// << "AIRCRAFT IS: " << _aircraftType << " HAS REcur IAT SIZE OF " << _myRecurIATmap.size() << endl;
	//cout << endl;
	//cout << endl;



	//recurIat vector, calobj and others populated after this copy
}

//
//bool Aircraft::HasRecurJob()
//{
//	if (_recurFlag == 'Y')
//	{
//		cout << "recurring flag is y" << endl;
//		return true;
//	}
//
//	else if (_recurFlag == 'N')
//	{
//		cout << "recurring flag is n" << endl;
//
//		return false;
//	}
//}
//
//void Aircraft::SetRecurFlag(char flag)
//{
//	_recurFlag = flag;
//	cout << "setting flag " << _recurFlag << endl;
//}
//
//char Aircraft::GetRecurFlag()
//{
//	return _recurFlag;
//}

//Copy all repair jobs
void Aircraft::CopyMyJobList(string aircraftType)
{
	_aircraftType = aircraftType;


	//cout << "Master map's aircraft version all repair map size" << InputReader::GetAircraft(aircraftType)->GetAllRJMapSize() << endl;

	//iterate through all repair jobs in _allrepairJobs map in mastermap
	map<string, RepairJob*>::iterator iter = InputReader::GetAircraft(_aircraftType)->GetRJMapBegin();

	while (InputReader::GetAircraft(_aircraftType)->IsMapEnd(iter))
	{
		//for all repair jobs in _allrepairjobs in mastermap with schedule type "Unplanned"
		if (iter->second->GetSchedType() == "Unplanned") {

			// cout << "got unplanned job" << endl;
			///* roll dice on unplanned probability to see if we're going to schedule it */
			//if (iter->second->WillSchedule() == true)
			//{
		//	cout << _aircraftType << "	will have a Random Repair Job called: " << iter->second->GetName() << endl;

			//make a copy of this repair job
			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*iter->second);

			//give this copy to this new aircraft's myrepairjobs list
	//		cout << "ABOUT TO ADD JOB " << currJob->GetName() << " TO" << this->GetAircraftType() << "MY LIST." << endl;
			this->AddMyRepairJob(currJob->GetName(), currJob);

			//add to map of only unplanned jobs
			this->AddMyUnplannedJob(currJob->GetName(), currJob);

			//_myRepairJobs.insert(pair<string, RepairJob*>(iter->second->GetName(), currJob));


			//cout << "NEW my own repair job map size:	" << GetMyRJMapSize() << endl;
			//cout << "Copied Job Is:	" << currJob->GetName() << endl;
			//cout << "Vec Step Size Is	" << currJob->GetStepVecSize() << endl;

			//map<string, RepairJob*>::const_iterator myJobIter = _myRepairJobs.begin();
			//while (myJobIter != _myRepairJobs.end())
			//{
			//	//////	schedule first step of this repair job
			//	Step* firstStep = myJobIter->second->GetStep(1);
			//	myJobIter->second->GetStep(1)->ScheduleFirstStep(firstStep, this);
			//	myJobIter++;
		//	//}
		//}

		//else
		//	cout << _aircraftType << "	will not have a Random Repair Job called: " << iter->second->GetName() << endl;
		}

		//for all repair jobs with schedule type "recurring"
		else if (iter->second->GetSchedType() == "Recurring") {

		//	cout << endl;
		//	cout << _aircraftType << "	has a Recurring Repair Job called: " << iter->second->GetName() << endl;

			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*iter->second);
			//give this copy to this new aircraft's myrepairjobs list
			//_myRepairJobs.insert(pair<string, RepairJob*>(iter->second->GetName(), currJob));
	//		cout << "ABOUT TO ADD JOB " << currJob->GetName() << " TO" << this->GetAircraftType() << "MY LIST." << endl;

			this->AddMyRepairJob(currJob->GetName(), currJob);

			_myRecurIATmap.insert(pair<string, Distribution*>(iter->second->GetName(), iter->second->GetRecurringAmt()));

			//note to self [andie]: need to check that multiple are stored in map
			//cout << "****MY RECURRING STUFF: " << iter->second->GetName() << " DISTRIBUTION ";
			//iter->second->GetRecurringAmt()->PrintDistribution();
			//cout << endl;

			//			cout << "NEW my own repair job map size:	" << GetMyRJMapSize() << endl;
//			cout << "Copied Recurring Job Is:	" << currJob->GetName() << endl;
	//		cout << "Vec Step Size Is	" << currJob->GetStepVecSize() << endl;
	//		cout << endl;

			//map<string, RepairJob*>::const_iterator myJobIter = _myRepairJobs.begin();
			//while (myJobIter != _myRepairJobs.end())
			//{
			//	//////	schedule first step of this repair job
			//	Step* firstStep = myJobIter->second->GetStep(1);
			//	//myJobIter->second->GetStep(1)->ScheduleFirstStep(firstStep, this);
			//	//Testing
			//	//------------
			//	myJobIter->second->GetStep(1)->ScheduleFirstRecurringStep(firstStep, this);
			//	//------------
			//	myJobIter++;
			//}
		}

		//for all repair jobs with schedule type "calendar"
		else if (iter->second->GetSchedType() == "Calendar") {

		//	cout << endl;
		//	cout << _aircraftType << "	has a Calendar Repair Job called: " << iter->second->GetName() << endl;

			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*iter->second);
			//give this copy to this new aircraft's myrepairjobs list
			//_myRepairJobs.insert(pair<string, RepairJob*>(iter->second->GetName(), currJob));
		//	cout << "ABOUT TO ADD JOB " << currJob->GetName() << " TO" << this->GetAircraftType() << "MY LIST." << endl;

			this->AddMyRepairJob(currJob->GetName(), currJob);

			SetCalendarObj(currJob->GetCalendarDate());
			//			cout << "NEW my own repair job map size:	" << GetMyRJMapSize() << endl;
//			cout << "Copied Recurring Job Is:	" << currJob->GetName() << endl;
			//cout << "Vec Step Size Is	" << currJob->GetStepVecSize() << endl;
			//cout << endl;

			//map<string, RepairJob*>::const_iterator myJobIter = _myRepairJobs.begin();
			//while (myJobIter != _myRepairJobs.end())
			//{
			//	//	schedule first step of this repair job
			//	Step* firstStep = myJobIter->second->GetStep(1);
			//	//myJobIter->second->GetStep(1)->ScheduleFirstStep(firstStep, this);
			//	//Testing
			//	//------------
			//	myJobIter->second->GetStep(1)->SceduleCalendarStep(firstStep, this, _myCalObj);
			//	//------------
			//	myJobIter++;
			//}
		}
		iter++;
	}
}


void Aircraft::AddRecurIAT(string repairJobName, Distribution* iatRecurring)
{
	_myRecurIATmap[repairJobName] = iatRecurring;
	//_myRecurIATmap.insert(pair<string, Distribution*>(iter->second->GetName(), iter->second->GetRecurringAmt()));

}

void Aircraft::SetCalendarObj(string date)
{
	Time month;
	Time day;
	int year;

	//	cout << "STRING " << date << endl;
	istringstream calDate(date);
	//calDate >> month >> delim >> day >> delim >> 

	string first;
	string sec;
	string third;

	getline(calDate, first, '/');
	getline(calDate, sec, '/');
	getline(calDate, third, '/');

	//	cout << "*********************" << endl;
	//	cout << "FIRST " << first << endl;
	//	cout << "SEC " << sec << endl;
	//	cout << "THIRD " << third << endl;
		//cout << endl;
		//cout << endl;
		//cout << endl;

	istringstream num1(first);
	istringstream num2(sec);
	istringstream num3(third);

	num1 >> month;
	num2 >> day;
	num3 >> year;

	//cout << "NEW MONTH IS " << month << endl;
	//cout << "NEW DAY IS " << day << endl;
	//cout << "NEW YEAR IS " << year << endl;


	/*Jordan: These throw exceptions during runtime: */

	// 3/30: Fixed runtime issues with _myCalObj push_back. [Jordan]
	//**Issue: Throws read access error
	//**Solution: _myCalObj was not instantiated.

	_myCalObj->_months.push_back(month - 1);
	_myCalObj->_days.push_back(day - 1);
	_myCalObj->_year.push_back(year);
	_myCalObj->_timeOfDays.push_back(0.0);
	_myCalObj->UpdateNumEvents();
}


//void Aircraft::SetNextStep(Aircraft* currAir, RepairJob* currJob, int stepID)
//{
//	
//}

void Aircraft::InsertJobName(string jobName)
{
	_unplannedRjVec.push_back(jobName);
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

void Aircraft::SetBaySizeReq(string baySizeReq)
{
	_baySizeReq = baySizeReq;
}

string Aircraft::GetBaySizeReq()
{
	return _baySizeReq;
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
	//receives current repair job name

	//get the object of that repair job
	RepairJob* nextJob = this->GetRepairJobObj(rjName);
	//variable for tracking priorities
	int highPriority = INT_MAX;
	//get its priority
	int myPriority = this->GetRepairJobObj(rjName)->GetPriority();

	//iterate through the map
	map<string, RepairJob*>::const_iterator iter = _myRepairJobs.begin();
	while (iter != _myRepairJobs.end())
	{
		//if next repairjob has lower priority (higher number)
		if (iter->second->GetPriority() > myPriority)
		{
			//if next repairjob has higher priority (lower number) than current high priority
			if (iter->second->GetPriority() < highPriority)
			{
				//this priority becomes the highest priority 
				highPriority = iter->second->GetPriority();

				//this one is the next job
				nextJob = iter->second;
			}
		}

		iter++;
	}

	return nextJob; //set this??

}

Step* Aircraft::GetNextStep()
{
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

void Aircraft::SetAircraftID(int id)
{
	_aircraftID = id;
}

int Aircraft::GetNextAircraftID()
{
	return _nextID;
}

Aircraft* Aircraft::New()
{
	Aircraft* newAircraft = new Aircraft(*this);
	return newAircraft; // add appropriate parameters
	//return new Aircraft();
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

//vector<Distribution*> Aircraft::GetRecurringIATs()
//{
//	return _recurIatVec;
//}


void Aircraft::AddRepairJob(RepairJob* repairJob, string repairJobName)
{
	_allRepairJobsMap[repairJobName] = repairJob;
}

void Aircraft::AddMyRepairJob(string jobName, RepairJob* myJob)
{
	_myRepairJobs.insert(pair<string, RepairJob*>(jobName, myJob));
}

void Aircraft::AddMyUnplannedJob(string jobName, RepairJob* myJob)
{
	_myUnplannedJobsMap.insert(pair<string, RepairJob*>(jobName, myJob));
}

RepairJob* Aircraft::GetRepairJobObj(string name)
{
	map<string, RepairJob*>::iterator it = _allRepairJobsMap.find(name);
	if (it == _allRepairJobsMap.end())
		return nullptr;
	return it->second;
}

map<string, RepairJob*> Aircraft::GetUnplanJobMap()
{
	return _myUnplannedJobsMap;
}

RepairJob* Aircraft::GetMyRepairJobObj(string name)
{
	map<string, RepairJob*>::iterator it = _myRepairJobs.find(name);
	if (it == _myRepairJobs.end())
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

map<string, RepairJob*>::iterator Aircraft::GetMyRJMapBegin()
{
	return _myRepairJobs.begin();
}

map<string, RepairJob*>::iterator  Aircraft::GetMyRJMapEnd()
{
	return _myRepairJobs.end();
}

map<string, RepairJob*>::iterator Aircraft::GetMyUnplannedMapBegin()
{
	return _myUnplannedJobsMap.begin();
}

map<string, RepairJob*>::iterator  Aircraft::GetMyUnplannedMapEnd()
{
	return _myUnplannedJobsMap.end();
}

map<string, Distribution*>::iterator Aircraft::GetRecurMapBegin()
{
	return _myRecurIATmap.begin();
}

map<string, Distribution*>::iterator  Aircraft::GetRecurMapEnd()
{
	return _myRecurIATmap.end();
}

string Aircraft::GetRandomElement()
{

	random_device random_device;
	mt19937 engine{ random_device() };
	uniform_int_distribution<int> dist(0, GetUnplanVecSize() - 1);
	string random_element = _unplannedRjVec[dist(engine)];

	return random_element;
}

int Aircraft::GetUnplanVecSize()
{
	return _unplannedRjVec.size();
}

int Aircraft::GetAllRJMapSize()
{
	return _allRepairJobsMap.size();
}

int Aircraft::GetMyRJMapSize()
{
	return _myRepairJobs.size();
}

int Aircraft::GetMyUnplannedMapSize()
{

	cout << "MY UNPLANNED RJ MAP SIZE IS ";
	cout << _myUnplannedJobsMap.size();
	cout << endl;

	return _myUnplannedJobsMap.size();
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
	//_iatUnplanned->PrintDistribution();
}
map<string, Distribution*> Aircraft::GetRecurIatMap()
{
	return _myRecurIATmap;
}

void Aircraft::PrintProperties()
{
	cout << "AIRCRAFT:" << endl;
	cout << "Aircraft Type: " << _aircraftType << endl;
	cout << "Aircraft Priority: " << _priority << endl;
	cout << "Aircraft Length: " << _length << endl;
	cout << "Aircraft Wingspan: " << _wingspan << endl;
	cout << "Aircraft's Unplanned Arrival ";
	_iatUnplanned->PrintDistribution();
	cout << "Aircraft's Recurring Arrivals";
	map<string, Distribution*>::const_iterator iter = _myRecurIATmap.begin();
	while (iter != _myRecurIATmap.end())
	{
		iter->second->PrintDistribution();
		iter++;
	}
	cout << endl;
	//cout << "Repair Jobs:" << endl;
	cout << endl;

	map<string, RepairJob*>::iterator it = _allRepairJobsMap.begin();
	//cout << "After creating the iterator to the map " << std::endl; 
	while (it != _allRepairJobsMap.end())
	{
		cout << "in the loop \n";

		it->second->PrintJobProperties();
		it++;
	}
}

void Aircraft::PrintMyProperties()
{
	cout << "AIRCRAFT:" << endl;
	cout << "Aircraft Type: " << _aircraftType << endl;
	cout << "Aircraft Priority: " << _priority << endl;
	cout << "Aircraft Length: " << _length << endl;
	cout << "Aircraft Wingspan: " << _wingspan << endl;
	cout << "Aircraft's Unplanned Arrival: " << endl;
	cout << "		";
	_iatUnplanned->PrintDistribution();
	cout << "Aircraft's Recurring Arrivals: " << endl;

	map<string, RepairJob*>::const_iterator iter = _myRepairJobs.begin();
	while (iter != _myRepairJobs.end())
	{
		cout << "		";
		iter->second->GetRecurringAmt()->PrintDistribution();
		cout << endl;
		iter++;
	}
	cout << endl;
	//cout << "Repair Jobs:" << endl;
	cout << endl;

	cout << "MY JOB LIST SIZE IS " << _myRepairJobs.size() << endl;
	map<string, RepairJob*>::iterator it = _myRepairJobs.begin();
	//cout << "After creating the iterator to the map " << std::endl; 
	while (it != _myRepairJobs.end())
	{
		//cout << "in the loop \n";

//		it->second->PrintJobProperties();
		it++;
	}
}