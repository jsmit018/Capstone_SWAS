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

class SourceBlock::ScheduleNextUnplannedAircraftEA : public EventAction {
public:
	ScheduleNextUnplannedAircraftEA(SourceBlock* source) {
		_source = source;
	}

	void Execute() {
		_source->ScheduleNextUnplannedAircraftEM();
	}

private:
	SourceBlock* _source;
};

/*Constructor for Unplanned*/
SourceBlock::SourceBlock(Distribution* iat, string aircraftType, Aircraft* aircraft, string name, 
	int numberOfAircraftToGenerate) : Task(name)
{
	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl;
	_name = name;
	
	cout << "Initializing Aircraft Distributions" << endl;
	_interarrivalTimeRND = aircraft->GetAircraftIAT();
	
	cout << "Printing Aircraft Distributions" << endl;
	_interarrivalTimeRND->PrintDistribution();

	_aircraftType = aircraftType;
	_aircraft = aircraft;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
	_numberGenerated = 0;

	SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new ScheduleNextUnplannedAircraftEA(this),
		_interarrivalTimeRND->GetRV(), "ScheduleNextUnplannedAircraftEA");
}

/*Constructor for Recurring*/
SourceBlock::SourceBlock(vector<Distribution*> recurringIAT, string aircraftType, Aircraft* aircraft, string name,
	int numberOfAircraftToGenerate) : Task(name)
{
	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl;
	_name = name;
	
	cout << "Initializing Aircraft Distributions" << endl;
	/*need to take the vector of recurring IATs and set multiple*/
	//_interarrivalTimeRecurring = 
	_interarrivalTimeRecurring = recurringIAT;

	cout << "Printing Aircraft Distributions" << endl;
	
	for (int i = 0; i < _interarrivalTimeRecurring.size(); ++i) {
		_interarrivalTimeRecurring[i]->PrintDistribution();
	} 

	_aircraftType = aircraftType;
	_aircraft = aircraft;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
	_numberGenerated = 0;

	for (int i = 0; i < _interarrivalTimeRecurring.size(); ++i) {
		SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this),
			_interarrivalTimeRecurring[i]->GetRV(), "ScheduleNextRecurringAircraftEA");
	}
}

/*Constructor for Calendar*/
SourceBlock::SourceBlock(string aircraftType, Aircraft* aircraft, string name,
	int numCalEventsToSched, CalendarObj* calobj, int numberOfAircraftToGenerate) : Task(name)
{
	cout << "Scheduling Calendar Events" << endl;
	for (int i = 0; i < numCalEventsToSched; ++i) {
		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _aircraft->GetAircraftPriority(), new ScheduleNextCalendarAircraftEA(this), "ScheduleNextCalendarAircraftEA");
		cout << "Calendar Event Scheduled" << endl;
	}
}

/*Old Constructor*/
//SourceBlock::SourceBlock(Distribution* interarrivalTimeRecurring, string aircraftType, Aircraft* aircraft, string name,
//	int numOfCalendarEventsToSchedule, CalendarObj* calobj, int numberOfAircraftToGenerate) : Task(name) {
//	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl;
//	_name = name;
//	cout << "Initializing Aircraft Distributions" << endl;
//	_interarrivalTimeRecurring = interarrivalTimeRecurring;
//	_interarrivalTimeRND = aircraft->GetAircraftIAT();
//	cout << "Printing Aircraft Distributions" << endl;
//	_interarrivalTimeRecurring->PrintDistribution();
//	_interarrivalTimeRND->PrintDistribution();
//	_aircraftType = aircraftType;
//	_aircraft = aircraft;
//	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
//	_numberGenerated = 0;
//	cout << "Scheduling Calendar Events" << endl;
//	for (int i = 0; i < numOfCalendarEventsToSchedule; ++i) {
//		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _aircraft->GetAircraftPriority(), new ScheduleNextCalendarAircraftEA(this), "ScheduleNextCalendarAircraftEA");
//		cout << "Calendar Event Scheduled" << endl;
//	}
//	SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this), _interarrivalTimeRecurring->GetRV(), "ScheduleNextRecurringAircraftEA");
//}

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
		//Not doing this anymore!
		//cout << "Scheduling Random aircraft arrival" << endl;
		//SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomAircraftEA");
		cout << "Departing new Aircraft" << endl;
		Depart(_aircraft->New());
		_numberGenerated++;
	}
}

void SourceBlock::ScheduleNextUnplannedAircraftEM() {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		cout << "Scheduling Unplanned Aircraft Arrival" << endl;
		SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextUnplannedAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextUnplannedAircraftEA");
		cout << "Departing Unexpected Arrival" << endl;
		Depart(_aircraft->New());
		_numberGenerated++;
	}
}

void SourceBlock::ScheduleNextRecurringAircraftEM() {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		//Scheduling recurring aircrafts
		for (int i = 0; i < _interarrivalTimeRecurring.size(); ++i) {
			cout << "Scheduling next Recurring arrival" << endl;
			SimExec::ScheduleEventAtRecurring(_aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this), _interarrivalTimeRecurring[i]->GetRV(), "ScheduleNextRecurringAircraftEA");
		}
		//No longer doing this!
		//cout << "Scheduling next Random Aircraft arrival" << endl;
		//SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomAircraftEA");
		cout << "Departing new Aircraft" << endl;
		Depart(_aircraft->New());
		_numberGenerated++;
	}
}

void SourceBlock::Execute(Aircraft* aircraft) {
	//This is just a declaration of the virtual function doesn't do anything
}