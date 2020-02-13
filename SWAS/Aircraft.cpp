#include "Aircraft.h"

int Aircraft::_nextID = 0;

Aircraft::Aircraft(string aircraftType, int aircraftID, int nextID, int aircraftPriority, 
    string repairJobName, double iat_unplanned, Footprint dimensions)
{
    _aircraftType = aircraftType;
    _aircraftID = _nextID++;
    _aircraftPriority = aircraftPriority;
    _repairJobName = repairJobName;
    _iat_unplanned = iat_unplanned;
    _dimensions.x = dimensions.x;
    _dimensions.y = dimensions.y;
}

int Aircraft::GetAircraftID() {
    return _aircraftID;
}

void Aircraft::SetAircraftID(int aircraftID) {
    _aircraftID = aircraftID;
}

int Aircraft::GetSource() {
    return _source;
}

void Aircraft::SetSource(int source) {
    _source = source;
}

Footprint Aircraft::GetAircraftFootprint() {
    return _dimensions;
}

void Aircraft::SetAircraftFootprint(Footprint dimensions) {
    _dimensions = dimensions;
}

string Aircraft::GetAircraftType() {
    return _aircraftType;
}

//Going to leave this commented for now as this should be taken care of in the constructor
/*void Airplane::SetAircraftType(string aircraftType) {

}*/

int Aircraft::GetAircraftPriority() {
    return _aircraftPriority;
}

//Going to leave this commented for now as this should be taken care of in the constructor
/*void Airplane::SetPriority(int priority) {

}*/

//Going to leave this for now but essentially this will update very time a new airplane is made
int Aircraft::GetNextAircraftID() {
    return _nextID;
}

//Aircraft* Aircraft::New(string aircraftType, int aircraftPriority, Footprint dimensions) {
////    return new Aircraft(aircraftType, aircraftPriority, dimensions);
//}

//Going to leave this as a commented function for now.
//@TODO make a function that creates a new aircraft of whatever type.
//@TODO make a decision on whether or not we are instantiating different specefic aircraft inherting.
