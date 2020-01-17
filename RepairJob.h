#pragma once
#include "SimObj.h"
#include "Task.h"

class RepairJob {
public:
	RepairJob(string name);
	string GetName();
	void SetName(string name);
	Task* GetNextStep();
	void AddStep(Task* step);
private:
	string _name;
	Task** _stepList;
	int _size;
};