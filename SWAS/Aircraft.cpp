//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"
int Aircraft::_nextID = 0;

Aircraft::Aircraft()
{
	_aircraftID = ++_nextID; // here instead of in setID funciton
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

void Aircraft::AddRepairJobMaster(RepairJob* repairJob, string repairJobName)
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
	if(it == _allRepairJobsMap.end())
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

void Aircraft::SetAircraftIAT(string iatUnplanned)
{
	istringstream iatUn(iatUnplanned);
	string firstHalf;
	string secHalf;
	
	getline(iatUn, firstHalf, '(');
	getline(iatUn, secHalf, ')');

	istringstream nums(secHalf);
	if( firstHalf ==  "Triangular")
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

//string Aircraft::GetAircraftIAT()
//{
//	//change to distribution
//}

void Aircraft::PrintProperties()
{
	cout << "type: " << _aircraftType << endl;
	//cout << "priority: " << _priority << endl;
	//cout << "length: " << _length << endl;
//	cout << "wingspan: " << _wingspan << endl;

	map<string, RepairJob*>::iterator it = _allRepairJobsMap.begin();
	//cout << "After creating the iterator to the map " << std::endl; 
	while (it != _allRepairJobsMap.end())
	{
		//cout << "in the loop \n";
		it->second->PrintJobProperties();
		it++;
	}
}
