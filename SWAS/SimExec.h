#pragma once
#include "SimObj.h"
#include "Distribution.h"
#include "CalendarConverter.h"
//#include "InputInterface.h"
#include <iostream>

/**
* Class for the input reader
*/
class InputReader;

using namespace std;

/**
* Calls the function RunSimulation()
*/
void PlayButton();
/**
* Calls the function RunSimulation(Time month, Time day, int year)
*/
void PlayButton(Time month, Time day, int year);
/**
* Calls the function FlipSimulationFlag()
*/
void StopSimulation();

/**
* Struct for the simulation time
*/
struct SimulationTime {
	Time _month;
	Time _day;
	Time _timeOfDay;
	int _year;
};

/**
* Class for the simulation executive which inherits from the SimObj class
*/
class SimExec : public SimObj {
public:
	/**
	* Returns the simulation time
	*/
	static SimulationTime GetSimulationTime();
	/**
	* Returns the input reader
	*/
	static InputReader GetInputReader();
	/**
	* Sets the input reader
	*/
	static void SetInputReader(InputReader inputReader);
	/**
	* Schedules an event
	*/
	static void ScheduleEventAt(int priority, EventAction* ea, double distributionValue, string eaName);
	/**
	* Schedules a calendar event
	*/
	static void ScheduleEventAtCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName);
	/**
	* Schedules a recurring event
	*/
	static void ScheduleEventAtRecurring(int priority, EventAction* ea, double distributionValue, string eaName, int recurring = 1);
	/**
	* Schedules a conditional event
	*/
	static void ScheduleConditionalEvent(int priority, CondEventAction* cea);
	/** 
	* Converts a month into a string
	*/
	static string ConvertDate(Time month);
	/**
	* Checks a conditional event based on resources and parts
	*/
	static void CheckConditionalEvents(Resource* resource = 0, Parts* parts = 0);
	/**
	* Prints an event set
	*/
	static void PrintEventSet();
	/**
	* Initializes the simulation
	*/
	static void InitializeSimulation(int numBins, int* days);
	//static void RunSimulation();
	/**
	* Runs the simulation by executing events in the event set
	*/
	static int RunSimulation();
	//static void RunSimulation(Time month, Time day, Time timeOfDay, int year);
	/**
	* Runs the simulation by executing events in the event set
	*/
	static int RunSimulation(Time month, Time day, int year);
	/**
	* Flips the simulation flag to true or false
	*/ 
	static void FlipSimulationFlag();
	/**
	* Returns the simulation flag
	*/
	static bool GetSimulationFlag();

private:
	/**
	* Struct for an event
	*/
	struct Event;
	/**
	* Struct for a conditional event
	*/
	struct CondEvent;
	static SimulationTime _simulationTime;
	/**
	* Class for an event set
	*/
	class EventSet;
	/**
	* Class for a conditional event set
	*/
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