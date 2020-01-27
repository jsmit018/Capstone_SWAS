#pragma once
#include "SimObj.h"
#include "Task.h"

class RepairJob {
public:
	//RepairJob(string name);
	RepairJob(string name, int listSize);
	string GetName();
	void SetName(string name);
	RepairJob* GetNextStep();
	void AddStep(RepairJob* step, int listSpot);
private:
	string _name;
	int _activeStep;
	//Task** _stepList;
	RepairJob** _stepList;
	int _size;
};
