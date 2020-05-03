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
    //cout << "Aircraft, " << aircraft->GetAircraftType() << " number " << aircraft->GetAircraftID() << "has finished tour of maintenance, departing" << endl; 
    _terminatedAircraft++; 
	//cout << "Number of Aircraft that have left so far: " << _terminatedAircraft << endl; 
    Scribe::TallyAircraft(aircraft->GetAircraftType()); 
    delete aircraft; 
    //aircraft->DeleteAircraft(aircraft); 
    aircraft = NULL; 
}

