//SinkTask.cpp: Provided by Dr. James Leathrum, modified by Jordan Smith

#include <iostream>
#include "SinkTask.h"
#include "Aircraft.h"

SinkBlock::SinkBlock(string name) : Task(name) {
    _name = name;
	_terminatedAircraft = 0;
}

string SinkBlock::GetName() {
    return _name;
}

void SinkBlock::SetName(string name) {
    _name = name;
}

int SinkBlock::GetTerminatedAircraft() {
    cout << "Returning terminated aircraft" << endl;
    return _terminatedAircraft;
}

void SinkBlock::Execute(Aircraft* aircraft) {
    _terminatedAircraft++;
    Scribe::TallyAircraft(aircraft->GetAircraftType());
    delete aircraft;
    aircraft = NULL;
}

