#pragma once

#include <iostream>

using namespace std; 

typedef double Time; 

/**
 * Class for resources
 */
class StepResource; 

/**
 * Class for parts
 */
class Parts; 

/**
 * Base class for event actions
 */
class EventAction {
public:
	EventAction() {}; 
	virtual void Execute() = 0; 
}; 

/**
 * Class for conditional event actions which inherits from the EventAction class
 */
class CondEventAction : public EventAction {
public:
	CondEventAction() {}; 
	virtual bool Condition(StepResource* resource = 0, Parts* parts = 0) = 0; 
}; 

/**
 * Class for the simulation object
 */
class SimObj {
protected:
    /**
     * Default constructor for the simulation object
     */
	SimObj(); 
    /**
     * Schedules an event at the current simulation time plus time
     */
	virtual void ScheduleEventIn(Time time, EventAction* ea) = 0; 
    /**
     * Schedules an event at a specified time
     */
	virtual void ScheduleEventAt(Time time, EventAction* ea) = 0; 
    /**
     * Returns the simulation time
     */
	virtual Time GetSimulationTime() = 0; 
}; 