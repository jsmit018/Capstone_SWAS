#include "Entity.h"

int Aircraft::_nextID = 0;

Aircraft::Aircraft(string aircraftType, int priority, Footprint dimensions) {
    _aircraftType = aircraftType;
    _priority = priority;
    _id = _nextID++;
    _dimensions.x = dimensions.x;
    _dimensions.y = dimensions.y;
}

int Aircraft::GetEntityID() {
    return _entityID;
}

void Aircraft::SetEntityID(int  entityID) {
    _entityID = entityID;
}

int Aircraft::GetSource() {
     return _source;
}

void Aircraft::SetSource(int source) {
    _source = source;
}

Footprint Aircraft::GetFootprint() {
     return _dimensions;
}

void Aircraft::SetFootprint(Footprint dimensions) {
    _dimensions = dimensions;
}

string Aircraft::GetAircraftType() {
    return _aircraftType;
}

//Going to leave this commented for now as this should be taken care of in the constructor
/*void Airplane::SetAircraftType(string aircraftType) {

}*/

int Aircraft::GetPriority() {
  return _priority;
}

//Going to leave this commented for now as this should be taken care of in the constructor
/*void Airplane::SetPriority(int priority) {

}*/

//Going to leave this for now but essentially this will update very time a new airplane is made
int Aircraft::GetNextID() {
    return _nextID;
}

Aircraft* Aircraft::New(string aircraftType, int priority, Footprint dimensions){
    return new Aircraft(aircraftType, priority, dimensions);
}

//Going to leave this as a commented function for now.
//@TODO make a function that creates a new aircraft of whatever type.
//@TODO make a decision on whether or not we are instantiating different specefic aircraft inherting.
