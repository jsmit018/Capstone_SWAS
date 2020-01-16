#pragma once

typedef double Time;

class EventAction {
};

class SimObj {
public:
//SimulationExecutive getSimulationExecutive()
//void setSimulationExecutive(SimulationExecutive simExec)

protected:
	SimObj();
	void ScheduleEventIn(Time time, EventAction* ea);
	void ScheduleEventAt(Time time, EventAction* ea);
	Time GetSimulationTime();

private:

};