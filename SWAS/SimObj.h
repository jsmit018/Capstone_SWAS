#pragma once

#include <iostream>

using namespace std;

typedef double Time;

class EventAction {
public:
	EventAction() {};
	virtual void Execute() = 0;
};

class SimObj {
public:
//SimulationExecutive getSimulationExecutive()
//void setSimulationExecutive(SimulationExecutive simExec)

protected:
	SimObj();
	virtual void ScheduleEventIn(Time time, EventAction* ea) = 0;
	virtual void ScheduleEventAt(Time time, EventAction* ea) = 0;
	virtual Time GetSimulationTime() = 0;

private:

};
