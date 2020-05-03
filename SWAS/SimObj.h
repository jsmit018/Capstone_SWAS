#pragma once

#include <iostream>

using namespace std ; 

typedef double Time ; 

class StepResource ; 
class Parts ; 

class EventAction {
public:
	EventAction() {} ; 
	virtual void Execute() = 0 ; 
} ; 

class CondEventAction : public EventAction {
public:
	CondEventAction() {} ; 
	virtual bool Condition(StepResource* resource = 0, Parts* parts = 0) = 0 ; 
} ; 

class SimObj {
protected:
	SimObj() ; 
	virtual void ScheduleEventIn(Time time, EventAction* ea) = 0 ; 
	virtual void ScheduleEventAt(Time time, EventAction* ea) = 0 ; 
	virtual Time GetSimulationTime() = 0 ; 
} ; 