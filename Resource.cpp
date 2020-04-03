#include "Resource.h"

/**
* Sets the failure name, failure type, and repair process to "none specified yet"
*/
Resource::Resource()
{
	_failureName = "none specified yet";
	_failureType = "none specified yet";
	_repairProc = "none specified yet";
}

/**
* Sets the resource count, number of resources needed, resource name, resource length, resource width, failure name, failure type, failure distribution, and repair process to default values
*/
void Resource::CopyMapResource(const Resource& mapResource)
{
	_resourceCount = mapResource._resourceCount;
	_numNeeded = mapResource._numNeeded;
	_resourceName = mapResource._resourceName;
	_length = mapResource._length;
	_width = mapResource._width;
	_failureName = mapResource._failureName;
	_failureType = mapResource._failureType;
	_failureDist = mapResource._failureDist;
	_repairProc = mapResource._repairProc;
}

/**
* Acquires resources by subtracting the amount of resources needed from the resource count
*/
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

/**
* Adds the amount of resources to release to the resource count; increases the resource count by the amount of resources to release
*/
void Resource::Release(int amountToRelease)
{
	_resourceCount += amountToRelease;
}

/**
* Checks if the amount of resources needed is greater than the resource count; if so, returns false; else, returns true
*/
bool Resource::IsAvailable(int amountNeeded)
{
	if (amountNeeded > _resourceCount)
		return false;
	else {
		return true;
	}
}

/**
* Schedules a resource failure event
*/
void Resource::FailResource()
{
	//@TODO write the algorithm for a resource failure essentially is just scheduling an event
	//so needed EA and EM
}

/**
* Increases the resource count
*/
void Resource::RestoreResource()
{
	//May probably need an associated Event to Execute function call. 
	_resourceCount++;
}

///////////////////////////////
////  GETTERS AND SETTERS  ////
///////////////////////////////

/**
* Sets the resource count
*/
void Resource::SetResourceCount(int resourceCount)
{
	_resourceCount = resourceCount;
}

/**
* Returns the resource count
*/
int Resource::GetResourceCount()
{
	return _resourceCount;
}

/**
* Sets the resource name
*/
void Resource::SetResourceName(string resourceName)
{
	_resourceName = resourceName;
}

/**
* Returns the resource name
*/
string Resource::GetResourceName()
{
	return _resourceName;
}

/**
* Sets the number of resources needed
*/
void Resource::SetNumResNeeded(int numNeeded)
{
	//get from step table values 
	_numNeeded = numNeeded;
}

/**
* Returns the number of resources needed
*/
int Resource::GetNumResNeeded()
{
	return _numNeeded;
}

/**
* Sets the resource length and width
*/
void Resource::SetResourceFootprint(double length, double width)
{
	_length = length;
	_width = width;
}

/**
* Returns the resource length and width
*/
double Resource::GetResourceFootprint()
{
	return _length, _width;
}

/**
* Sets the resource failure name
*/
void Resource::SetFailureName(string failureName)
{
	_failureName = failureName;
}

/**
* Returns the resource failure name
*/
string Resource::GetFailureName()
{
	return _failureName;
}

/**
* Sets the resource failure type
*/
void Resource::SetFailureType(string failureType)
{
	_failureType = failureType;
}

/**
* Returns the resource failure type
*/
string Resource::GetFailureType()
{
	return _failureType;
}

/**
* Turns the failure distribution from a string into the correct distribution, either triangular, exponential, uniform, normal, poisson, constant, or Weibull; prints the distribution
*/
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
	_failureDist->PrintDistribution();

}

/**
* Returns the resource failure distribution
*/
Distribution* Resource::GetFailureDistr()
{
	return _failureDist; //check if this works
}

/**
* Sets the repair process
*/
void Resource::SetRepairProcess(string repairProc)
{
	_repairProc = repairProc;
}

/**
* Returns the repair process
*/
string Resource::GetRepairProcess()
{
	return _repairProc;
}

/**
* Prints the resource name, initial count, resource footprint X, resource footprint Y, failure name, failure type, and repair process
*/
void Resource::PrintResProperties()
{
	cout << "			Resource name: " << _resourceName << endl;
	cout << "			Initial count: " << _resourceCount << endl;
	cout << "			Resource footprint X: " << _length << endl;
	cout << "			Resource footprint Y: " << _width << endl;
	cout << "			Failure name: " << _failureName << endl;
	cout << "			Failure type: " << _failureType << endl;
	cout << "			Repair process: " << _repairProc << endl;
	cout << endl;
}