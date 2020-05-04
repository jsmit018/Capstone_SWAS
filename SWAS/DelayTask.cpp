#include "DelayTask.h"
#include "SimExec.h"

/**
* Sets the delay time and delay name
*/
DelayBlock::DelayBlock(string name, Distribution* delayTime) : Task(name) {
    _delayTime = delayTime; 
    _name = name; 
}

/**
* Schedules a start delay event method and inherits from the EventAction class
*/
class DelayBlock::StartDelayEA : public EventAction {
public:
    StartDelayEA(DelayBlock* delayBlock, Aircraft* aircraft) {
        _delayBlock = delayBlock; 
        _aircraft = aircraft; 
    }

    void Execute() {
        _delayBlock->StartDelayEM(_aircraft); 
    }

private:
    DelayBlock* _delayBlock; 
    Aircraft* _aircraft; 
}; 

/**
* Schedules an end delay event method and inherits from the EventAction class
*/
class DelayBlock::EndDelayEA : public EventAction {
public:
    EndDelayEA(DelayBlock* delayBlock, Aircraft* aircraft) {
        _delayBlock = delayBlock; 
        _aircraft = aircraft; 
    }

    void Execute() {
        _delayBlock->EndDelayEM(_aircraft); 
    }

private:
    DelayBlock* _delayBlock; 
    Aircraft* _aircraft; 
}; 

/**
* Returns the name of the delay
*/
string DelayBlock::GetName() {
    return _name; 
}

/**
* Sets the name of the delay
*/
void DelayBlock::SetName(string name) {
    _name = name; 
}

/**
* Schedules a delay event at a specified time based on aircraft priority
*/
void DelayBlock::Execute(Aircraft* aircraft) {
    cout << "Scheduling Delay Event" << endl; 
    SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new StartDelayEA(this, aircraft), 0.0, "StartDelayEA"); 
}

/**
* Schedules a finish delay event at a specified time based on aircraft priority
*/
void DelayBlock::StartDelayEM(Aircraft* aircraft) {
    cout << "Scheduling a finish Delay Event" << endl; 
    SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new EndDelayEA(this, aircraft), _delayTime->GetRV(), "EndDelayEA"); 
}

/**
* Aircraft depart once delay is finished
*/
void DelayBlock::EndDelayEM(Aircraft* aircraft) {
    cout << "Delay is finished" << endl; 
    Depart(aircraft); 
}