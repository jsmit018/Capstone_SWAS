#include "Parts.h"

//send to Parts class
void Parts::SetPartsCount(int partsCount)
{
	_partsCount = partsCount;
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

bool Parts::AreEnoughParts(int numNeeded)
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