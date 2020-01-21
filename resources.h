#pragma once
#include "SimObj.h"
#include "task.h"


class Resource
{
public:
	void Acquire();
	//virtual void Acquire() = 0;
	void Release();
	//virtual void Release = 0;
	bool isAvailable();
	void failResource();
	void restoreResource();
private:
	int _resourceCount;
	string _resourceName;
	string _failureName;
	Distribution* _failureDist;
};
