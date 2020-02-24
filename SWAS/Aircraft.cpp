//Aircraft.cpp: Andrea Robey

#include "Aircraft.h"

Aircraft::Aircraft()
{
}

//Aircraft::Aircraft(string aircraftType, string aircraftID, int aircraftPriority, double aircraftFootPrintX,
//	double aircraftFootPrintY, string repairJobName, double iat_unplanned)
//{
//	_aircraftType = aircraftType;
//	_aircraftID = aircraftID;
//	_aircraftPriority = aircraftPriority;
//	_aircraftFootPrintX = aircraftFootPrintX;
//	_aircraftFootPrintY = aircraftFootPrintY;
//	_repairJobName = repairJobName;
//	_iat_unplanned = iat_unplanned;
//}
//
//int Aircraft::GetAircraftID() {
//
//}
//
//void Aircraft::SetAircraftID(int aircraftID) {
//	_aircraftID = aircraftID;
//}
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
//	_aircraftFootPrintX - dimensions.x;
//	_aircraftFootPrintY - dimensions.y;
//}

void Aircraft::SetAircraftFootprint(double length, double wingspan)
{
	_length = length;
	_wingspan = wingspan;
}


//string Aircraft::GetAircraftType() {
//
//}
//
void Aircraft::SetAircraftType(string aircraftType) 
{
	_aircraftType = aircraftType;
//	cout << "Aircraft Type: " << aircraftType << endl;
}

//int Aircraft::GetAircraftPriority() {
//
//}
//
void Aircraft::SetAircraftPriority(int priority) 
{
	_priority = priority;
//	cout << "priority: " << priority << endl;
}

//int Aircraft::GetNextAircraftID() {
//
//}

void Aircraft::SetAircraftIAT(string iatUnplanned)
{
	_iatUnplanned = iatUnplanned;
//	cout << "IAT: " << iatUnplanned << endl;
}

void Aircraft::PrintProperties(/*string aircraftType, int priority, double length, double wingspan*/)
{
	cout << "type: " << _aircraftType << endl;
	cout << "priority: " << _priority << endl;
	cout << "length: " << _length << endl;
	cout << "wingspan: " << _wingspan << endl;
	cout << "iat: " << _iatUnplanned << endl;
}
