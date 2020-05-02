#pragma once
//#include "SimObj.h"
//#include "Task.h"
#include "Distribution.h"
#include <string>
#include <sstream>

class StepResource
{
public:
	StepResource();
	void CopyMapResource(const StepResource& mapResource);
	void Acquire(int amountNeeded);
	//virtual void Acquire() = 0;
	void Release(int amountToRelease);
	//virtual void Release = 0;
	bool IsAvailable(int amountNeeded);
	void FailResource();
	void RestoreResource();
	void PrintResProperties();
	void SetResourceID(int resourceID);
	int GetResourceID();


	void SetResourceCount(double resourceCount);
	void SetResourceName(string resourceName);
	void SetResourceFootprint(double length, double width);
	void SetNumResNeeded(int numNeeded);
	void SetFailureName(string failureName);
	void SetFailureType(string failureType);
	void SetFailureDistr(string failureDistr);
	void SetRepairProcess(string repairProc);

	void ScheduleFirstFailures(StepResource* resource);

	double GetResourceCount();
	string GetResourceName();
	double GetResourceFootprint();
	int GetNumResNeeded();
	string GetFailureName();
	string GetFailureType();
	Distribution* GetFailureDistr();
	string GetRepairProcess();
	bool IsAfterCEL();
	void SetCELflag(int CELflag);
	

private:
	//int _resourceCount;
	int _resourceID; // NOT SET YET
	double _resourceCount;
	int _numNeeded;
	int _CELflag;
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

	Distribution* failureDistribution;

	void FailResourceEM(StepResource* resource);
	void RestoreResourceEM(StepResource* resource);
};