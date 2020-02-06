#pragma once
#include "SimObj.h"
#include "Task.h"

class SourceBlock : public Task {
public:
	Source(Distribution* _interarrivalTimeRecurring, Distribution* _interarrivalTimeCalendar,
		Distribution* _interarrivalTimeRND, string aircraftType, int numberOfAircraftToGenerate,
		Time timeForFirstAircraft, Aircraft* aircraft);
	//Time GetInterarrivalTime();
	//void SetInterarrivalTime(Time interarrivalTime);
	string GetAircraftType();
	void SetAircraftType(string aircraftType);
	int GetNumberOfAircraftToGenerate();
	//void SetNumberOfAircraftToGenerate(int numberToGenerate);
	string GetName();
	void SetName(string name);
	Time GetTimeForFirstAircraft();
	void SetTimeforFirstAircraft(Time setTime);
	int GetNumberGenerated();
	//void SetNumberGenerated(int numberGenerated);
	void Exeucte(Aircraft* aircraft);
private:
	Distribution* _interarrivalTimeRecurring;
	Distribution* _interarrivalTimeCalendar;
	Distribution* _interarrivalTimeRND;
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
