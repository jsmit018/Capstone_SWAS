#pragma once
#include "SimObj.h"
#include "RepairJob.h"
#include <vector>

struct Footprint {
	int x;
	int y;
};

class Aircraft {
public:
	Aircraft(string aircraftType, int aircraftID, int nextID, int aircraftPriority, string repairJobName, double iat_unplanned, Footprint dimensions);
	int GetAircraftID();
	void SetAircraftID(int aircraftID);
	int GetSource();
	void SetSource(int source);
	Footprint GetAircraftFootprint();
	void SetAircraftFootprint(Footprint dimensions);
	string GetAircraftType();
	void SetAircraftType(string aircraftType);
	int GetAircraftPriority();
	void SetAircraftPriority(int aircraftPriority);
	int GetNextAircraftID(); 
	//Aircraft* New(string aircraftType, int aircraftPriority, Footprint dimensions);
private:
	int			_source;					//	Aircraft's source
	Footprint	_dimensions;				//	Aircraft dimensions?
	string		_aircraftType;				//	Aircraft name: i.e. F35
	int			_aircraftID;				//	Aircraft id: unique to individual aircraft
	int			_aircraftPriority;			//	Aircraft priority: helps determine first to be served in queues
	string		_repairJobName;				//	Repair job name
	double		_iat_unplanned;				//	Distribution for random aircraft interarrival times (iat) 
	static int	_nextID;					//	Next aircraft ID
	vector<string> myRepairJobs;			//	Vector of repair job names unique to each new aircraft to be compared to master list


};
