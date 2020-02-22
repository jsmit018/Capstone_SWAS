//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"

Aircraft::Aircraft()
{
}

Aircraft::Aircraft(string aircraftName, string aircraftID, int aircraftPriority, double aircraftFootPrintX,
	double aircraftFootPrintY, string repairJobName, double iat_unplanned)
{
	_aircraftName = aircraftName;
	_aircraftID = aircraftID;
	_aircraftPriority = aircraftPriority;
	_aircraftFootPrintX = aircraftFootPrintX;
	_aircraftFootPrintY = aircraftFootPrintY;
	_repairJobName = repairJobName;
	_iat_unplanned = iat_unplanned;
}
//
//int Aircraft::GetAircraftID() {
//
//}
//
void Aircraft::SetAircraftID(int aircraftID) {
	_aircraftID = aircraftID;
}
//
//int Aircraft::GetSource() {
//
//}
//
//void Aircraft::SetSource(int source) {
//
//}
//
//Footprint Aircraft::GetAircraftFootprint() {
//
//}
//
//void Aircraft::SetAircraftFootprint(Footprint dimensions) {
//
//}
//
//string Aircraft::GetAircraftType() {
//
//}
//
//void Aircraft::SetAircraftType(string aircraftType) {
//
//}
//
//int Aircraft::GetAircraftPriority() {
//
//}
//
//void Aircraft::SetAircraftPriority(int priority) {
//
//}
//
//int Aircraft::GetNextAircraftID() {
//
//}