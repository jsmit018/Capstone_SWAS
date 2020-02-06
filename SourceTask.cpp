#include "SourceTask.h"
#include "SimExec.h"

SourceBlock::SourceBlock(Distribution* interarrivalTimeRecurring, Distribution* interarrivalTimeCalendar, 
	Distribution* interarrivalTimeRND, string aircraftType, int numberOfAircraftToGenerate, 
	Time timeForFirstAircraft,	Aircraft* aircraft) : Task(name)  {
    _name = name;
    _interarrivalTimeRecurring = interarrivalTimeRecurring;
	_interarrivalTimeCalendar = interarrivalTimeCalendar;
	_interarrivaltimeRND = interarrivalTimeRND;
    _aircraftType = aircraftType;
    _aircraft = aircraft;
    _numberOfAircraftToGenerate = numberOfAircraftToGenerate;
   // _timeForFirstAircraft = timeForFirstAircraft;
    _numberGenerated = 0;
    //I believe we should change the name of this variable
    SimulationExecutive::ScheduleEventAt(timeForFirstAircraft, new ScheduleNextEntityEA(this));
}

class SourceBlock::ScheduleNextEntityCalendarEA : public EventAction {
public:
    ScheduleNextEntityEA(SourceBlock* source){
        _source = source;
    }
    void ExecuteEA(){
        _source->ScehduleNextEntityCalendarEM();
    }
private:
    SourceBlock* _source;
};

class SourceBlock::ScheduleNextRandomEntityEA : public EventAction {
public:
	ScheduleNextRandomEntityEA(SourceBlock* source) {
		_source = source;
	}

	void ExecuteEA() {
		_source->ScheduleNextRandomEntityEM();
	}

private:
	SourceBlock* _source;
};

class SourceBlock::ScheduleNextRecurringEA : public EventAction {
public:
	ScheduleNextRecurringEA(SourceBlock* source) {
		_source = source;
	}

	void ExecuteEA() {
		_source->ScheduleNextRecurringEM();
	}
private:
	SourceBlock* _source;
};

string SourceBlock::GetAircraftType() {
    return _aircraftType;
}

void  SourceBlock::SetAircraftType(string aircraftType) {
    _aircraftType = aircraftType;
}

int SourceBlock::GetNumberOfAircraftToGenerate() {
    _return _numberOfAircraftToGenerate();
}

string SourceBlock::GetName() {
    return _name;
}

void SourceBlock::SetName(string name) {
    _name = name;
}

Time SourceBlock::GetTimeForFirstAircraft() {
    return _timeForFirstAircraft;
}

void SourceBlock::SetTimeforFirstAircraft(Time setTime) {
    _timeForFirstAircraft = setTime;
}

int SourceBlock::GetNumberGenerated() {
    return _numberGenerated;
}

void SourceBlock::ScheduleNextEntityCalendarEM() {
   // while(_numberOFAircraftToGenerate != _numberGenerated){
        SimExec::ScheduleEventIn(_interarrivalTime->GetRV(), new ScheduleNextEntityEA(this));
		if (rand() >= .51) {
			SimExec::ScheduleEventIn(_interarrivalTimeRND->GetRV(), new ScheduleNextRandomEntityEa(this));
		}
        Depart(_aircraft->New());
        _numberGenerated++;
    //}
}

void SourceBlock::ScheduleNextRandomEntityEM() {
	Depart(_aircraft->New());
	_numberGenerated++;
}

void SourceBlock::ScheduleNextRecurringEA() {
	Depart(_aircraft->New());
	SimExec::ScheduleEventIn(_interarrivalTimeCalendar->GetRV(), new
		ScheduleNextEntityCalendarEA(this));
	if (rand() >= .51) {
		SimExec::ScheduleEventIn(_interarrivalTimeRND->GetRV(), new ScheduleNextRandomEntityEa(this));
	}
	_numberGenerated++;
}


void SourceBlock::Execute(Aircraft* aircraft){
	//This is just a declaration of the virtual function doesn't do anything
}
