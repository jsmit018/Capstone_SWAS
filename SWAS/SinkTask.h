#pragma once
#include "Task.h"
//#include "SimObj.h"
#include "SimExec.h"
#include "Scribe.h"

class SinkBlock : public Task {
public:
	SinkBlock(string name);
	void Execute(Aircraft* aircraft);
	string GetName();
	void SetName(string name);
	int GetTerminatedAircraft();
private:
	string _name;
	int _terminatedAircraft;
	//Scribe* outputRecorder = new Scribe();
};
