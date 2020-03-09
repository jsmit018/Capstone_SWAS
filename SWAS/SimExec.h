#pragma once
#include "SimObj.h"
#include "Distribution.h"
#include "CalendarConverter.h"
#include <iostream>

using namespace std;


class SimExec : public SimObj {
public:
	static Time GetSimulationTime();
	static void ScheduleEventAt(int priority, EventAction* ea, double distributionValue, string eaName);
	static void ScheduleEventAtCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName);
	static void ScheduleEventAtRecurring(int priority, EventAction* ea, double distributionValue, string eaName, int recurring = 1);
	static void PrintEventSet();
	static void InitializeSimulation(int numBins, int* days);
	static void RunSimulation();
	static void RunSimulation(Time time);

private:
	struct Event;
	static Time _simulationTime;
	class EventSet;
	static EventSet _eventSet;
	enum months {
		January, February, March, April, May, June, July,
		August, September, October, Novemember, December
	};


};
