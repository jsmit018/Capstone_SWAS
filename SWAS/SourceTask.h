//SourceTask.h: Jordan Smith and Andrea Robey

#pragma once
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
	SourceBlock(
		map<string, Distribution*> recurringIATS,
		string aircraftType,
		Aircraft* aircraft,
		string name,
		int numberOfAircraftToGenerate = -1); 

	/*Constructor for Calendar Arrivals*/
	SourceBlock(
		string aircraftType,
		Aircraft* aircraft,
		string name,
		CalendarObj* calobj,
		RepairJob* repairJob); 

	string GetAircraftType(); 
	void SetAircraftType(string aircraftType); 
	string GetName(); 
	int GetNumberGenerated(); 
	void Execute(Aircraft* aircraft); 
	void AddToPriorityMap(int priority, string job); 
private:
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
	void ScheduleNextRecurringAircraftEM(Distribution* recurringIAT, RepairJob* repairJob); 
}; 