/*InputInterface.h: Andrea Robey

This file acts as an interface between the csv input file and the simulation.
It reads the file and parses the data from the tables in the file into necessary formats 
and data structures which will be used to populate the many objects' attributes. 
*/

#pragma once
#include <map>
#include "Aircraft.h"

using namespace std;

class InputReader
{
public:
	InputReader();
	~InputReader();
	void ReadInputData();
	void PrintEverything();
	void PrintMasterResMap();
	void ClearGuiAircraft();
	void AddSelectedAircraft(int aircraftNumber);
	void SetNumRuns(int numRuns);
	void ResetAirCount();
	bool FindSelectedAircraft(string aircraftName);

	CalConverter* GetCalConverter();
	CalendarObj* GetTerminationObj();

	static map<string, Aircraft*>::iterator GetMasterMapBegin();
	static map<string, Aircraft*>::iterator GetMasterMapEnd();
	static map<string, RepairJob*>::iterator FindMasterRJ(string aircraft, string job);
	static map<string, StepResource*>::iterator GetMasterResMapBegin();
	static map<string, StepResource*>::iterator GetMasterResMapEnd();
	static void SetMasterResNum(string name, int num);
	static void SetMasterPartsNum(string name, int num);
	static void ResetIDCount();
	static Aircraft* GetAircraft(string aircraftName);
	static map<string, Aircraft*> GetMasterMap();
	static map<string, StepResource*> GetMasterResourceMap();
	static map<string, Parts*> GetMasterPartsMap();
	static int GetMapSize();
	static int GetNumRuns();
	static int GetAirCount();
	static int GetIDcount();
	static void AddAirCount();
	static void CalAirFix();
	static void SetShiftOneStartTime(int starttime);
	static double GetShiftOneStartTime();
	static void SetShiftTwoStartTime(int starttime);
	static double GetShiftTwoStartTime();
	static void SetShiftThreeStartTime(int starttime);
	static double GetShiftThreeStartTime();
	static bool IsWartime();
	static void SetWartimeFlag(int flag);

private:
	static int _wartimeFlag;
	static int _numRuns;
	static int _airCount;
	static int _IDcount;
	static double _shiftOneStartTime;
	static double _shiftTwoStartTime;
	static double _shiftThreeStartTime;
	static map<string, Aircraft*>_masterMap;
	static map<string, StepResource*> _masterResourceMap;
	static map<string, Parts*> _masterPartsMap;
	map<int, string> _addedAircraft;
	CalConverter* calConvert;
	CalendarObj* _terminationDates;
	struct GUISelectedAircraft;
	GUISelectedAircraft* _GUIListHead;
	string _seedType; //seed type read in - if statement elsewhere
};