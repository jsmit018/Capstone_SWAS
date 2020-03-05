#pragma once
#include "SimObj.h"
#include "Distribution.h"
#include "CalendarConverter.h"
#include <iostream>

using namespace std;


class SimExec : public SimObj {
public:
	//SimExec();
	static Time GetSimulationTime();
	static void ScheduleEventAt(int priority, EventAction* ea, double distributionValue);
	static void ScheduleEventAtCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea);
	static void InitializeSimulation(int numBins, Time timeRange, int* days);
	static void RunSimulation();
	static void RunSimulation(Time time);

private:
	struct Event;
	static Time _simulationTime;
	class EventSet;
	static EventSet _eventSet;
	enum months{January, February, March, April, May, June, July,
	      August, September, October, Novemember, December};


};
