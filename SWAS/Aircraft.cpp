//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"
int Aircraft::_nextID = 0;

Aircraft::Aircraft()
{
	_aircraftID = ++_nextID;
}

//void Aircraft::SetSource(int source) {
//
//}

//int Aircraft::GetSource() {
//
//}

//int Aircraft::GetAircraftID() {
//
//}

//int Aircraft::GetNextAircraftID() {
//
//}

void Aircraft::SetAircraftFootprint(double length, double wingspan)
{
	_length = length;
	_wingspan = wingspan;
}

//void GetAircraftFootprint()
//{
//
//}

void Aircraft::SetAircraftType(string aircraftType)
{
	_aircraftType = aircraftType;
	//	cout << "Aircraft Type: " << aircraftType << endl;
}

//string Aircraft::GetAircraftType() {
//
//}

void Aircraft::SetAircraftPriority(int priority) 
{
	_priority = priority;
//	cout << "priority: " << priority << endl;
}

//int Aircraft::GetAircraftPriority() {
//
//}

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

void Aircraft::PrintProperties(/*string aircraftType, int priority, double length, double wingspan*/)
{
	cout << "type: " << _aircraftType << endl;
	cout << "priority: " << _priority << endl;
	cout << "length: " << _length << endl;
	cout << "wingspan: " << _wingspan << endl;
}
