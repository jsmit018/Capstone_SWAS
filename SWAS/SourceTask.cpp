#include "SourceTask.h"
#include "SimExec.h"

SourceBlock::SourceBlock(Distribution* interarrivalTimeRecurring, Distribution* interarrivalTimeCalendar, 
	Distribution* interarrivalTimeRND, string aircraftType, int numberOfAircraftToGenerate, 
	Time timeForFirstAircraft,	Aircraft* aircraft, string name) : Task(name) {
    _name = name;
    _interarrivalTimeRecurring = interarrivalTimeRecurring;
	_interarrivalTimeCalendar = interarrivalTimeCalendar;
	_interarrivalTimeRND = interarrivalTimeRND;
    _aircraftType = aircraftType;
    _aircraft = aircraft;
    _numberOfAircraftToGenerate = numberOfAircraftToGenerate;
   // _timeForFirstAircraft = timeForFirstAircraft;
    _numberGenerated = 0;
    //I believe we should change the name of this variable
    //SimExec::ScheduleEventAt(timeForFirstAircraft, new ScheduleNextEntityEA(this));
}

class SourceBlock::ScheduleNextEntityCalendarEA : public EventAction {
public:
    ScheduleNextEntityCalendarEA(SourceBlock* source){
        _source = source;
    }
    void Execute(){
		_source->ScheduleNextEntityCalendarEM();
    }
private:
    SourceBlock* _source;
};

class SourceBlock::ScheduleNextRandomEntityEA : public EventAction {
public:
	ScheduleNextRandomEntityEA(SourceBlock* source) {
		_source = source;
	}

	void Execute() {
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

	void Execute() {
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

string SourceBlock::GetName() {
    return _name;
}

int SourceBlock::GetNumberGenerated() {
    return _numberGenerated;
}

void SourceBlock::ScheduleNextEntityCalendarEM() {
   // while(_numberOFAircraftToGenerate != _numberGenerated){
       // SimExec::ScheduleEventIn(_interarrivalTimeCalendar->GetRV(), new ScheduleNextEntityCalendarEA(this));
		if (rand() >= .51) {
			//SimExec::ScheduleEventIn(_interarrivalTimeRND->GetRV(), new ScheduleNextRandomEntityEA(this));
		}
       // Depart(_aircraft->New());
        _numberGenerated++;
    //}
}

void SourceBlock::ScheduleNextRandomEntityEM() {
	//Depart(_aircraft->New());
	_numberGenerated++;
}

void SourceBlock::ScheduleNextRecurringEM() {
	//Depart(_aircraft->New());
	//SimExec::ScheduleEventIn(_interarrivalTimeCalendar->GetRV(), new
		//ScheduleNextEntityCalendarEA(this));
	if (rand() >= .51) {
		//SimExec::ScheduleEventIn(_interarrivalTimeRND->GetRV(), new ScheduleNextRandomEntityEA(this));
	}
	_numberGenerated++;
}


void SourceBlock::Execute(Aircraft* aircraft){
	//This is just a declaration of the virtual function doesn't do anything
}
