#include <iostream>
#include "SinkTask.h"
#include "Aircraft.h"

/**
 * Constructor for sink
 */
SinkBlock::SinkBlock(string name) : Task(name) {
    _name = name;
	_terminatedAircraft = 0;
}

/**
 * Returns the sink name
 */
string SinkBlock::GetName() {
    return _name;
}

/**
 * Sets the sink name
 */
void SinkBlock::SetName(string name) {
    _name = name;
}

/**
 * Returns the terminated aircraft
 */
int SinkBlock::GetTerminatedAircraft() {
    cout << "Returning terminated aircraft" << endl;
    return _terminatedAircraft;
}

/**
 * Prints the aircraft type and ID, increments the number of terminated aircraft, and deletes the aircraft
 */
void SinkBlock::Execute(Aircraft* aircraft) {
    //cout << "Aircraft, " << aircraft->GetAircraftType() << " number " << aircraft->GetAircraftID() << "has finished tour of maintenance, departing" << endl;
    _terminatedAircraft++;
	//cout << "Number of Aircraft that have left so far: " << _terminatedAircraft << endl;
    Scribe::TallyAircraft(aircraft->GetAircraftType());
    delete aircraft;
    //aircraft->DeleteAircraft(aircraft);
    aircraft = NULL;
}

