#include "SourceTask.h"
//#include "SimExec.h"

/**
 * A class that inherits from the source block that schedules the next calendar aircraft event action. 
 */
class SourceBlock::ScheduleNextCalendarAircraftEA : public EventAction {
public:
	/**
	 * Sets source.
	 */
	ScheduleNextCalendarAircraftEA(SourceBlock* source) {
		_source = source;
	}

	/**
	 * Schedules next calendar aircraft event method of source.
	 */
	void Execute() {
		_source->ScheduleNextCalendarAircraftEM();
	}
private:
	SourceBlock* _source;
};

//class SourceBlock::ScheduleNextRecurringAircraftEA : public EventAction {
//public:
//	ScheduleNextRecurringAircraftEA(SourceBlock* source) {
//		_source = source;
//	}
//
//	void Execute() {
//		_source->ScheduleNextRecurringAircraftEM();
//	}
//private:
//	SourceBlock* _source;
//};

/**
 * A class that inherits from the source block and schedules the next recurring aircraft event action.
 */
class SourceBlock::ScheduleNextRecurringAircraftEA : public EventAction {
public:
	/**
	 * Sets source, repair job, and recurring IAT.
	 */
	ScheduleNextRecurringAircraftEA(SourceBlock* source, Distribution* recurringiAT, RepairJob* repairJob) {
		_source = source;
		_repairJob = repairJob;
		_recurringIAT = recurringiAT;
	}
	/**
	 * Schedules next recurring aircraft event method of source.
	 */
	void Execute() {
		_source->ScheduleNextRecurringAircraftEM(_recurringIAT, _repairJob);
	}
private:
	SourceBlock* _source;
	RepairJob* _repairJob;
	Distribution* _recurringIAT;
};

/**
 * A class inherited by the source block and schedules the next unplanned aircraft event action.
 */
class SourceBlock::ScheduleNextUnplannedAircraftEA : public EventAction {
public:
	/**
	 * Sets source.
	 */
	ScheduleNextUnplannedAircraftEA(SourceBlock* source) {
		_source = source;
	}
	/**
	 * Schedules next unplanned aircraft event method of source.
	 */
	void Execute() {
		_source->ScheduleNextUnplannedAircraftEM();
	}

private:
	SourceBlock* _source;
};

/*Constructor for Unplanned*/
/**
 * Source block Constructor for an unplanned source block that initializes the aircraft and schedules the next unplanned aircraft event action.
 */
SourceBlock::SourceBlock(Distribution* iat, string aircraftType, Aircraft* aircraft, string name,
	int numberOfAircraftToGenerate) : Task(name)
{
	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl;
	_name = name;

	//cout << "Initializing Aircraft Distributions" << endl;
	_interarrivalTimeRND = iat->CopyThis();

	//cout << "Printing Aircraft Distributions" << endl;
	//_interarrivalTimeRND->PrintDistribution();

	_aircraftType = aircraftType;
	_aircraft = aircraft;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
	_numberGenerated = 0;

	//cout << "Scheduling First Unplanned Aircraft Arrival" << endl;
	SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new ScheduleNextUnplannedAircraftEA(this),
		_interarrivalTimeRND->GetRV(), "ScheduleNextUnplannedAircraftEA");
}

/*Constructor for Recurring*/
//SourceBlock::SourceBlock(vector<Distribution*> recurringIAT, string aircraftType, Aircraft* aircraft, string name,
//	int numberOfAircraftToGenerate) : Task(name)
//{
//	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl;
//	_name = name;
//
//	cout << "Initializing Aircraft Distributions" << endl;
//	/*need to take the map of recurring IATs and set multiple*/
//	//_interarrivalTimeRecurring = 
//	_interarrivalTimeRecurring = recurringIAT;
//
//	cout << "Printing Aircraft Distributions" << endl;
//
//	for (int i = 0; i < _interarrivalTimeRecurring.size(); ++i) {
//		_interarrivalTimeRecurring[i]->PrintDistribution();
//	}
//
//	_aircraftType = aircraftType;
//	_aircraft = aircraft;
//	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
//	_numberGenerated = 0;
//
//	cout << "Scheduling first Recurring Aircraft Arrival(s)" << endl;
//	for (int i = 0; i < _interarrivalTimeRecurring.size(); ++i) {
//		SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this),
//			_interarrivalTimeRecurring[i]->GetRV(), "ScheduleNextRecurringAircraftEA");
//	}
//}



//recurring constructor
/**
 * Source block constructor for recurring events.
 */
SourceBlock::SourceBlock(map<string, Distribution*> recurringIATS, string aircraftType, Aircraft* aircraft, string name,
	int numberOfAircraftToGenerate) : Task(name)
{
	cout << "Initialzing Source Object for " << aircraft->GetAircraftType() << endl;
	_name = name;

	cout << "Initializing Aircraft Distributions" << endl;
	/*need to take the map of recurring IATs and set multiple*/
	_recurringIATs = recurringIATS;

	cout << "Printing Aircraft Distributions" << endl;
	map<string, Distribution*>::const_iterator it = _recurringIATs.begin();
	while (it != _recurringIATs.end()) {
		it->second->PrintDistribution();
		it++;
	}

	_aircraftType = aircraftType;
	_aircraft = aircraft;
	_numberOfAircraftToGenerate = numberOfAircraftToGenerate;
	_numberGenerated = 0;

	it = _recurringIATs.begin();
	//cout << "Scheduling first Recurring Aircraft Arrival(s)" << endl;
	//for (int i = 0; i < _interarrivalTimeRecurring.size(); ++i) {
	while (it != _recurringIATs.end()) {
		//map<string, RepairJob*>::const_iterator iter = aircraft->GetMyRJMapBegin();
		/*while (iter != aircraft->GetMyRJMapEnd()) {
			if (it->first == iter->first)
				break;
			else {
				iter++;
			}
		}*/
		RepairJob* newptr = aircraft->GetMyRepairJobObj(it->first);
		SimExec::ScheduleEventAtRecurring(aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this, it->second, newptr),
			it->second->GetRV(), "ScheduleNextRecurringAircraftEA");
		it++;
		delete newptr;
	}
}

/*Constructor for Calendar*/
//scheduling aircraft arrival
/**
 * Source block constructor for calendar.
 */
SourceBlock::SourceBlock(string aircraftType, Aircraft* aircraft, string name,
	/*int numCalEventsToSched,*/ CalendarObj* calobj) : Task(name)
{
	//cout << "Scheduling list of Calendar Events" << endl;
	//for (int i = 0; i < numCalEventsToSched; ++i) {
	//	SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _aircraft->GetAircraftPriority(), new ScheduleNextCalendarAircraftEA(this), "ScheduleNextCalendarAircraftEA");
	//	cout << "Calendar Event Scheduled" << endl;
	//}
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

/**
 * Returns the aircraft type.
 */
string SourceBlock::GetAircraftType() {
	return _aircraftType;
}

/**
 * Sets the aircraft type.
 */
void  SourceBlock::SetAircraftType(string aircraftType) {
	_aircraftType = aircraftType;
}

/**
 * Returns the name.
 */
string SourceBlock::GetName() {
	return _name;
}

/**
 * Returns the number of generated aircrafts.
 */
int SourceBlock::GetNumberGenerated() {
	return _numberGenerated;
}

/**
 * Schedules the next calendar aircraft event method of source and increments number generated for the source block.
 */
void SourceBlock::ScheduleNextCalendarAircraftEM() {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		//Not doing this anymore!
		//cout << "Scheduling Random aircraft arrival" << endl;
		//SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomAircraftEA");
		cout << "Calendar Scheduled aircraft has arrived, ";



		cout << "Departing new Aircraft" << endl;
		//Depart(_aircraft->New());
		//_aircraft->New()->CopyMyJobList(_aircraft->GetAircraftType());
		_numberGenerated++;
	}
}

/**
 * Schedules the next unplanned aircraft event method and increments number generated for the source block.
 */
void SourceBlock::ScheduleNextUnplannedAircraftEM() {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		cout << "Unplanned Aircraft has arrived, ";
		cout << "Scheduling Unplanned Aircraft Arrival" << endl;
		SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextUnplannedAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextUnplannedAircraftEA");
		cout << "Departing Unplanned Arrival" << endl;
		//_aircraft->New()->CopyMyJobList(_aircraft->GetAircraftType());
		//Depart(_aircraft->New());
		_numberGenerated++;
	}
}

//void SourceBlock::ScheduleNextRecurringAircraftEM() {
//	if (_numberGenerated != _numberOfAircraftToGenerate) {
//		//Scheduling recurring aircrafts
//		for (int i = 0; i < _interarrivalTimeRecurring.size(); ++i) {
//			cout << "Recurring Aircraft has arrived, ";
//			cout << "Scheduling next Recurring arrival(s)" << endl;
//			SimExec::ScheduleEventAtRecurring(_aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this), _interarrivalTimeRecurring[i]->GetRV(), "ScheduleNextRecurringAircraftEA");
//		}
//		//No longer doing this!
//		//cout << "Scheduling next Random Aircraft arrival" << endl;
//		//SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomAircraftEA");
//		cout << "Departing new Aircraft" << endl;
//		//Depart(_aircraft->New());
//		//_aircraft->New()->CopyMyJobList(_aircraft->GetAircraftType());
//		_numberGenerated++;
//	}
//}

/**
 * Schedules the next recurring aircraft event method and increments the number generated of the source block.
 */
void SourceBlock::ScheduleNextRecurringAircraftEM(Distribution* recurringIAT, RepairJob* repairJob) {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		//Scheduling recurring aircrafts
		cout << "Recurring Aircraft has arrived, ";
		cout << "Scheduling next Recurring Repair Job: " << repairJob->GetName() << endl;
		SimExec::ScheduleEventAtRecurring(_aircraft->GetAircraftPriority(), new ScheduleNextRecurringAircraftEA(this, recurringIAT, repairJob), recurringIAT->GetRV(), "ScheduleNextRecurringAircraftEA");
		//No longer doing this!
		//cout << "Scheduling next Random Aircraft arrival" << endl;
		//SimExec::ScheduleEventAt(_aircraft->GetAircraftPriority(), new ScheduleNextRandomAircraftEA(this), _interarrivalTimeRND->GetRV(), "ScheduleNextRandomAircraftEA");
		//cout << "Departing new Aircraft" << endl;
		//Depart(_aircraft->New());
		_numberGenerated++;
	}
}

/**
 * Decleration of the virtual function.
 */
void SourceBlock::Execute(Aircraft* aircraft) {
	//This is just a declaration of the virtual function doesn't do anything
}