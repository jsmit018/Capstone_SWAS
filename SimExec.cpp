#include "SimExec.h"

SimExec::SimExec() : SimObj(){

}

void SimExec::InitializeSimulation() {
   _simulationTime = 0;
   //_eventSet = 0;
}

Time SimExec::GetSimulationTime() {
    return _simulationTime;
}

//This is handled by the advancing of the Simulation.. not needed.
/*void SimExec::SetSimulationTime() {

}*/

EventAction* SimExec::GetEventAction() {

}

//We will never set an event action this is done through scheduling.
/*void SetEventAction(EventAction* ea) {

}*/

EventSet* SimExec::GetEventSet() {
    return _eventSet;
}

void SimExec::ScheduleEventAt(Time time, EventAction* ea) {

}

void SimExec::ScheduleEventIn(Time time, EventAction* ea) {

}

void SimExec::RunSimulation() {

}

void SimExec::RunSimulation(Time time) {

}

