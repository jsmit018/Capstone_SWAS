//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"
#include "InputInterface.h"

int Aircraft::_nextID = 0;

Aircraft::Aircraft()
{
	_aircraftID = ++_nextID;

}
void Aircraft::GetMyJobList(string aircraftType)
{
	_aircraftType = aircraftType;

	cout << "BEFORE current repair map size" << InputReader::GetAircraft(aircraftType)->GetAllRJMapSize() << endl;

	//iterate through all repair jobs in _allrepairJobs map in mastermap
	cout << "getting my jobs" << endl;
	map<string, RepairJob*>::iterator iter = InputReader::GetAircraft(_aircraftType)->GetRJMapBegin();

	//while (iter != InputReader::GetAircraft(_aircraftType)->GetRJMapEnd())
	while(InputReader::GetAircraft(_aircraftType)->IsMapEnd(iter))
	{ 
		cout << "in while loop" << endl;
		/*
		//for all repair jobs in allrepairjobs in mastermap with schedule type "random"
		if (iter->second->GetSchedType() == "Random" || iter->second->GetSchedType() == "Unplanned") {

			cout << "got random job" << endl;

			//roll dice on unplanned probability
			if (iter->second->WillSchedule() == true)
			{
				cout << _aircraftType << " will have a Random Repair Job called: " << iter->second->GetName() << endl;

				cout << "current map size" << InputReader::GetMapSize() << endl;
				//cout << "current vec size" << iter->second->GetStepVecSize() << endl;

				//make a copy of this repair job
				RepairJob* currJob = new RepairJob(*iter->second);

				cout << "current repair map size" << _allRepairJobsMap.size() << endl;

				//give this copy to this new aircraft's myrepairjobs list
				_myRepairJobs.insert(pair<string, RepairJob*>(iter->second->GetName(),currJob));

				cout << "NEW my map size" << GetMyRJMapSize() << endl;

				map<string, RepairJob*>::const_iterator myJobIter = _myRepairJobs.begin();
				while (myJobIter != _myRepairJobs.end())
				{
					//	delete myJobIter->second;

					//	schedule first step of this repair job
					cout << "VEC SIZE" << myJobIter->second->GetStepVecSize() << endl; 
					cout << endl;
					//Step* firstStep = myJobIter->second->GetStep(1);
					//myJobIter->second->GetStep(1)->ScheduleFirstStep(firstStep, this);
					myJobIter++;

				}
				cout << "AFTER current repair map size" << InputReader::GetAircraft(aircraftType)->GetAllRJMapSize() << endl;
				//	cout << "NEW DELETE my map size" << GetMyRJMapSize() << endl;
				continue;
			}

			else
				cout << _aircraftType << "will not have a Random Repair Job called: " << iter->second->GetName() << endl;
		}

		//for all repair jobs with schedule type "recurring"
		else if (iter->second->GetSchedType() == "Recurring") {

			cout << _aircraftType << "has a Recurring Repair Job called: " << iter->second->GetName() << endl;
			
			//schedule first step of repair job
			iter->second->GetFirstStep()->ScheduleFirstStep(iter->second->GetFirstStep(), this);
		}

		//for all repair jobs with schedule type "calendar"
		else if (iter->second->GetSchedType() == "Calendar") {

			cout << _aircraftType << "has a Calendar Repair Job called: " << iter->second->GetName() << endl;
			
			//schedule first step of repair job
			iter->second->GetFirstStep()->ScheduleFirstStep(iter->second->GetFirstStep(), this);
		} */
		iter++;
	}
}

bool Aircraft::IsMapEnd(map<string, RepairJob*>::const_iterator iter)
{
	if (iter != _allRepairJobsMap.end())
		return true;
	return false; 
}
void Aircraft::SetSource(int sourceID)
{
	_sourceID = sourceID;
} 

int Aircraft::GetSource()
{
	return _sourceID;
}

//void Aircraft::SetAircraftID(int aircraftID)
//{
//
//}

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

map<string,RepairJob*>::iterator Aircraft::GetRJMapBegin()
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
	if (firstHalf == "Triangular")
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
