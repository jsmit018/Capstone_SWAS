#pragma once
//#include "SimObj.h"
#include "SimExec.h"
#include "Task.h"
#include "Distribution.h"
#include "Aircraft.h"
#include "CalendarConverter.h"

class SourceBlock : public Task {
public:
	SourceBlock(Distribution* _interarrivalTimeRecurring, string aircraftType, Time timeForFirstAircraft, 	
		Aircraft* aircraft, string name, int numOfCalendarEventsToSchedule, CalendarObj* calobj, int numberOfAircraftToGenerate = NULL);
	string GetAircraftType();
	void SetAircraftType(string aircraftType);
	string GetName();
	int GetNumberGenerated();
	void Execute(Aircraft* aircraft);
private:
	Distribution* _interarrivalTimeRecurring;
	Distribution* _interarrivalTimeRND;
	Aircraft* _aircraft;
	string _aircraftType;
	int _numberOfAircraftToGenerate;
	string _name;
	int _numberGenerated;
	class ScheduleNextEntityCalendarEA;
	class ScheduleNextRandomEntityEA;
	class ScheduleNextRecurringEA;
	void ScheduleNextRandomEntityEM();
	void ScheduleNextEntityCalendarEM();
	void ScheduleNextRecurringEM();
};