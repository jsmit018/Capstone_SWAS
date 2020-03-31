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

class SourceBlock::ScheduleNextRecurringAircraftEA : public EventAction {
public:
	ScheduleNextRecurringAircraftEA(SourceBlock* source, Distribution* recurringiAT, RepairJob* repairJob) {
		_source = source;
		_repairJob = repairJob;
		_recurringIAT = recurringiAT;
	}
	void Execute() {
		_source->ScheduleNextRecurringAircraftEM(_recurringIAT, _repairJob);
	}
private:
	SourceBlock* _source;
	RepairJob* _repairJob;
	Distribution* _recurringIAT;
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


//Jordan: Need to take map of recurring IAT and schedule, instead of vector
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
//Jordan : This needs to take the map and search prexisting maps as previously discussed
//There's no reason to pass yet another map (the repair job map you've included) when the info is already avail
//in other words, the recurringIAT <string, distribution*>map it is receving has a string of the repair job name attached to a recurring 
//distribution instead of passing another map that doesn't exist, take the key of the recurringIat map being passed and search the aircraft's 
//myrepairjob map for the same name. if the repair job string from our recurringIAT map is found in the myrepairjobs map, you've found
//the recurring IAt's repair jobs and you'll schedule an arrival of this aircraft with a using the distribution you already were passed in the map.

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

SourceBlock::SourceBlock(string aircraftType, Aircraft* aircraft, string name,
	int numCalEventsToSched, CalendarObj* calobj) : Task(name)
{
	cout << "Scheduling list of Calendar Events" << endl;
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
		cout << "Calendar Scheduled aircraft has arrived, ";
		cout << "Departing new Aircraft" << endl;
		//Depart(_aircraft->New());
		//_aircraft->New()->CopyMyJobList(_aircraft->GetAircraftType());
		_numberGenerated++;
	}
}

void SourceBlock::ScheduleNextUnplannedAircraftEM() {
	if (_numberGenerated != _numberOfAircraftToGenerate) {
		cout << "Unpanned Aircraft has arrived, ";
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

void SourceBlock::Execute(Aircraft* aircraft) {
	//This is just a declaration of the virtual function doesn't do anything
}