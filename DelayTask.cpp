#include "DelayTask.h"

DelayBlock::DelayBlock() : Task() {

}

class DelayBlock::StartDelayEA : public EventAction {

};

class DelayBlock::EndDelayEA : public EventAction {

};

Time DelayBlock::GetDelayTime() {

}

void DelayBlock::SetDelayTime(Time delayTime) {

}

string DelayBlock::GetName() {

}

void DelayBlock::SetName(string name) {

}

void DelayBlock::Execute(Airplane* airplane) {

}