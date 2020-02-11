#pragma once
#include "Task.h"
#include "SimObj.h"

class SinkBlock : public Task {
public:
	SinkBlock(string name);
	void Execute(Aircraft* aircraft);
	string GetName();
	void SetName(string name);
	int GetTerminatedAircraft();
	//void SetTerminatedAircraft(int terminatedAircraft);
private:
	string _name;
	int _terminatedAircraft;
};
