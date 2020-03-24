#include "DelayTask.h"
#include "SimExec.h"

DelayBlock::DelayBlock(string name, Distribution* delayTime) : Task(name) {
      _delayTime = delayTime;
      _name = name;
}

class DelayBlock::StartDelayEA : public EventAction {
public:
    StartDelayEA(DelayBlock* delayBlock, Aircraft* aircraft){
          _delayBlock = delayBlock;
          _aircraft = aircraft;
    }
  
    void Execute(){
        _delayBlock->StartDelayEM(_aircraft);
    }
  
private:
    DelayBlock* _delayBlock;
    Aircraft* _aircraft;
};

class DelayBlock::EndDelayEA : public EventAction {
public:
    EndDelayEA(DelayBlock* delayBlock, Aircraft* aircraft){
          _delayBlock = delayBlock;
          _aircraft = aircraft;
    }
  
    void Execute(){
        _delayBlock->EndDelayEM(_aircraft);
    }
  
private:
    DelayBlock* _delayBlock;
    Aircraft* _aircraft;
};

string DelayBlock::GetName() {
    return _name;
}

void DelayBlock::SetName(string name) {
    _name = name;
}

void DelayBlock::Execute(Aircraft* aircraft) {
	cout << "Scheduling Delay Event" << endl;
	SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new StartDelayEA(this, aircraft), 0.0, "StartDelayEA");
}

void DelayBlock::StartDelayEM(Aircraft* aircraft){
	cout << "Scheduling a finish Delay Event" << endl;
	SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new EndDelayEA(this, aircraft), _delayTime->GetRV(), "EndDelayEA");
}

void DelayBlock::EndDelayEM(Aircraft* aircraft){
	cout << "Delay is finished" << endl;
    Depart(aircraft);
}