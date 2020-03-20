//Aircraft.h: Andrea Robey
//This class is responsible for providing aircraft information to source.cpp when it's called to create new aircraft. 
//It holds a linked list of repair jobs and steps
//It's responsible for getting/setting type, footprint, id, source, & nextid of every aircraft

#pragma once
#include "SimObj.h"
#include "RepairJob.h"
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

class RepairJob;

class Aircraft {
public:
	Aircraft();
	void GetMyJobList(string aircraftType);
	void PrintProperties();

	void SetSource(int sourceID);
//	void SetAircraftID(int aircraftID); //may not need because handling it in constructor
	void SetAircraftFootprint(double length, double wingspan);
	void SetAircraftIAT(string iatUnplanned);
	void SetAircraftType(string aircraftType);
	void SetAircraftPriority(int priority);
	void AddRepairJob(RepairJob* repairJob, string repairJobName); //map of aircraft's repair jobs
	void AddRandRepairJob();	//populated when new craft is created - look at master, find random jobs, roll dice, add here
	void AddSchedRepairJob();
	//void SetNextID(int id);

	RepairJob* GetRepairJobObj(string name); //used for searching 
	int GetSource();
	int GetAircraftID();
	double GetAircraftFootprint();
	Distribution* GetAircraftIAT(); //switch to distribution pointer
	string GetAircraftType();
	int GetAircraftPriority();
	int GetNextAircraftID();
	int GetAllRJMapSize();
	int GetMyRJMapSize();
	map<string, RepairJob*>::iterator  GetRJMapBegin();
	map<string, RepairJob*>::iterator  GetRJMapEnd();
	Aircraft* New(); //new aircraft creation, need to add appropriate parameters
	bool IsMapEnd(map<string, RepairJob*>::const_iterator iter);
private:
	string	_aircraftType;								//	Aircraft name: i.e. F35
	int		_aircraftID;								//	Aircraft id: unique to individual aircraft
	static int _nextID;
	int _sourceID;
	int		_priority;									//	Aircraft priority: helps determine first to be served in queues
	double	_length;									//	Aircraft size x dimension  ***make a tuple or struct later
	double	_wingspan;									//	Aircraft size y dimension
	string	_repairJobName;								//	Repair job name
	Distribution* _iatUnplanned;						//	Distribution for random aircraft interarrival times (iat) 
	map<string, RepairJob*> _myRepairJobs;
	vector<string> mySchedRepairJobs;					//	Vector of sched repair job names unique to each new aircraft to be compared to master list
	vector<string> myRandRepairJobs;					//	Vector of rand repair job names unique to each new aircraft to be compared to master list
	map<string, RepairJob*> _allRepairJobsMap;			//	Map of all possible repair jobs
};