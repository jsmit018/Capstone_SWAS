#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Distribution.h"
#include <string>
#include <sstream>

class Parts
{
public:
	Parts();
	void AcquireParts(int numNeeded);
	bool AreEnoughParts(int numNeeded);
	void PrintPartsProperties();

	void SetPartsCount(int partsCount);
	void SetPartsName(string partsName);
	void SetThreshold(int orderThreshold);
	void SetLeadTime(string leadTime);

	int GetPartsCount();
	string GetPartsName();
	int GetThreshold();

private:
	int _partsCount;
	int _numNeeded;
	string _partsName;
	int _orderThreshold;
//	string _leadTimeTemp;
	Distribution* _leadTime;
};