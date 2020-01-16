#pragma once
#include "SimObj.h"
#include "Task.h"

class SourceBlock : public Task {
public:
	Source();
	Time GetInterarrivalTime();
	void SetInterarrivalTime();
	string GetAircraftType();
	void SetAircraftType(string aircraftType);
	int GetNumberOfAircraftToGenerate();
	void SetNumberOfAircraftToGenerate(int numberToGenerate);
	string GetName();
	void SetName(string name);
	Time GetTimeForFirstAircraft();
	void SetTimeforFirstAircraft(Time setTime);
	int GetNumberGenerated();
	void SetNumberGenerated(int numberGenerated);
	void Exeucte(Airplane* airplane);
private:
	Time _interarrivalTime;
	string _aircraftType;
	int _numberOfAircraftToGenerate;
	string _name;
	Time _timeForFirstAircraft;
	int _numberGenerated;
	class ScheduleNextEntityEA;
	void ScheduleNextEntityEM(Airplane* airplane);

};