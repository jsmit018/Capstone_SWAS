#pragma once
//#include "SimObj.h"
//#include "Task.h"
#include "Distribution.h"
#include <string>
#include <sstream>

class Resource
{
public:
	Resource();
	void CopyMapResource(const Resource& mapResource);
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
	void SetNumResNeeded(int numNeeded);
	void SetFailureName(string failureName);
	void SetFailureType(string failureType);
	void SetFailureDistr(string failureDistr);
	void SetRepairProcess(string repairProc);

	void ScheduleFirstFailures(Resource* resource);

	int GetResourceCount();
	string GetResourceName();
	double GetResourceFootprint();
	int GetNumResNeeded();
	string GetFailureName();
	string GetFailureType();
	Distribution* GetFailureDistr();
	string GetRepairProcess();

private:
	int _resourceCount;
	int _numNeeded;
	string _resourceName;
	double _length;
	double _width;
	string _failureName;
	string _failureType;
	Distribution* _failureDist;
	string _repairProc;

	class RestoreResourceEA;
	class FailResourceEA;
	class WaitForResourceEA;

	void FailResourceEM(Resource* resource);
	void RestoreResourceEM(Resource* resource);
};