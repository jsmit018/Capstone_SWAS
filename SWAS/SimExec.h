#pragma once
#include "SimObj.h"
#include "Distribution.h"


class SimExec : public SimObj {
public:
	//SimExec();
	static Time GetSimulationTime();
	//void SetSimulationTime();
	//EventAction* GetEventAction();
	//void SetEventAction(EventAction* ea);
	//EventSet* GetEventSet();
	//void SetEventSet(EventSet* es);
	static void ScheduleEventIn(Time time, EventAction* ea);
	static void ScheduleEventAt(Time time, EventAction* ea);
	static void InitializeSimulation(int numBins, Time timeRange);
	static void RunSimulation();
	static void RunSimulation(Time time);

private:
	struct Event;
	static Time _simulationTime;
	class EventSet;
	static EventSet _eventSet;
	enum {January, February, March, April, May, June, July,
		  August, September, October, Novemember, December};
	
};
