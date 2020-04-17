#include <iostream>
#include "SinkTask.h"
#include "Aircraft.h"

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
    cout << "Returning terminated aircraft" << endl;
    return _terminatedAircraft;
}

void SinkBlock::Execute(Aircraft* aircraft) {
    cout << "Aircraft, " << aircraft->GetAircraftType() << " number " << aircraft->GetAircraftID() << "has finished tour of maintenance, departing" << endl;
    _terminatedAircraft++;
    Scribe::TallyAircraft(aircraft->GetAircraftType());
    cout << " IN SINK EXECUTE - BEFORE DELETE " << aircraft->GetAircraftID() << endl;
    delete aircraft;
    //aircraft->DeleteAircraft(aircraft);
    aircraft = NULL;
   // cout << " IN SINK EXECUTE - AFTER DELETE " << aircraft->GetAircraftID() << endl;
}

