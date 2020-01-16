#pragma once
#include "SimObj.h"


class SimExec : public SimObj {
public:
	SimExec();
	Time GetSimulationTime();
	void SetSimulationTime();
	EventAction* GetEventAction();
	void SetEventAction(EventAction* ea);
	EventSet* GetEventSet();
	//void SetEventSet(EventSet* es);
	void ScheduleEventIn(Time time, EventAction* ea);
	void ScheduleEventAt(Time time, EventAction* ea);
	void InitializeSimulation();
	void RunSimulation();
	void RunSimulation(Time time);

private:
	Time _simulationTime;
	EventSet _eventSet;
};