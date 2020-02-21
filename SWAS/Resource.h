#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Distribution.h"
#include <string>


class Resource
{
public:
	void Acquire(int amountNeeded);
	//virtual void Acquire() = 0;
	void Release(int amountToRelease);
	//virtual void Release = 0;
	bool isAvailable(int amountNeeded);
	void failResource();
	void restoreResource();
private:
	int _resourceCount;
	string _resourceName;
	string _failureName;
	int _resourceThreshold;
	Distribution* _failureDist;
};