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
		RepairJob* repairJob,
		int numberOfAircraftToGenerate = -1); 

	/*Constructor for Recurring Arrivals*/
	/*SourceBlock(
		vector<Distribution*> recurringIATs,
		string aircraftType,
		Aircraft* aircraft,
		string name,
		int numberOfAircraftToGenerate = NULL); */
	SourceBlock(
		//vector<Distribution*> recurringIATs,
		map<string, Distribution*> recurringIATS,
		//map<string, RepairJob*> list, //this is using unecessary memory - just look it up from aircraft's prexisting map - see sourcetask.cpp
		string aircraftType,
		Aircraft* aircraft,
		string name,
		//string myRJ,
		int numberOfAircraftToGenerate = -1); 

	/*Constructor for Calendar Arrivals*/
	SourceBlock(
		string aircraftType,
		Aircraft* aircraft,
		string name,
		//int numCalEventsToSched,
		CalendarObj* calobj,
		RepairJob* repairJob); 
	//Removing the below line, as this is set through numCalEvents
	//int numberOfAircraftToGenerate = NULL); 

/*Old Constructor*/
/*SourceBlock(Distribution* iat,
	string aircraftType,
	Aircraft* aircraft,
	string name,
	int numOfCalendarEventsToSchedule,
	CalendarObj* calobj,
	int numberOfAircraftToGenerate = NULL); */

	string GetAircraftType(); 
	void SetAircraftType(string aircraftType); 
	string GetName(); 
	int GetNumberGenerated(); 
	void Execute(Aircraft* aircraft); 
	void AddToPriorityMap(int priority, string job); 
private:
	//vector<Distribution*> _interarrivalTimeRecurring; 
	map<string, Distribution*> _recurringIATs; 
	map<int, string> _jobPriority; 
	Distribution* _interarrivalTimeRND; 
	Aircraft* _aircraft; 
	string _aircraftType; 
	int _numberOfAircraftToGenerate; 
	string _name; 
	int _numberGenerated; 
	class ScheduleNextCalendarAircraftEA; 
	class ScheduleNextUnplannedAircraftEA; 
	class ScheduleNextRecurringAircraftEA; 
	void ScheduleNextUnplannedAircraftEM(RepairJob* repairJob); 
	void ScheduleNextCalendarAircraftEM(RepairJob* repairJob, CalendarObj* calobj); 
	//void ScheduleNextRecurringAircraftEM(); 
	void ScheduleNextRecurringAircraftEM(Distribution* recurringIAT, RepairJob* repairJob); 
	//Scribe* outputRecorder = new Scribe(); 
}; 