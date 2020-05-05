//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"

/**
 * Sets nextID equal to zero
 */
int Aircraft::_nextID = 0;

/**
 * Sets airCount equal to zero
 */
int _airCount = 0;

/**
 * Sets CELflag equal to zero
 */
int _CELflag = 0;

/**
 * Aircraft constructor
 */
Aircraft::Aircraft()
{

}

//void Aircraft::CopyAircraftInfo(const Aircraft& mapAircraft)

/**
 * Aircraft constructor that sets _aircraftType, _priority, _numCalEvents, _length, _wingspan, _repairJobName, and _iatUnplanned are set to according mapAircraft. CalendarObj is initialized. Iterates through old recurring iat map, inserting its first and second into the new recurring iat map.
 */
Aircraft::Aircraft(const Aircraft& mapAircraft)
{
	int _IDCount = InputReader::GetIDcount();
	_aircraftID = ++_nextID;
	_aircraftID = _aircraftID - _IDCount;

	//cout << " IN AIRCRAFT COPY CONSTRUCTOR " << endl; 
	//cout << "AIRCRAT ID IS " << _aircraftID << endl; 

	_aircraftType = mapAircraft._aircraftType; 						//	Aircraft name: i.e. F35
	_priority = mapAircraft._priority; 								//	Aircraft priority: helps determine first to be served in queues
	_numCalEvents = mapAircraft._numCalEvents;
	_length = mapAircraft._length; 									//	Aircraft size x dimension  ***make a tuple or struct later
	_wingspan = mapAircraft._wingspan; 								//	Aircraft size y dimension
	_repairJobName = mapAircraft._repairJobName;
	_baySizeReq = mapAircraft._baySizeReq;
	_CELflag = mapAircraft._CELflag;
	//cout << "MY UNPLANNED IAT " << _aircraftType << endl;
	//mapAircraft._iatUnplanned->PrintDistribution(); 
	_iatUnplanned = mapAircraft._iatUnplanned->CopyThis();
	//_myUnplannedJobsMap = mapAircraft._myUnplannedJobsMap; 

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

	CopyMyJobList(_aircraftType);
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

/**
 * Copies Job list from Master map to aircraft's myrepairjobs list logic.
 */
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

			//make a copy of this repair job
			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*iter->second);

			//give this copy to this new aircraft's myrepairjobs list
			this->AddMyRepairJob(currJob->GetName(), currJob);

			//add to map of only unplanned jobs
			this->AddMyUnplannedJob(currJob->GetName(), currJob);
		}

		//for all repair jobs with schedule type "recurring"
		else if (iter->second->GetSchedType() == "Recurring") {

			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*iter->second);

			this->AddMyRepairJob(currJob->GetName(), currJob);

			_myRecurIATmap.insert(pair<string, Distribution*>(iter->second->GetName(), iter->second->GetRecurringAmt()));
		}

		//for all repair jobs with schedule type "calendar"
		else if (iter->second->GetSchedType() == "Calendar") {


			RepairJob* currJob = new RepairJob();
			currJob->CopyRepairJob(*iter->second);

			this->AddMyRepairJob(currJob->GetName(), currJob);

			SetCalendarObj(currJob->GetCalendarDate());

			AddBayReqToRes();
		}
		iter++;
	}

}

/**
 * Adds a recurring IAT for a repair job to an aircrafts myRecurIATmap
 */
void Aircraft::AddRecurIAT(string repairJobName, Distribution* iatRecurring)
{
	_myRecurIATmap[repairJobName] = iatRecurring;
	//_myRecurIATmap.insert(pair<string, Distribution*>(iter->second->GetName(), iter->second->GetRecurringAmt())); 

}

/**
 * Sets the CalendarObj for an aircraft and declares date variables
 */
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

	istringstream num1(first);
	istringstream num2(sec);
	istringstream num3(third);

	num1 >> month;
	num2 >> day;
	num3 >> year;

	_myCalObj->_months.push_back(month - 1);
	_myCalObj->_days.push_back(day - 1);
	_myCalObj->_year.push_back(year);
	_myCalObj->_timeOfDays.push_back(0.0);
	_myCalObj->UpdateNumEvents();
}

/**
 * Function that inserts the job name to the repair job. 
 */
void Aircraft::InsertJobName(string jobName)
{
	_unplannedRjVec.push_back(jobName);
}

/**
 * Bool function that states whether or not an aircraft is at the end of its RepairJobsMap.
 */
bool Aircraft::IsMapEnd(map<string, RepairJob*>::const_iterator iter)
{
	if (iter != _allRepairJobsMap.end())
		return true;
	return false;
}

/**
 * Bool function that states whether or not an aircraft has more repair jobs.
 */
bool Aircraft::AreMoreJobs()
{
	map<string, RepairJob*>::const_iterator iter = _myRepairJobs.begin();
	if (iter != _myRepairJobs.end())
		return true;
	return false;
}

/**
 * Sets the CEL flag.
 */
void Aircraft::SetCELflag(int CELflag)
{
	_CELflag = CELflag;
}

/**
 * Bool function that returns true or false if after CEL.
 */
bool Aircraft::IsAfterCEL()
{
	if (_CELflag == 1)
		return true;
	if (_CELflag == 0)
		return false;
}

/**
 * Bool function that returns true that there are more steps to an aircrafts repair job.
 */
bool Aircraft::AreMoreSteps()
{
	return true;
}

/**
 * Sets the aircrafts required bay size.
 */
void Aircraft::SetBaySizeReq(string baySizeReq)
{
	_baySizeReq = baySizeReq;
}

/**
 * Gets the aircrafts required bay size.
 */
string Aircraft::GetBaySizeReq()
{
	return _baySizeReq;
}

/**
 * Sets numCalEvents
 */
void Aircraft::SetNumCalEvents(int numCalEvents)
{
	_numCalEvents = numCalEvents;
}

/**
 * Returns _myCalObj
 */
CalendarObj* Aircraft::GetCalendarObj()
{
	return _myCalObj;
}

/**
 * Logic to iterate to the appropriate next repair job based off of priority.
 */
RepairJob* Aircraft::GetNextRepairJob(string rjName)
{
	//receives current repair job name

	//get the object of that repair job
	RepairJob* nextJob = this->GetMyRepairJobObj(rjName);
	//variable for tracking priorities
	int highPriority = INT_MAX;
	//get its priority
	int myPriority = this->GetMyRepairJobObj(rjName)->GetStep(1)->GetRJPriority();

	map<string, RepairJob*>::const_iterator iter = _myRepairJobs.begin();
	while (iter != _myRepairJobs.end())
	{
		//if job is not my current job and is of the same type
		//if next repairjob has higher priority (lower number) than current high priority
		if (iter->second->GetName() != GetMyRepairJobObj(rjName)->GetName()
			&& iter->second->GetSchedType() == GetMyRepairJobObj(rjName)->GetSchedType()
			&& iter->second->GetPriority() <= highPriority)
		{
			highPriority = iter->second->GetPriority();
			//this one is the next job
			nextJob = iter->second;

			iter++;
		}

		else if (iter->second->GetSchedType() != GetMyRepairJobObj(rjName)->GetSchedType())
		{
			_myRepairJobs.erase(iter);
			iter = _myRepairJobs.begin();
		}
		else
		{
			iter++;
		}

	}


	return nextJob;//set this??


}

/**
 * Returns the next step to an aircrafts repair job.
 */
Step* Aircraft::GetNextStep()
{
	return _nextStep;
}

/**
 * Sets the source ID.
 */
void Aircraft::SetSource(int sourceID)
{
	_sourceID = sourceID;
}

/**
 * Returns the source ID.
 */
int Aircraft::GetSource()
{
	return _sourceID;
}

/**
 * Returns the aircraft ID.
 */
int Aircraft::GetAircraftID()
{
	return _aircraftID;
}

/**
 * Sets the aircraft ID.
 */
void Aircraft::SetAircraftID(int id)
{
	_aircraftID = id;
}

/**
 * Returns the next aircraft ID.
 */
int Aircraft::GetNextAircraftID()
{
	return _nextID;
}

/**
 * Clears the map of an aircraft.
 */
void Aircraft::ClearMyMap()
{
	_myRepairJobs.clear();
}

/**
 * Clears and starts a new repair job list.
 */
void Aircraft::UpdateList(const Aircraft& mapAircraft)
{
	ClearMyMap();
	_myRepairJobs = mapAircraft._myRepairJobs;
}

/**
 * Returns the new Aircraft
 */
Aircraft* Aircraft::New()
{
	//_aircraftID = ++_nextID; 
	Aircraft* newAircraft = new Aircraft(*this);
	InputReader::AddAirCount();
	return newAircraft;// add appropriate parameters
}

/**
 * Sets the aircraft footprint--length and wingspan.
 */
void Aircraft::SetAircraftFootprint(double length, double wingspan)
{
	_length = length;
	_wingspan = wingspan;
}

/**
 *Returns the length and wingspan of an aircraft.
 */
double Aircraft::GetAircraftFootprint()
{
	return _length, _wingspan;
}

/**
 * Sets the Aircraft Type.
 */
void Aircraft::SetAircraftType(string aircraftType)
{
	_aircraftType = aircraftType;
	//	cout << "Aircraft Type: " << aircraftType << endl; 
}

/**
 * Returns the aircraft type.
 */
string Aircraft::GetAircraftType()
{
	//cout << " in get aircraft type " << _aircraftType << endl; 
	return _aircraftType;
}

/**
 * Adds a repair job and its name to the allRepairJobsMap.
 */
void Aircraft::AddRepairJob(RepairJob* repairJob, string repairJobName)
{
	_allRepairJobsMap[repairJobName] = repairJob;
}

/**
 * Adds a repair job to an aircrafts myRepairJobs
 */
void Aircraft::AddMyRepairJob(string jobName, RepairJob* myJob)
{
	_myRepairJobs.insert(pair<string, RepairJob*>(jobName, myJob));
}

/**
 * Adds a repair job to an aircrafts myUnplannedJobsMap
 */
void Aircraft::AddMyUnplannedJob(string jobName, RepairJob* myJob)
{
	_myUnplannedJobsMap.insert(pair<string, RepairJob*>(jobName, myJob));
}

/**
 * Iterates through allRepairJobsMap to find the repair job's name and returns it.
 */
RepairJob* Aircraft::GetRepairJobObj(string name)
{

	map<string, RepairJob*>::iterator it = _allRepairJobsMap.find(name);
	if (it == _allRepairJobsMap.end())
		return nullptr;
	return it->second;
}

/**
 * Iterates throught an aircraft's myRepairJobs to find a repair job's name and returns it.
 */
map<string, RepairJob*> Aircraft::GetUnplanJobMap()
{
	return _myUnplannedJobsMap;
}

/**
 * Returns the repair job.
 */
RepairJob* Aircraft::GetMyRepairJobObj(string name)
{
	map<string, RepairJob*>::iterator it = _myRepairJobs.find(name);
	if (it == _myRepairJobs.end())
		return nullptr;

	return it->second;
}

/**
 * Sets an aircraft's priority.
 */
void Aircraft::SetAircraftPriority(int priority)
{
	_priority = priority;
}

/**
 * Returns an aircrafts priority
 */
int Aircraft::GetAircraftPriority()
{
	return _priority;
}

/**
 * Returns the beginning of _allRepairJobsMap.
 */
map<string, RepairJob*>::iterator Aircraft::GetRJMapBegin()
{
	return _allRepairJobsMap.begin();
}

/**
 * Returns the end of _allRepairJobsMap.
 */
map<string, RepairJob*>::iterator  Aircraft::GetRJMapEnd()
{
	return _allRepairJobsMap.end();
}

/**
 * Returns the beginning of an aircrafts repair jobs map.
 */
map<string, RepairJob*>::iterator Aircraft::GetMyRJMapBegin()
{
	return _myRepairJobs.begin();
}

/**
 * Returns the end of an aircrafts repair jobs map.
 */
map<string, RepairJob*>::iterator  Aircraft::GetMyRJMapEnd()
{
	return _myRepairJobs.end();
}

/**
 * Returns the beginning of an aircrafts unplanned repair jobs map.
 */
map<string, RepairJob*>::iterator Aircraft::GetMyUnplannedMapBegin()
{
	return _myUnplannedJobsMap.begin();
}

/**
 * Returns the end of an aircrafts unplanned repair jobs map.
 */
map<string, RepairJob*>::iterator  Aircraft::GetMyUnplannedMapEnd()
{
	return _myUnplannedJobsMap.end();
}

/**
 * Returns an aircrafts beginning Recurring IAT map.
 */
map<string, Distribution*>::iterator Aircraft::GetRecurMapBegin()
{
	return _myRecurIATmap.begin();
}

/**
 * Returns an aircrafts end Recurring IAT map.
 */
map<string, Distribution*>::iterator  Aircraft::GetRecurMapEnd()
{
	return _myRecurIATmap.end();
}


/**
 * Returns a reandom element for unplanned repair jobs.
 */
string Aircraft::GetRandomElement()
{

	random_device random_device;
	mt19937 engine{ random_device() };
	uniform_int_distribution<int> dist(0, GetUnplanVecSize() - 1);
	string random_element = _unplannedRjVec[dist(engine)];

	return random_element;
}

void Aircraft::AddBayReqToRes()
{
	map<string, RepairJob*>::const_iterator iter = _myRepairJobs.begin();
	while (iter != _myRepairJobs.end())
	{
		for (int i = 0; i < iter->second->GetStepVecSize(); i++)
		{
			map<string, StepResource*>::const_iterator resIter = InputReader::GetMasterResMapBegin();
			while (resIter != InputReader::GetMasterResMapEnd())
			{
				//cout << "ADDING BAY REQ TO RESOURCE REQ " << resIter->first << endl; 
				if (resIter->first == _baySizeReq)
				{
					resIter->second->SetNumResNeeded(1);
					iter->second->GetStep(i + 1)->AddResource(resIter->second, resIter->first, 1);

					//cout << "adding" << resIter->first << endl; 
				}
				resIter++;
			}
		}
		iter++;
	}

}

/**
 * Returns the size of the vector for unplanned repair jobs.
 */
int Aircraft::GetUnplanVecSize()
{
	return _unplannedRjVec.size();
}

/**
 * Returns the size of the map for all repair jobs.
 */
int Aircraft::GetAllRJMapSize()
{
	return _allRepairJobsMap.size();
}

/**
 * Returns the size of the map for an aircrafts repair jobs.
 */
int Aircraft::GetMyRJMapSize()
{
	return _myRepairJobs.size();
}

/**
 * Returns the size of the map for an aircrafts unplanned repair jobs.
 */
int Aircraft::GetMyUnplannedMapSize()
{
	return _myUnplannedJobsMap.size();
}

/**
 * Erases the repair job from an aircraft
 */
void Aircraft::DeleteJob(string repairJob)
{
	_myRepairJobs.erase(repairJob);
}

/**
 * Logic to set an aircraft's unplanned IATs based off of a distribution.
 */
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

/**
 * Returns the unplanned IAT of an aircraft.
 */
Distribution* Aircraft::GetAircraftIAT()
{
	return _iatUnplanned;//check if this works?
	//_iatUnplanned->PrintDistribution(); 
}

/**
 * Returns an aircrafts recurring IAT map.
 */
map<string, Distribution*> Aircraft::GetRecurIatMap()
{
	return _myRecurIATmap;
}

/**
 * Returns an aircrafts job map.
 */
map<string, RepairJob*> Aircraft::GetMyJobsMap()
{
	return _myRepairJobs;
}

/**
 * Returns the print properties of aircrafts.
 */
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

	map<string, RepairJob*>::iterator it = _allRepairJobsMap.begin();
	//cout << "After creating the iterator to the map " << std::endl;
	while (it != _allRepairJobsMap.end())
	{
		cout << "in the loop \n";

		it->second->PrintJobProperties();
		it++;
	}
}

/**
 * Returns print properties for an individual aircraft.
 */
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
		//cout << endl; 
		iter++;
	}


	cout << "MY JOB LIST SIZE IS " << _myRepairJobs.size() << endl;
	map<string, RepairJob*>::iterator it = _myRepairJobs.begin();
	//cout << "After creating the iterator to the map " << std::endl;
	while (it != _myRepairJobs.end())
	{
		//		it->second->PrintJobProperties(); 
		it++;
	}
}