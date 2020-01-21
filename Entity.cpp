#include "Entity.h"

int Airplane::_nextID = 0;

Airplane::Airplane(string aircraftType, int priority, Footprint dimensions) {
    _aircraftType = aircraftType;
    _priority = priority;
    _id = _nextID++;
    _dimensions.x = dimensions.x;
    _dimensions.y = dimensions.y;
}

int Airplane::GetEntityID() {
    return _entityID;
}

void Airplane::SetEntityID(int  entityID) {
    _entityID = entityID;
}

int Airplane::GetSource() {
     return _source;
}

void Airplane::SetSource(int source) {
    _source = source;
}

Footprint Airplane::GetFootprint() {
     return _dimensions;
}

void Airplane::SetFootprint(Footprint dimensions) {
    _dimensions = dimensions;
}

string Airplane::GetAircraftType() {
    return _aircraftType;
}

//Going to leave this commented for now as this should be taken care of in the constructor
/*void Airplane::SetAircraftType(string aircraftType) {

}*/

int Airplane::GetPriority() {
  return _priority;
}

//Going to leave this commented for now as this should be taken care of in the constructor
/*void Airplane::SetPriority(int priority) {

}*/

//Going to leave this for now but essentially this will update very time a new airplane is made
int Airplane::GetNextID() {
    return _nextID;
}

//Going to leave this as a commented function for now.
//@TODO make a function that creates a new aircraft of whatever type.
//@TODO make a decision on whether or not we are instantiating different specefic aircraft inherting.
