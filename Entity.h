#pragma once
#include "SimObj.h"

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
	virtual Aircraft* New() = 0;
private:
	int _entityID;
	int _source;
	Footprint _dimensions;
	string _aircraftType;
	int _priority;
	static int _nextID;
};
