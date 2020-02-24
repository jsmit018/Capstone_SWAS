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
//	Aircraft(string aircraftName, string aircraftID, int aircraftPriority, double aircraftFootPrintX,
//	double aircraftFootPrintY, string repairJobName, double iat_unplanned);
	void PrintProperties(/*string aircraftType, int priority, double length, double wingspan*/);
	//int GetAircraftID();
	void SetAircraftID(int aircraftID);
	void SetAircraftFootprint(double length, double wingspan);
	void SetAircraftIAT(string iatUnplanned);
	//int GetSource();
	//void SetSource(int source);
	//Footprint GetAircraftFootprint();
//	void SetAircraftFootprint(Footprint dimensions);
	//string GetAircraftType();
	void SetAircraftType(string aircraftType);
	//int GetAircraftPriority();
	void SetAircraftPriority(int priority);
	//int GetNextAircraftID(); 
	//void SetNextID(int id);

private:
	string	_aircraftType;								//	Aircraft name: i.e. F35
	string	_aircraftID;								//	Aircraft id: unique to individual aircraft
	int		_priority;									//	Aircraft priority: helps determine first to be served in queues
	double	_length;						//	Aircraft size x dimension  ***make a tuple or struct later
	double	_wingspan;						//	Aircraft size y dimension
	string	_repairJobName;								//	Repair job name
	string	_iatUnplanned;							//	Distribution for random aircraft interarrival times (iat) 
	//string	_schedType;								//	Schedule used for if statement: calendar or recurring
	//string	_schedDate;								//	Calendar date of maintenance event to be scheduled
	//int		_schedRecurAmt;							//	Value indicating number of months between recurring maintenance
	vector<string> mySchedRepairJobs;					//	Vector of sched repair job names unique to each new aircraft to be compared to master list
	vector<string> myRandRepairJobs;					//	Vector of rand repair job names unique to each new aircraft to be compared to master list
	unordered_map<string, RepairJob> _allRepairJobsMap;	//	Map of all possible repair jobs (***make Repair job a pointer later to save memory)


	//int _aircraftID;
	//int _source;
	//Footprint _dimensions;
	//string _aircraftType;
	//int _priority;
	//static int _nextID;
};