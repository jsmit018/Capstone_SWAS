#pragma once
#include "SimObj.h"
#include "Distribution.h"
#include <iostream>

using namespace std;


class SimExec : public SimObj {
public:
	//SimExec();
	static Time GetSimulationTime();
	static void ScheduleEventAt(Time timeMonth, Time timeDay, Time timeOfDay, int year, int priority, EventAction* ea,
				   double distributionValue);
	static void InitializeSimulation(int numBins, Time timeRange, int* days);
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
