#include "Resource.h"

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

////send to Parts class
//void Resource::OrderPartsEA()
//{
//
//}
//
////send to Parts class
//void Resource::OrderPartsEM()
//{
//
//}

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

void Resource::SetResourceCount(int initialCount)
{
	_resourceCount = initialCount;
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

void Resource::SetFailureName(string failureName)
{
	_failureName = failureName;
}

string Resource::GetFailureName()
{
	return _failureName;
}
