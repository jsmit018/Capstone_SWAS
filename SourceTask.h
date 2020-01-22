#pragma once
#include "SimObj.h"
#include "Task.h"

class SourceBlock : public Task {
public:
	Source((Distribution* interarrivalTime, string aircraftType, int numberOfAircraftToGenerate
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
	void Exeucte(Airplane* airplane);
private:
	Distribution* _interarrivalTime;
	Aircraft* _aircraft;
	string _aircraftType;
	int _numberOfAircraftToGenerate;
	string _name;
	//Time _timeForFirstAircraft;
	int _numberGenerated;
	class ScheduleNextEntityEA;
	void ScheduleNextEntityEM(Airplane* airplane);

};
