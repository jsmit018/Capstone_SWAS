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
	CalConverter* GetCalConverter();
	CalendarObj* GetF35Calendar();
	CalendarObj* GetF18Calendar();
	CalendarObj* GetFixedWingCalendar();
	CalendarObj* GetApacheCalendar();
	map<string, Aircraft*>::iterator GetMasterMapBegin();
	map<string, Aircraft*>::iterator GetMasterMapEnd();
//	static void GetMyStep(string repairName, int stepId); //for getting step object 
	static Aircraft* GetAircraft(string aircraftName);
	static map<string, Aircraft*> GetMasterMap();
	static int GetMapSize();
private:
	static map<string, Aircraft*> _masterMap;
	static map<string, Resource*> _masterResourceMap;
	CalConverter* calConvert;
	//map<Aircraft *, unordered_map<string, RepairJob>> masterMap;		//Master list of all repair job options for each unique aircraft type
	string _seedType; //seed type read in - if statement elsewhere

	//Trial CalObjs
	CalendarObj* F_35 = new CalendarObj();
	CalendarObj* F_18 = new CalendarObj();
	CalendarObj* Fixed_Wing = new CalendarObj();
	CalendarObj* Apache = new CalendarObj();

};