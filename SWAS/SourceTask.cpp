#include "SourceTask.h"
//#include "SimExec.h"

class SourceBlock::ScheduleNextCalendarAircraftEA : public EventAction {
public:
	ScheduleNextCalendarAircraftEA(SourceBlock* source) {
		_source = source;
	}
	void Execute() {
		_source->ScheduleNextCalendarAircraftEM();
	}
private:
	SourceBlock* _source;
};

class SourceBlock::ScheduleNextRecurringAircraftEA : public EventAction {
public:
	ScheduleNextRecurringAircraftEA(SourceBlock* source) {
		_source = source;
	}

	void Execute() {
		_source->ScheduleNextRecurringAircraftEM();
	}
private:
	SourceBlock* _source;
};

SourceBlock::SourceBlock(Distribution* interarrivalTimeRecurring, string aircraftType, Aircraft* aircraft, string name,
	int numOfCalendarEventsToSchedule, CalendarObj* calobj, int numberOfAircraftToGenerate) : Task(name) {
	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl;
	_name = name;
	cout << "Initializing Aircraft Distributions" << endl;
	_interarrivalTimeRecurring = interarrivalTimeRecurring;
	_interarrivalTimeRND = aircraft->GetAircraftIAT();
	cout << "Printing Aircraft Distributions" << endl;
	_interarrivalTimeRecurring->PrintDistribution();
	_interarrivalTimeRND->PrintDistribution();
	_aircraftType = aircraftType;
	_aircraft = aircraft;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
	_numberGenerated = 0;
	cout << "Scheduling Calendar Events" << endl;
	for (int i = 0; i < numOfCalendarEventsToSchedule; ++i) {
		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _aircraft->GetAircraftPriority(), new ScheduleNextCalendarAircraftEA(this), "ScheduleNextCalendarAircraftEA");
		cout << "Calendar Event Scheduled" << endl;
	}
	SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this), _interarrivalTimeRecurring->GetRV(), "ScheduleNextRecurringAircraftEA");
}

class SourceBlock::ScheduleNextRandomAircraftEA : public EventAction {
public:
	ScheduleNextRandomAircraftEA(SourceBlock* source) {
		_source = source;
	}

	void Execute() {
		_source->ScheduleNextRandomAircraftEM();
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

void SourceBlock::ScheduleNextCalendarAircraftEM() {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		cout << "Scheduling Random aircraft arrival" << endl;
		SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomAircraftEA");
		cout << "Departing new Aircraft" << endl;
		Depart(_aircraft->New());
		_numberGenerated++;
	}
}

void SourceBlock::ScheduleNextRandomAircraftEM() {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		cout << "Departing Unexpected Arrival" << endl;
		Depart(_aircraft->New());
		_numberGenerated++;
	}
}

void SourceBlock::ScheduleNextRecurringAircraftEM() {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		cout << "Scheduling next Recurring arrival" << endl;
		SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this), _interarrivalTimeRecurring->GetRV(), "ScheduleNextRecurringAircraftEA");
		cout << "Scheduling next Random Aircraft arrival" << endl;
		SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomAircraftEA");
		cout << "Departing new Aircraft" << endl;
		Depart(_aircraft->New());
		_numberGenerated++;
	}
}

void SourceBlock::Execute(Aircraft* aircraft) {
	//This is just a declaration of the virtual function doesn't do anything
}