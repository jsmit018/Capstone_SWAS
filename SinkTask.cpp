#include "SinkTask.h"

SinkBlock::SinkBlock() : Task(name) {
    _name = name;
}

string SinkBlock::GetName() {
    return _name;
}

void SinkBlock::SetName(string name) {
    _name = name;
}

int SinkBlock::GetTerminatedAircraft() {
    _return _terminatedAircraft;
}

void SinkBlock::Execute(Aircraft* aircraft){
   _terminatedAircraft++; 
}
