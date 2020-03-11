#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Distribution.h"
#include <string>
#include <sstream>

//Make separate class for Parts...they have nothing in common

class Resource
{
public:
	Resource();
	void Acquire(int amountNeeded);
	//virtual void Acquire() = 0;
	void Release(int amountToRelease);
	//virtual void Release = 0;
	bool IsAvailable(int amountNeeded);
	void FailResource();
	void RestoreResource();
	void PrintResProperties();

	void SetResourceCount(int resourceCount);
	void SetResourceName(string resourceName);
	void SetResourceFootprint(double length, double width);
	void SetNumResNeeded(int numResNeeded);
	void SetFailureName(string failureName);
	void SetFailureType(string failureType);
	void SetFailureDistr(string failureDistr);
	void SetRepairProcess(string repairProc);

	int GetResourceCount();
	string GetResourceName();
	double GetResourceFootprint();
	int GetNumResNeeded();
	string GetFailureName();
	string GetFailureType();
	Distribution* GetFailureDistr();	//change to distribution
	string GetRepairProcess();

private:
	int _resourceCount;
	int _numResNeeded;
	string _resourceName;
	double _length;
	double _width;
	string _failureName;
	string _failureType;
	Distribution* _failureDist;
	string _repairProc;
//	string _failureDistTemp; // remove and replace with distrib
};