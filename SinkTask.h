#pragma once
#include "Task.h"
#include "SimObj.h"

class SinkBlock : public Task {
public:
	SinkBlock();
	void Execute();
	string GetName();
	void SetName(string name);
	int GetTerminatedAircraft();
	//void SetTerminatedAircraft(int terminatedAircraft);
private:
	string _name;
	int _terminatedAircraft;
};