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
	void PrintMasterResMap();
	void AddSelectedAircraft(string aircraftName);
	void SetNumRuns(int numRuns);
	bool FindSelectedAircraft(string aircraftName);
	CalConverter* GetCalConverter();
	map<string, Aircraft*>::iterator GetMasterMapBegin();
	map<string, Aircraft*>::iterator GetMasterMapEnd();
	static map<string, Resource*>::iterator GetMasterResMapBegin();
	static map<string, Resource*>::iterator GetMasterResMapEnd();
	static void SetMasterResNum(string name, int num);
	static void SetMasterPartsNum(string name, int num);
	//	static void GetMyStep(string repairName, int stepId); //for getting step object 
	static Aircraft* GetAircraft(string aircraftName);
	static map<string, Aircraft*> GetMasterMap();
	static map<string, Resource*> GetMasterResourceMap();
	static map<string, Parts*> GetMasterPartsMap();
	static int GetMapSize();
	static int GetNumRuns();
	static int GetAirCount();
	static void AddAirCount();
	static void CalAirFix();
private:
	static int _numRuns;
	static int _airCount;
	static map<string, Aircraft*>_masterMap;
	static map<string, Resource*> _masterResourceMap;
	static map<string, Parts*> _masterPartsMap;
	CalConverter* calConvert;
	struct GUISelectedAircraft;
	GUISelectedAircraft* _GUIListHead;
	//map<Aircraft *, unordered_map<string, RepairJob>> masterMap;		//Master list of all repair job options for each unique aircraft type
	string _seedType; //seed type read in - if statement elsewhere
	//Scribe* outputRecorder = new Scribe();

	//Trial CalObjs

};