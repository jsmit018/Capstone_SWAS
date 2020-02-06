#include <iostream>
#include "SinkTask.h"

SinkBlock::SinkBlock(string name) : Task(name) {
    _name = name;
}

string SinkBlock::GetName() {
    return _name;
}

void SinkBlock::SetName(string name) {
    _name = name;
}

int SinkBlock::GetTerminatedAircraft() {
    return _terminatedAircraft;
}

void SinkBlock::Execute(Aircraft* aircraft) {
	terminatedAircraft++; 
   delete aircraft;
}

