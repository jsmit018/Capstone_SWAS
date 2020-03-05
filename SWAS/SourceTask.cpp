#include "SourceTask.h"
#include "SimExec.h"

class SourceBlock::ScheduleNextEntityCalendarEA : public EventAction {
public:
	ScheduleNextEntityCalendarEA(SourceBlock* source) {
		_source = source;
	}
	void Execute() {
		_source->ScheduleNextEntityCalendarEM();
	}
private:
	SourceBlock* _source;
};

SourceBlock::SourceBlock(Distribution* interarrivalTimeRecurring, Distribution* interarrivalTimeCalendar,
	Distribution* interarrivalTimeRND, string aircraftType, int numberOfAircraftToGenerate,
	Time timeForFirstAircraft, Aircraft* aircraft, string name, int numOfCalendarEventsToSchedule) : Task(name) {
	_name = name;
	_interarrivalTimeRecurring = interarrivalTimeRecurring;
	_interarrivalTimeCalendar = interarrivalTimeCalendar;
	_interarrivalTimeRND = interarrivalTimeRND;
	_aircraftType = aircraftType;
	_aircraft = aircraft;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
	_numberGenerated = 0;
	for (int i = 0; i < numOfCalendarEventsToSchedule; ++i){
		SimExec::ScheduleEventAtCalendar(0, 0, 0.0, 2020, _aircraft->GetAircraftPriority(), new ScheduleNextEntityCalendarEA(this));
	}
}

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
	SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomEntityEA(this), _interarrivalTimeRND->GetRV());
	Depart(_aircraft->New());
	_numberGenerated++;
}

void SourceBlock::ScheduleNextRandomEntityEM() {
	 Depart(_aircraft->New());
	_numberGenerated++;
}

void SourceBlock::ScheduleNextRecurringEM() {
	SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRecurringEA(this), _interarrivalTimeRecurring->GetRV());
	SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomEntityEA(this), _interarrivalTimeRND->GetRV());
	Depart(_aircraft->New());
	_numberGenerated++;
}


void SourceBlock::Execute(Aircraft* aircraft) {
	//This is just a declaration of the virtual function doesn't do anything
}
