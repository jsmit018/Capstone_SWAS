#include "DelayTask.h"

DelayBlock::DelayBlock(Distribution* delayTime, string name) : Task(name) {
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
    Aicraft* _aircraft;
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
    Aicraft* _aircraft;
};

//Because delayTime is a distribution we will not need getters and setters as this will be populated by
//the appropriate distributions
/*Time DelayBlock::GetDelayTime() {

}

void DelayBlock::SetDelayTime(Time delayTime) {

}*/

string DelayBlock::GetName() {
    return _name;
}

void DelayBlock::SetName(string name) {
    _name = name;
}

void DelayBlock::Execute(Aircraft* aircraft) {
    SimulationExecutive::ScheduleEventIn(0.0, new StartDelayEA(this, aircraft));
}

void DelayBlock::StartDelayEM(Aircraft* aircraft){
    SimulationExecutive::ScheduleEventIn(_delayTime->GetRV(), new EndDelayEA(this, aircraft));
}

void DelayBlock::EndDelayEM(Aircraft* aircraft){
    Depart(aircraft);
}
