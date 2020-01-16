#pragma once
#include "SimObj.h"
#include "Task.h"

class DelayBlock : public Task {
public:
	DelayBlock();
	Time GetDelayTime();
	void SetDelayTime(Time delayTime);
	string GetName();
	void SetName(string name);
	void Execute(Airplane* airplane);
	

private:
	Time delayTime;
	string _name;
	class StartDelayEA;
	class EndDelayEA;
	void StartDelayEM(Airplane* airplane);
	void EndDelayEM(Airplane* airplane);
};