#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Distribution.h"
#include <string>

class Parts
{
public:
	void AcquireParts(int numNeeded);
	void SetPartsCount(int partsCount);
	void SetPartsName(string partsName);
	void SetThreshold(int orderThreshold);
	bool AreEnoughParts(int numNeeded);

	int GetPartsCount();
	string GetPartsName();
	int GetThreshold();

private:
	int _partsCount;
	int _numNeeded;
	string _partsName;
	int _orderThreshold;
	Distribution* _partsLeadTime;
};