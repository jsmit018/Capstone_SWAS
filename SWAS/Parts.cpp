#include "Parts.h"

Parts::Parts()
{
	_partsName = "none needed";
	_partsCount = 0;
	_orderThreshold = 0;
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
	istringstream leadDist(leadTime);
	string firstHalf;
	string secHalf;

	getline(leadDist, firstHalf, '(');
	getline(leadDist, secHalf, ')');
//	cout << "first: " << firstHalf << endl;
//	cout << "sec: " << secHalf << endl;

	istringstream nums(secHalf);
	if (firstHalf == "Triangular")
	{
		double min, expected, max;
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

	//Determines correct distribution and prints
	_leadTime->PrintDistribution();
}

Distribution* Parts::GetLeadTime()
{
	return _leadTime;
}

void Parts::PrintPartsProperties()
{
	cout << "			Parts name: " << _partsName << endl;
	cout << "			Initial count: " << _partsCount << endl;
	cout << "			Order threshold: " << _orderThreshold << endl;
	cout << endl;
}