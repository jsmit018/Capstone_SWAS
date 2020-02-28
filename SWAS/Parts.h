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
	void SetThreshold(int orderThreshold); 	//send to Parts class
	bool AreEnoughParts(int numNeeded);

	int GetPartsCount();	//send to Parts class
	string GetPartsName();	//send to Parts class
	int GetThreshold();		//send to Parts class

private:
	int _partsCount;
	int _numNeeded;
	string _partsName;
	int _orderThreshold;//send to Parts class
	Distribution* _partsLeadTime;//send to Parts class
};