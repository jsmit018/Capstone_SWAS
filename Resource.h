#pragma once
//#include "SimObj.h"
//#include "Task.h"
#include "Distribution.h"
#include <string>
#include <sstream>

//Make separate class for Parts...they have nothing in common

/**
* Class for resources
*/
class Resource
{
public:
	/**
	* Constructor for resources
	*/
	Resource();
	/**
	* Sets resource values to map resource values
	*/
	void CopyMapResource(const Resource& mapResource);
	/**
	* Acquires resources
	*/
	void Acquire(int amountNeeded);
	//virtual void Acquire() = 0;
	/**
	* Releases resources and adds to the resource count
	*/
	void Release(int amountToRelease);
	//virtual void Release = 0;
	/**
	* Checks if the amount of resources needed is greater than the resource count
	*/
	bool IsAvailable(int amountNeeded);
	/**
	* Schedules a resource failure event
	*/
	void FailResource();
	/**
	* Increases the resource count
	*/
	void RestoreResource();
	/**
	* Prints the resource properties
	*/
	void PrintResProperties();

	/**
	* Sets the resource count
	*/
	void SetResourceCount(int resourceCount);
	/**
	* Sets the resource name
	*/
	void SetResourceName(string resourceName);
	/**
	* Sets the resource length and width
	*/
	void SetResourceFootprint(double length, double width);
	/**
	* Sets the number of resources needed
	*/
	void SetNumResNeeded(int numNeeded);
	/**
	* Sets the resource failure name
	*/
	void SetFailureName(string failureName);
	/**
	* Sets the resource failure type
	*/
	void SetFailureType(string failureType);
	/**
	* Turns the failure distribution from a string into the correct distribution
	*/
	void SetFailureDistr(string failureDistr);
	/**
	* Sets the repair process
	*/
	void SetRepairProcess(string repairProc);

	/**
	* Returns the resource count
	*/
	int GetResourceCount();
	/** 
	* Returns the resource name
	*/
	string GetResourceName();
	/**
	* Returns the resource length and width
	*/
	double GetResourceFootprint();
	/**
	* Returns the number of resources needed
	*/
	int GetNumResNeeded();
	/**
	* Returns the failure name
	*/
	string GetFailureName();
	/**
	* Returns the failure type
	*/
	string GetFailureType();
	/**
	* Returns the failure distribution
	*/
	Distribution* GetFailureDistr();	//change to distribution
	/**
	* Returns the repair process
	*/
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
};