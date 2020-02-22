//Aircraft.h: Andrea Robey
//This class is responsible for providing aircraft information to source.cpp when it's called to create new aircraft. 
//It holds a linked list of repair jobs and steps
//It's responsible for getting/setting type, footprint, id, source, & nextid of every aircraft

#pragma once
#include "SimObj.h"
#include "RepairJob.h"
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

//struct Footprint {
//	int x;
//	int y;
//};

class Aircraft {
public:
	Aircraft();
	Aircraft(string aircraftName, string aircraftID, int aircraftPriority, double aircraftFootPrintX,
		double aircraftFootPrintY, string repairJobName, double iat_unplanned);

	//int GetAircraftID();
	void SetAircraftID(int aircraftID);
	//int GetSource();
	//void SetSource(int source);
	//Footprint GetAircraftFootprint();
	//void SetAircraftFootprint(Footprint dimensions);
	//string GetAircraftType();
	//void SetAircraftType(string aircraftType);
	//int GetAircraftPriority();
	//void SetAircraftPriority(int priority);
	//int GetNextAircraftID(); 
	//void SetNextID(int id);

private:
	string	_aircraftName;							//	Aircraft name: i.e. F35
	string	_aircraftID;							//	Aircraft id: unique to individual aircraft
	int		_aircraftPriority;						//	Aircraft priority: helps determine first to be served in queues
	double	_aircraftFootPrintX;					//	Aircraft size x dimension  ***make a tuple or struct later
	double	_aircraftFootPrintY;					//	Aircraft size y dimension
	string	_repairJobName;							//	Repair job name
	double	_iat_unplanned;							//	Distribution for random aircraft interarrival times (iat) 
///include schedule date
///include recurring amount
	vector<string> myRepairJobs;					//	Vector of repair job names unique to each new aircraft to be compared to master list

	//if object encapsulation method is chosen:
//	unordered_map<string, RepairJob> _repairJobMap;	//	Map of "my" repair jobs (***make Repair job a pointer later to save memory)


	//int _aircraftID;
	//int _source;
	//Footprint _dimensions;
	//string _aircraftType;
	//int _priority;
	//static int _nextID;
};