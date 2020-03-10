#pragma once
//#include "SimObj.h"
#include "SimExec.h"
#include "Task.h"
#include "Distribution.h"
#include "Aircraft.h"
#include "CalendarConverter.h"

class SourceBlock : public Task {
public:
	SourceBlock(Distribution* _interarrivalTimeRecurring, Distribution* _interarrivalTimeCalendar,
		string aircraftType, int numberOfAircraftToGenerate, Time timeForFirstAircraft, 
		Aircraft* aircraft, string name, int numOfCalendarEventsToSchedule, CalendarObj* calobj);
	string GetAircraftType();
	void SetAircraftType(string aircraftType);
	string GetName();
	int GetNumberGenerated();
	void Execute(Aircraft* aircraft);
private:
	Distribution* _interarrivalTimeRecurring;
	Distribution* _interarrivalTimeCalendar;
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