#include "Resource.h"

Resource::Resource()
{
	//_failureName = "none specified yet";
	//_failureType = "none specified yet";
	//_repairProc = "none specified yet";
}

void Resource::CopyMapResource(const Resource& mapResource)
{
	_resourceCount = mapResource._resourceCount;
	_numNeeded = mapResource._numNeeded;
	_resourceName = mapResource._resourceName;
	_length = mapResource._length;
	_width = mapResource._width;
	_failureName = mapResource._failureName;
	_failureType = mapResource._failureType;
	_repairProc = mapResource._repairProc;
	if (mapResource._failureDist == nullptr)
		_failureDist = nullptr; 
	else
		_failureDist = mapResource._failureDist->CopyThis();
}

//@TODO will need to figure out logic for what happens if amount needed is greater
void Resource::Acquire(int amountNeeded)
{
	//if there aren't enough resources for this step
	if (amountNeeded > _resourceCount) {
		//continuously check when available and schedule now when its avail
	}
	else {
		//otherwise, subtract amount needed from resource count
		_resourceCount -= amountNeeded;
	}

	_resourceCount -= amountNeeded;
}

void Resource::Release(int amountToRelease)
{
	_resourceCount += amountToRelease;
}

bool Resource::IsAvailable(int amountNeeded)
{
	if (amountNeeded > _resourceCount)
		return false;
	else {
		return true;
	}
}

void Resource::FailResource()
{
	//@TODO write the algorithm for a resource failure essentially is just scheduling an event
	//so needed EA and EM
}

void Resource::RestoreResource()
{
	//May probably need an associated Event to Execute function call. 
	_resourceCount++;
}

///////////////////////////////
////  GETTERS AND SETTERS  ////
///////////////////////////////

void Resource::SetResourceCount(int resourceCount)
{
	_resourceCount = resourceCount;
}

int Resource::GetResourceCount()
{
	return _resourceCount;
}

void Resource::SetResourceName(string resourceName)
{
	_resourceName = resourceName;
}

string Resource::GetResourceName()
{
	return _resourceName;
}

void Resource::SetNumResNeeded(int numNeeded)
{
	//get from step table values 
	_numNeeded = numNeeded;
}

int Resource::GetNumResNeeded()
{
	return _numNeeded;
}

void Resource::SetResourceFootprint(double length, double width)
{
	_length = length;
	_width = width;
}

double Resource::GetResourceFootprint()
{
	return _length, _width;
}

void Resource::SetFailureName(string failureName)
{
	_failureName = failureName;
}

string Resource::GetFailureName()
{
	return _failureName;
}

void Resource::SetFailureType(string failureType)
{
	_failureType = failureType;
}

string Resource::GetFailureType()
{
	return _failureType;
}

void Resource::SetFailureDistr(string failureDistr)
{
	//turn failure distr from string into distributions
	istringstream failDist(failureDistr);
	string firstHalf;
	string secHalf;

	getline(failDist, firstHalf, '(');
	getline(failDist, secHalf, ')');
	//	cout << "first: " << firstHalf << endl;
	//	cout << "sec: " << secHalf << endl;

	istringstream nums(secHalf);
	if (firstHalf == "Triangular" || firstHalf == "Tri")
	{
		double min, expected, max;
		nums >> min;
		nums >> expected;
		nums >> max;
		_failureDist = new Triangular(min, expected, max);
	}

	else if (firstHalf == "Exponential")
	{
		double mean;
		nums >> mean;

		_failureDist = new Exponential(mean);
	}

	else if (firstHalf == "Uniform")
	{
		double min, max;
		nums >> min >> max;

		_failureDist = new Uniform(min, max);
	}

	else if (firstHalf == "Normal")
	{
		double mean, stdev;
		nums >> mean >> stdev;

		_failureDist = new Normal(mean, stdev);
	}

	else if (firstHalf == "Poisson")
	{
		double mean;
		nums >> mean;

		_failureDist = new Poisson(mean);
	}

	else if (firstHalf == "Constant" || firstHalf == "Fixed")
	{
		double mean;
		nums >> mean;

		_failureDist = new Constant(mean);
	}

	else if (firstHalf == "Weibull")
	{
		double scale, shape;
		nums >> scale >> shape;

		_failureDist = new Weibull(scale, shape);
	}

	//Determines correct distribution and prints
	//_failureDist->PrintDistribution();

}

Distribution* Resource::GetFailureDistr()
{
	return _failureDist; //check if this works
}

void Resource::SetRepairProcess(string repairProc)
{
	_repairProc = repairProc;
}

string Resource::GetRepairProcess()
{
	return _repairProc;
}

void Resource::PrintResProperties()
{
	cout << "			Resource name: " << _resourceName << endl;
	cout << "			Initial count: " << _resourceCount << endl;
	cout << "			Resource footprint X: " << _length << endl;
	cout << "			Resource footprint Y: " << _width << endl;
	cout << "			Failure name: " << _failureName << endl;
	cout << "			Failure type: " << _failureType << endl;
	cout << "			Failure distr: ";
	if (_failureDist == nullptr)
	{
		cout << " no failure distr" << endl;
	}
	else
		_failureDist->PrintDistribution();
	cout << endl;
	cout << "			Repair process: " << _repairProc << endl;
	cout << endl;
}