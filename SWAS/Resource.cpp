#include "Resource.h"

Resource::Resource()
{

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

void Resource::SetNumResNeeded(int numResNeeded)
{
	//get from step table values 
}

int Resource::GetNumResNeeded()
{
	return _numResNeeded;
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
	_failureDistTemp = failureDistr;

	//turn failure distr from string into distributions
}

string Resource::GetFailureDistr()
{
	return _failureDistTemp;
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
	cout << "res name: " << _resourceName << endl;
	cout << "count: " << _resourceCount << endl;
	cout << "length: " << _length << endl;
	cout << "width: " << _width << endl;
	cout << "failure name: " << _failureName << endl;
	cout << "failure type: " << _failureType << endl;
	cout << "failure dist: " << _failureDistTemp << endl;
	cout << "repair proc: " << _repairProc << endl;
	cout << endl;
}