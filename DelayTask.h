#pragma once
#include "SimObj.h"
#include "Task.h"

class DelayBlock : public Task {
public:
	DelayBlock(Distribution* delayTime, string name);
	//Time GetDelayTime();
	//void SetDelayTime(Time delayTime);
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
