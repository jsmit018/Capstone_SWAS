#pragma once
#include "SimObj.h"
#include "task.h"


class Resource
{
public:
	void virtualAcquire();
	void virtualRelease();
	bool isAvailable();
	void failResource();
	void restoreResource();
private:
	int resourceCount;
	string resourceName;
	string failureName;
	double failureDist;
};