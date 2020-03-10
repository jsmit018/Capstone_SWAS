#include "SourceTask.h"
//#include "SimExec.h"

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

SourceBlock::SourceBlock(Distribution* interarrivalTimeRecurring, string aircraftType, int numberOfAircraftToGenerate, 
	Time timeForFirstAircraft, Aircraft* aircraft, string name, int numOfCalendarEventsToSchedule, CalendarObj* calobj) : Task(name) {
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
	for (int i = 0; i < numOfCalendarEventsToSchedule; ++i){
		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_years[i], 
				_aircraft->GetAircraftPriority(), new ScheduleNextEntityCalendarEA(this), "ScheduleNextEntityCalendarEA");
		cout << "Calendar Event Scheduled" << endl;
	}
	SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringEA(this), _interarrivalTimeRecurring->GetRV(), "ScheduleNextRecurringEA");
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
	cout << "Scheduling Random aircraft arrival" << endl;
	SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomEntityEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomEntityEA");
	cout << "Departing new Aircraft" << endl;
	Depart(_aircraft->New());
	_numberGenerated++;
}

void SourceBlock::ScheduleNextRandomEntityEM() {
	cout << "Departing Unexpected Arrival" << endl;
	 Depart(_aircraft->New());
	_numberGenerated++;
}

void SourceBlock::ScheduleNextRecurringEM() {
	cout << "Scheduling next Recurring arrival" << endl;
	SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRecurringEA(this), _interarrivalTimeRecurring->GetRV(), "ScheduleNextRecurringEA");
	cout << "Scheduling next Random Aircraft arrival" << endl;
	SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomEntityEA(this), _interarrivalTimeRND->GetRV(), "SchedulenextRandomEntityEA");
	cout << "Departing new Aircraft" << endl;
	Depart(_aircraft->New());
	_numberGenerated++;
}


void SourceBlock::Execute(Aircraft* aircraft) {
	//This is just a declaration of the virtual function doesn't do anything
}
