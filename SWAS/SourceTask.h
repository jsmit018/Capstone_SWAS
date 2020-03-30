#pragma once
//#include "SimObj.h"
#include "SimExec.h"
#include "Task.h"
#include "Distribution.h"
#include "Aircraft.h"
#include "CalendarConverter.h"

class SourceBlock : public Task {
public:
	/*Constructor for Unplanned Arrivals*/
	SourceBlock(
		Distribution* unplannedIat, 
		string aircraftType, 
		Aircraft* aircraft, 
		string name,
		int numberOfAircraftToGenerate = NULL);

	/*Constructor for Recurring Arrivals*/
	/*SourceBlock(
		vector<Distribution*> recurringIATs,
		string aircraftType,
		Aircraft* aircraft,
		string name,
		int numberOfAircraftToGenerate = NULL);*/
	SourceBlock(
		//vector<Distribution*> recurringIATs,
		//map<string, Distribution*> recurringIATS,
		map<string, RepairJob*> list,
		string aircraftType,
		Aircraft* aircraft,
		string name,
		//string myRJ,
		int numberOfAircraftToGenerate = NULL);

	/*Constructor for Calendar Arrivals*/
	SourceBlock(
		string aircraftType,
		Aircraft* aircraft,
		string name,
		int numCalEventsToSched,
		CalendarObj* calobj);
		//Removing the below line, as this is set through numCalEvents
		//int numberOfAircraftToGenerate = NULL);

	/*Old Constructor*/
	/*SourceBlock(Distribution* iat,
		string aircraftType,
		Aircraft* aircraft,
		string name,
		int numOfCalendarEventsToSchedule,
		CalendarObj* calobj,
		int numberOfAircraftToGenerate = NULL);*/

	string GetAircraftType();
	void SetAircraftType(string aircraftType);
	string GetName();
	int GetNumberGenerated();
	void Execute(Aircraft* aircraft);
private:
	vector<Distribution*> _interarrivalTimeRecurring;
	Distribution* _interarrivalTimeRND;
	Aircraft* _aircraft;
	string _aircraftType;
	int _numberOfAircraftToGenerate;
	string _name;
	int _numberGenerated;
	class ScheduleNextCalendarAircraftEA;
	class ScheduleNextUnplannedAircraftEA;
	class ScheduleNextRecurringAircraftEA;
	void ScheduleNextUnplannedAircraftEM();
	void ScheduleNextCalendarAircraftEM();
	//void ScheduleNextRecurringAircraftEM();
	void ScheduleNextRecurringAircraftEM(RepairJob* repairJob);
};