//InputInterface.h: Andrea Robey
//This file is responsible for inputting data from the database into necessary formats and data structures
#pragma once
#include <map>
#include "Aircraft.h"
//#include "RepairJob.h"
//#include "Step.h"
//#include "Parts.h"
//#include "CalendarConverter.h"
//#include "Distribution.h"
//#include "Resource.h"

using namespace std;

class InputReader
{
public:
	InputReader();
	~InputReader();
	void ReadInputData();
	void PrintEverything();
	void AddSelectedAircraft(string aircraftName);
	void SetNumRuns(int numRuns);
	bool FindSelectedAircraft(string aircraftName);
	CalConverter* GetCalConverter();
	map<string, Aircraft*>::iterator GetMasterMapBegin();
	map<string, Aircraft*>::iterator GetMasterMapEnd();
	//	static void GetMyStep(string repairName, int stepId); //for getting step object 
	static Aircraft* GetAircraft(string aircraftName);
	static map<string, Aircraft*> GetMasterMap();
	static map<string, Resource*> GetMasterResourceMap();
	static int GetMapSize();
	static int GetNumRuns();
private:
	static int _numRuns;
	static map<string, Aircraft*> _masterMap;
	static map<string, Resource*> _masterResourceMap;
	CalConverter* calConvert;
	struct GUISelectedAircraft;
	GUISelectedAircraft* _GUIListHead;
	//map<Aircraft *, unordered_map<string, RepairJob>> masterMap;		//Master list of all repair job options for each unique aircraft type
	string _seedType; //seed type read in - if statement elsewhere

	//Trial CalObjs

};