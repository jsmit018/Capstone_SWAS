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
#include "InputInterface.h"
#include "CalendarConverter.h"

using namespace std;

class RepairJob;

class Aircraft {
public:
	Aircraft();
	Aircraft(const Aircraft& mapAircraft);
	//void CopyAircraftInfo(const Aircraft& mapAircraft);
	void CopyMyJobList(string aircraftType);
	void PrintProperties();
	void PrintMyProperties();

	void SetSource(int sourceID);
	//	void SetAircraftID(int aircraftID); //may not need because handling it in constructor
	void SetAircraftFootprint(double length, double wingspan);
	void SetAircraftIAT(string iatUnplanned);
	void SetAircraftType(string aircraftType);
	void SetAircraftPriority(int priority);
	void AddRepairJob(RepairJob* repairJob, string repairJobName); //map of aircraft's repair jobs
	void AddRecurIAT(string repairJobName, Distribution* iatRecurring);
	void AddMyRepairJob(string jobName, RepairJob* myJob);
	void AddMyUnplannedJob(string jobName, RepairJob* myJob);
	void InsertJobName(string jobName);
	void SetCalendarObj(string date);
	void SetNumCalEvents(int numCalEvents);
	void SetAircraftID(int id);
	void SetBaySizeReq(string baySizeReq);
	void DeleteJob(string repairJob);
	void ClearMyMap();
	void DeleteAircraft(Aircraft*& aircraft);
	//void SetRecurFlag(char flag);

	RepairJob* GetRepairJobObj(string name); //used for searching 
	RepairJob* GetMyRepairJobObj(string name); //used for searching 
	int GetSource();
	int GetAircraftID();
	double GetAircraftFootprint();
	Distribution* GetAircraftIAT(); //switch to distribution pointer
	string GetAircraftType();
	int GetAircraftPriority();
	int GetNextAircraftID();
	int GetAllRJMapSize();
	int GetMyRJMapSize();
	int GetMyUnplannedMapSize();
	int GetUnplanVecSize();
	string GetRandomElement();
	string GetBaySizeReq();
	//char GetRecurFlag();
	map<string, RepairJob*> GetUnplanJobMap();
	map<string, Distribution*> GetRecurIatMap();
	map<string, RepairJob*> GetMyJobsMap();
	RepairJob* GetNextRepairJob(string rjName);
	//vector<Distribution*> GetRecurringIATs();
	Step* GetNextStep();
	CalendarObj* GetCalendarObj();

	map<string, RepairJob*>::iterator  GetRJMapBegin();
	map<string, RepairJob*>::iterator  GetRJMapEnd();

	map<string, RepairJob*>::iterator  GetMyRJMapBegin();
	map<string, RepairJob*>::iterator  GetMyRJMapEnd();

	map<string, RepairJob*>::iterator  GetMyUnplannedMapBegin();
	map<string, RepairJob*>::iterator  GetMyUnplannedMapEnd();

	map<string, Distribution*>::iterator  GetRecurMapBegin();
	map<string, Distribution*>::iterator  GetRecurMapEnd();
	Aircraft* New(); //new aircraft creation, need to add appropriate parameters
	bool IsMapEnd(map<string, RepairJob*>::const_iterator iter);
	bool AreMoreJobs();
	bool AreMoreSteps();
	//bool HasRecurJob();
private:
	string	_aircraftType;								//	Aircraft name: i.e. F35
	int		_aircraftID;								//	Aircraft id: unique to individual aircraft
	static int _nextID;
	int _sourceID;
	int		_priority;									//	Aircraft priority: helps determine first to be served in queues
	int _numCalEvents;
	double	_length;									//	Aircraft size x dimension  ***make a tuple or struct later
	double	_wingspan;									//	Aircraft size y dimension
	string _baySizeReq;
	string	_repairJobName;
	//char _recurFlag = 'N';
	vector<Distribution*> _recurIatVec;
	//vector<string> _jobsLeft;
	//	Repair job name
	Distribution* _iatUnplanned;						//	Distribution for random aircraft interarrival times (iat) 
	Distribution* _iatRecurring;
	CalendarObj* _myCalObj;
	Step* _nextStep;
	map<string, RepairJob*> _myRepairJobs;
	map<string, RepairJob*> _myUnplannedJobsMap;
	map<string, Distribution*> _myRecurIATmap;				//  Recurring IATs for all of my recurring repairjobs
	vector<string> _unplannedRjVec;					//	Vector of rand repair job names unique to each new aircraft to be compared to master list
	map<string, RepairJob*> _allRepairJobsMap;			//	Map of all possible repair jobs
};
