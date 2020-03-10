#pragma once
#include "SimObj.h"
#include "Distribution.h"
#include "CalendarConverter.h"
#include <iostream>

using namespace std;

struct SimulationTime {
	Time _month;
	Time _day;
	Time _timeOfDay;
	int _year;
};

class SimExec : public SimObj {
public:
	static SimulationTime GetSimulationTime();
	static void ScheduleEventAt(int priority, EventAction* ea, double distributionValue, string eaName);
	static void ScheduleEventAtCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName);
	static void ScheduleEventAtRecurring(int priority, EventAction* ea, double distributionValue, string eaName, int recurring = 1);
	static void ScheduleConditionalEvent(int priority, CondEventAction* cea);
	static void CheckConditionalEvents(Resource* resource);
	static void PrintEventSet();
	static void InitializeSimulation(int numBins, int* days);
	static void RunSimulation();
	static void RunSimulation(Time month, Time day, Time timeOfDay, int year);

private:
	struct Event;
	struct CondEvent;
	static SimulationTime _simulationTime;
	class EventSet;
	class CondEventSet;
	static EventSet _eventSet;
	static CondEventSet _conditionalSet;
	enum months {
		January, February, March, April, May, June, July,
		August, September, October, Novemember, December
	};


};
