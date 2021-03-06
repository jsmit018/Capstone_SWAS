//Parts.cpp: Andrea Robey
#include "Parts.h"

Parts::Parts()
{
}

void Parts::CopyMapParts(const Parts& mapParts)
{
	_initCount = mapParts._initCount;
	_partsCount = mapParts._partsCount;
	_numNeeded = mapParts._numNeeded;
	_partsName = mapParts._partsName;
	_orderThreshold = mapParts._orderThreshold;

	if (mapParts._leadTime == nullptr)
		_leadTime = nullptr;
	else
	{
		_leadTime = mapParts._leadTime->CopyThis();
	}
}


void Parts::SetPartsCount(int partsCount)
{
	_partsCount = partsCount;
}

void Parts::SetNumPartsNeeded(int numNeeded)
{
	_numNeeded = numNeeded;
}

int Parts::GetNumPartsNeeded()
{
	return _numNeeded;
}

void Parts::AcquireParts(int numNeeded)
{
	//if there aren't enough resources for this step
	if (numNeeded > _partsCount) {
		//continuously check when available and schedule now when its avail
	}
	else {
		//otherwise, subtract amount needed from parts count
		_partsCount -= numNeeded;
	}

	_partsCount -= numNeeded;

	if (_partsCount < _orderThreshold) {
		//Schedule an ordering event... may need to have an EA/EM with this class for that
	}
}

bool Parts::AreEnoughParts()
{
	if (_numNeeded > _partsCount)
		return false;
	else {
		return true;
	}
}



//send to Parts class
int Parts::GetPartsCount()
{
	return _partsCount;
}

void Parts::SetInitPartsCount(int partsCount)
{
	_initCount = partsCount;
}

int Parts::GetInitPartsCount()
{
	return _initCount;
}

//send to Parts class
void Parts::SetPartsName(string partsName)
{
	_partsName = partsName;
}

//send to Parts class
string Parts::GetPartsName()
{
	return _partsName;
}

void Parts::SetThreshold(int orderThreshold)
{
	_orderThreshold = orderThreshold;
}

int Parts::GetThreshold()
{
	return _orderThreshold;
}

void Parts::SetLeadTime(string leadTime)
{
	//the string being passed in is now split into two strings
	istringstream leadDist(leadTime);
	string firstHalf;
	string secHalf;

	//they're split based on the ( and ) symbols. The parenthesis are treated as a delimiter.
	getline(leadDist, firstHalf, '(');
	getline(leadDist, secHalf, ')');
	//	cout << "first: " << firstHalf << endl; 
	//	cout << "sec: " << secHalf << endl; 

		//this is used for the second half to turn the numbers into the doubles
	istringstream nums(secHalf);

	//if statements for determining which distribution it is
	if (firstHalf == "Triangular")
	{
		double min, expected, max;
		//the first part of the string segment called "nums" is set a the double min. second one is double expected, etc. 
		nums >> min;
		nums >> expected;
		nums >> max;
		_leadTime = new Triangular(min, expected, max);
	}

	else if (firstHalf == "Exponential")
	{
		double mean;
		nums >> mean;

		_leadTime = new Exponential(mean);
	}

	else if (firstHalf == "Uniform")
	{
		double min, max;
		nums >> min >> max;

		_leadTime = new Uniform(min, max);
	}

	else if (firstHalf == "Normal")
	{
		double mean, stdev;
		nums >> mean >> stdev;

		_leadTime = new Normal(mean, stdev);
	}

	else if (firstHalf == "Poisson")
	{
		double mean;
		nums >> mean;

		_leadTime = new Poisson(mean);
	}

	else if (firstHalf == "Constant" || firstHalf == "Fixed")
	{
		double mean;
		nums >> mean;

		_leadTime = new Constant(mean);
	}

	else if (firstHalf == "Weibull")
	{
		double scale, shape;
		nums >> scale >> shape;

		_leadTime = new Weibull(scale, shape);
	}


	//Calls the print function added to each distribution - Determines correct distribution and prints
//	cout << "LEAD TIME "; 
//	_leadTime->PrintDistribution(); 
}

Distribution* Parts::GetLeadTime()
{
	return _leadTime;
}

void Parts::PrintPartsProperties()
{
	cout << "			Parts name: " << _partsName << endl;
	cout << "			Parts count: " << _partsCount << endl;
	cout << "			Nun needed:	" << _numNeeded << endl;
	cout << "			Initial count: " << _initCount << endl;
	cout << "			Order threshold: " << _orderThreshold << endl;
	cout << "			Lead Time: ";
	if (_leadTime != nullptr)
		_leadTime->PrintDistribution();
	cout << endl;
}