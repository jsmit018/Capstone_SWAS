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
	//SimExec::ScheduleEventIn(0.0, new StartDelayEA(this, aircraft));
}

void DelayBlock::StartDelayEM(Aircraft* aircraft){
   // SimExec::ScheduleEventIn(_delayTime->GetRV(), new EndDelayEA(this, aircraft));
}

void DelayBlock::EndDelayEM(Aircraft* aircraft){
    Depart(aircraft);
}
