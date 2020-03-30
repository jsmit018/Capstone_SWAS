#pragma once
#include "SimObj.h"
#include "Distribution.h"
#include "CalendarConverter.h"
//#include "InputInterface.h"
#include <iostream>

class InputReader;

using namespace std;

void PlayButton();
void PlayButton(Time month, Time day, int year);
void StopSimulation();


struct SimulationTime {
	Time _month;
	Time _day;
	Time _timeOfDay;
	int _year;
};

class SimExec : public SimObj {
public:
	static SimulationTime GetSimulationTime();
	static InputReader GetInputReader();
	static void SetInputReader(InputReader inputReader);
	static void ScheduleEventAt(int priority, EventAction* ea, double distributionValue, string eaName);
	static void ScheduleEventAtCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName);
	static void ScheduleEventAtRecurring(int priority, EventAction* ea, double distributionValue, string eaName, int recurring = 1);
	static void ScheduleConditionalEvent(int priority, CondEventAction* cea);
	static string ConvertDate(Time month);
	static void CheckConditionalEvents(Resource* resource = 0, Parts* parts = 0);
	static void PrintEventSet();
	static void InitializeSimulation(int numBins, int* days);
	//static void RunSimulation();
	static int RunSimulation();
	//static void RunSimulation(Time month, Time day, Time timeOfDay, int year);
	static int RunSimulation(Time month, Time day, int year);
	static void FlipSimulationFlag();
	static bool GetSimulationFlag();

private:
	struct Event;
	struct CondEvent;
	static SimulationTime _simulationTime;
	class EventSet;
	class CondEventSet;
	static EventSet _eventSet;
	static CondEventSet _conditionalSet;
	static InputReader _inputReader;
	enum months {
		January, February, March, April, May, June, July,
		August, September, October, Novemember, December
	};

	static bool _simulationFlag;


};