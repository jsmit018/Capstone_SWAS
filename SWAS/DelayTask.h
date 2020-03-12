#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Aircraft.h"
#include "Distribution.h"
#include <iostream>

class DelayBlock : public Task {
public:
	DelayBlock(string name, Distribution* delayTime);
	string GetName();
	void SetName(string name);
	void Execute(Aircraft* aircraft);
	

private:
	Distribution* _delayTime;
	string _name;
	class StartDelayEA;
	class EndDelayEA;
	void StartDelayEM(Aircraft* aircraft);
	void EndDelayEM(Aircraft* aircraft);
};
