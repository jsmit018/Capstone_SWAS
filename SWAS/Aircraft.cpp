//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"
#include "InputInterface.h"

int Aircraft::_nextID = 0;

Aircraft::Aircraft()
{
	_aircraftID = ++_nextID;
}
//** Andrea:: I see that you called this function in main, however this functionality of 
//random, recurring, and calendar is handled by aircraft arrivals in the source object. I see that this function is trying 
//to schedule physical steps and would essentially undo a lot of the stuff that has already been coded. 
//What I mean is when an aircraft that arrives from a random arrival would have a RepairJob
//with steps that were populated from your WillSchedule() function which when the aircraft moves to the step object the logic
//there will work through the repair job list. Same with Recurring and Calendar. When the Source Object creates an aircraft from that
//EM it will have a RepairJob based off of its arrival that step will take care of. I'm sorry to say but this functionality right here
//just will not work with what we have already coded and the flow of our Object-Oriented Program. We stated in the proposal that the 
//aircraft wouldn't do anything except flow through the system, but here the aircraft is calling upon steps to schedule events.
void Aircraft::GetMyJobList(string aircraftType)
{
	_aircraftType = aircraftType;
	
	//iterate through all repair jobs in _allrepairJobs map

	map<string, RepairJob*>::const_iterator iter = InputReader::GetAircraft(_aircraftType)->_allRepairJobsMap.begin();
	while (iter != InputReader::GetAircraft(_aircraftType)->_allRepairJobsMap.end())
	{
		//for all repair jobs with schedule type "random"
		if (iter->second->GetSchedType() == "Random") {

			//roll dice on unplanned probability
			if (iter->second->WillSchedule() == true)
			{
				cout << _aircraftType << "will have a Random Repair Job called: " << iter->second->GetName() << endl;

				iter->second->GetFirstStep()->ScheduleFirstStep(iter->second->GetFirstStep(), this);
			}

			else
				cout << _aircraftType << "will not have a Random Repair Job called: " << iter->second->GetName() << endl;
		}

		//for all repair jobs with schedule type "recurring"
		if (iter->second->GetSchedType() == "Recurring") {

			cout << _aircraftType << "has a Recurring Repair Job called: " << iter->second->GetName() << endl;
			
			iter->second->GetFirstStep()->ScheduleFirstStep(iter->second->GetFirstStep(), this);
		}

		//for all repair jobs with schedule type "calendar"
		if (iter->second->GetSchedType() == "Calendar") {

			cout << _aircraftType << "has a Calendar Repair Job called: " << iter->second->GetName() << endl;
			
			iter->second->GetFirstStep()->ScheduleFirstStep(iter->second->GetFirstStep(), this);
		}
	}
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
