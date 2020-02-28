#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Distribution.h"
#include <string>

//Make separate class for Parts...they have nothing in common

class Resource
{
public:
	void Acquire(int amountNeeded);
	//virtual void Acquire() = 0;
	void Release(int amountToRelease);
	//virtual void Release = 0;
	bool IsAvailable(int amountNeeded);
	void FailResource();
	void RestoreResource();
	//void OrderPartsEA(); //send to Parts class
	//void OrderPartsEM(); //send to Parts class make class refer to source for examples

	void SetResourceCount(int resourceCount);
	void SetResourceName(string resourceName);
	void SetNumResNeeded(int numResNeeded);
	void SetFailureName(string failureName);

	int GetResourceCount();
	string GetResourceName();
	int GetNumResNeeded();
	string GetFailureName();

private:
	int _resourceCount;
	int _numResNeeded;
	string _resourceName;
	string _failureName;
	Distribution* _failureDist;

	int _partsCount;
	string _partsName;
};