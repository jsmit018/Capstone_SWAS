#pragma once
#include "SimObj.h"
#include "RepairJob.h"

struct Footprint {
	int x;
	int y;
};

class Aircraft {
public:
	Aircraft();
	int GetEntityID();
	void SetEntityID(int entityID);
	int GetSource();
	void SetSource(int source);
	Footprint GetFootprint();
	void SetFootprint(Footprint dimensions);
	string GetAircraftType();
	void SetAircraftType(string aircraftType);
	int GetPriority();
	void SetPriority(int priority);
	int GetNextID();
	//void SetNextID(int id);
	Aircraft* New();
private:
	int _entityID;
	int _source;
	RepairJob** _repairList;
	Footprint _dimensions;
	string _aircraftType;
	int _priority;
	static int _nextID;
	
};
