#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Distribution.h"
#include "Aircraft.h"

class SourceBlock : public Task {
public:
	SourceBlock(Distribution* _interarrivalTimeRecurring, Distribution* _interarrivalTimeCalendar,
		Distribution* _interarrivalTimeRND, string aircraftType, int numberOfAircraftToGenerate, Time timeForFirstAircraft, 
		Aircraft* aircraft, string name, int numOfCalendarEventsToSchedule);
	//Time GetInterarrivalTime();
	//void SetInterarrivalTime(Time interarrivalTime);
	string GetAircraftType();
	void SetAircraftType(string aircraftType);
	//void SetNumberOfAircraftToGenerate(int numberToGenerate);
	string GetName();
	int GetNumberGenerated();
	//void SetNumberGenerated(int numberGenerated);
	void Execute(Aircraft* aircraft);
private:
	Distribution* _interarrivalTimeRecurring;
	Distribution* _interarrivalTimeCalendar;
	Distribution* _interarrivalTimeRND;
	//Distribution* _randomAircraftGeneration;
	Aircraft* _aircraft;
	string _aircraftType;
	int _numberOfAircraftToGenerate;
	string _name;
	//Time _timeForFirstAircraft;
	int _numberGenerated;
	class ScheduleNextEntityCalendarEA;
	class ScheduleNextRandomEntityEA;
	class ScheduleNextRecurringEA;
	void ScheduleNextRandomEntityEM();
	void ScheduleNextEntityCalendarEM();
	void ScheduleNextRecurringEM();
};