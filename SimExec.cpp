#include "SimExec.h"
#include "Resource.h"
#include "Parts.h"
#include "InputInterface.h"

/*SimExec::SimExec() : SimObj(){
}*/

/**
* Calls the function RunSimulation()
*/
void PlayButton()
{
	SimExec::RunSimulation();
}

/**
* Calls the function RunSimulation(Time month, Time day, int year)
*/
void PlayButton(Time month, Time day, int year)
{
	SimExec::RunSimulation(month, day, year);
}

/**
* Struct for the simulation executive
*/
struct SimExec::Event {
	/**
	* Sets the variables to default values
	*/
	Event(EventAction* ea, Time timeMonth, Time timeDay, Time timeOfDay, int priority, int year, string eaName) {
		_ea = ea;
		_nextEvent = 0;
		_timeMonth = timeMonth;
		_timeDay = timeDay;
		_year = year;
		_timeOfDay = timeOfDay;
		_priority = priority;
		_eventActionName = eaName;
	}

	/**
	* Prints the event action name, when it will occur, and its priority
	*/
	void PrintEvent() {
		cout << "_________________________________________________________________________________" << endl;
		cout << "Event Action: " << _eventActionName << endl;
		if (_timeOfDay >= 10)
			cout << "Will occur on: " << _timeMonth + 1 << " " << _timeDay + 1 << " at " << _timeOfDay << "00 in " << _year << endl;
		else
			cout << "Will occur on: " << _timeMonth + 1 << " " << _timeDay + 1 << " at 0" << _timeOfDay << "00 in " << _year << endl;
		cout << "Priority: " << _priority << endl;
		cout << "_________________________________________________________________________________" << endl;
	}
	Time _timeOfDay;
	Time _timeMonth;
	Time _timeDay;
	EventAction* _ea;
	Event* _nextEvent;
	string _eventActionName;
	int _year;
	int _priority;
};

/**
* Struct for a conditional event
*/
struct SimExec::CondEvent {
	/**
	* Sets the variables to default values
	*/
	CondEvent(int priority, CondEventAction* cea) {
		_priority = priority;
		_cea = cea;
		_nextCondEvent = 0;
	}

	int _priority;
	CondEventAction* _cea;
	CondEvent* _nextCondEvent;
};

/**
* Class for a conditional event set
*/
class SimExec::CondEventSet {
public:
	/**
	* Sets the variable to 0
	*/
	CondEventSet() {
		_condSet = 0;
	}

	/**
	* Adds a new conditional event to the event set based on priority
	*/
	void AddConditionalEvent(int priority, CondEventAction* cea) {
		CondEvent* c = new CondEvent(priority, cea);
		if (_condSet == 0) {
			_condSet = c;
		}
		else if (_condSet->_priority > c->_priority) {
			c->_nextCondEvent = _condSet;
			_condSet = c;
		}
		else {
			CondEvent* curr = _condSet;
			while ((curr != 0) ? (c->_priority <= curr->_priority) : false) {
				curr = curr->_nextCondEvent;
			}
			if (curr->_nextCondEvent == 0) {
				curr->_nextCondEvent = c;
			}
			else {
				c->_nextCondEvent = curr->_nextCondEvent;
				curr->_nextCondEvent = c;
			}
		}
	}

	/**
	* Executes a conditional event as long as there are resources and parts available, and returns true; else, returns false
	*/
	bool CheckConditionalEvents(Resource* resource, Parts* parts) {
		CondEvent* curr = _condSet;
		while (curr != 0) {
			if (curr->_cea->Condition(resource, parts)) {
				curr->_cea->Execute();
				return true;
			}
			curr = curr->_nextCondEvent;
		}
		return false;
	}

private:
	CondEvent* _condSet;
};

/**
* Class for the event set
*/
class SimExec::EventSet {
public:
	/**
	* Default constructor for the event set
	*/
	EventSet() {

	}

	/**
	* Initializes the event set based on the number of bins and days
	*/
	void InitEventSet(int numBins, int* days) {
		cout << "Initializing Event Set" << endl;
		cout << "Setting the initial number of events to 0" << endl;
		_numEvents = 0;
		cout << "Setting the number of Bins" << endl;
		_numBins = numBins;
		cout << "Initialzing the Month to January and the Day to the 1st" << endl;
		_baseX = 0;
		_baseY = 0;
		cout << "Initializing the year to the current year" << endl;
		_year = 2020;
		cout << "Initializing the extra bin for the Calendar Queue" << endl;
		_overflow = 12;
		cout << "Creating the Event List" << endl;
		_eventSet = new Event * *[numBins + 1];
		cout << "Creating the End Of the Month dates array" << endl;
		_endOfMonth = new int[numBins];
		cout << "Initializing the size of each month" << endl;
		for (int i = 0; i < numBins + 1; ++i) {
			if (_year % 4 == 0 && i == February) {
				cout << "February is a leap year in " << _year << endl;
				_eventSet[i] = new Event *[days[i] + 1];
			}
			else if (i == 12)
				_eventSet[i] = new Event *[1];
			else
				_eventSet[i] = new Event *[days[i]];

		}
		cout << "Initializing the linked list of each Month & Day" << endl;
		for (int i = 0; i < numBins + 1; ++i) {
			if (i == 12) {
				cout << "Overflow Initialzed" << endl;
				_eventSet[i][0] = 0;
			}
			for (int j = 0; j < days[i]; ++j) {
				cout << ConvertMonth(i) << " " << j + 1 << " initialized" << endl;
				if (i == February && j == 27 && _year % 4 == 0) {
					cout << ConvertMonth(i) << " " << j + 2 << " initialized" << endl;
					_eventSet[i][j + 1] = 0;
				}
				_eventSet[i][j] = 0;
				if (i == 12 && j == 1) {
					cout << "Overflow Initialzed breaking initialization loop" << endl;
					break;
				}
			}
		}
		cout << "Setting the end of month dates" << endl;
		for (int i = 0; i < numBins; ++i) {
			_endOfMonth[i] = days[i] - 1;
		}
	}

	/**
	* Adds a calendar event to the event list
	*/
	void AddEventCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName) {
		cout << "***********************************************************************" << endl;
		cout << "Adding Event to the Event List" << endl;
		_numEvents++;
		cout << "Number of Events increased to " << _numEvents << endl;
		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year, eaName);
		e->PrintEvent();
		int binX;
		int binY;
		cout << "Hashing year to see the appopriate place to add the event" << endl;
		if (e->_year > _year) {
			cout << "Event isn't in this calendar year, adding it to the overflow bin" << endl;
			binX = _overflow;
			binY = 0;
		}
		else {
			cout << "Setting the bins to the appopriate month & day" << endl;
			binX = Month;
			binY = Day;
		}
		cout << "Checking to see where to place the new event" << endl;
		//cout << "Checking to see if Month and Day tuple is 0" << endl;
		//cout << "If tuple isn't 0 then we check to see if the new event has a higher priority or an earlier time than the head" << endl;
		//cout << "If either of those conditions aren't met then we cycle through the list to find the appopriate	place to put the new event" << endl;
		if (_eventSet[binX][binY] == 0) {
			cout << "Tuple is 0, adding the event to the head of the list" << endl;
			_eventSet[binX][binY] = e;
		}
		else if (timeOfDay < _eventSet[binX][binY]->_timeOfDay) {
			cout << "New event has an earlier time than the head updating the list" << endl;
			e->_nextEvent = _eventSet[binX][binY];
			_eventSet[binX][binY] = e;
		}
		else {
			Event* curr = _eventSet[binX][binY];
			cout << "Searching the list on where to place the event based on time and priority" << endl;
			while ((curr->_nextEvent != 0) ? (e->_timeOfDay >= curr->_timeOfDay && !(e->_timeOfDay < curr->_nextEvent->_timeOfDay)) : false) {
				if (e->_timeOfDay == curr->_nextEvent->_timeOfDay) {
					if (e->_priority < curr->_nextEvent->_priority) {
						break;
					}
					else
						curr = curr->_nextEvent;
				}
				else
					curr = curr->_nextEvent;
			}
			if (curr->_nextEvent == 0 && e->_timeOfDay >= curr->_timeOfDay) {
				curr->_nextEvent = e;
			}
			else {
				e->_nextEvent = curr->_nextEvent;
				curr->_nextEvent = e;
			}
		}
		if (timeOfDay >= 10)
			cout << "Added Event to the Event Set on " << ConvertMonth(Month) << " " << Day + 1 << " at " << timeOfDay << "00 in " << e->_year << endl;
		else
			cout << "Added Event to the Event Set on " << ConvertMonth(Month) << " " << Day + 1 << " at 0" << timeOfDay << "00 in " << e->_year << endl;
		cout << "***********************************************************************" << endl;
	}

	/**
	* Adds a recurring event to the event list
	*/
	void AddEventRecurring(int priority, EventAction* ea, double distributionValue, int recurring, string eaName) {
		cout << "***********************************************************************" << endl;
		Time Month = 0.0;
		Time Day = 0.0;
		Time timeOfDay = 0.0;
		int year = _year;
		cout << "Adding Event to the Event List" << endl;
		_numEvents++;
		cout << "Number of Events increased to " << _numEvents << endl;
		cout << "Converting Distribution to Appropriate Time" << endl;
		TimeConverter::ConvertDistributionToMonthDay(Month, Day, timeOfDay, year, distributionValue, _baseX, _baseY, _endOfMonth, recurring, _simulationTime._timeOfDay);
		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year, eaName);
		e->PrintEvent();
		int binX;
		int binY;
		cout << "Hashing year to see the appopriate place to add the event" << endl;
		if (e->_year > _year) {
			cout << "Event isn't in this calendar year, adding it to the overflow bin" << endl;
			binX = _overflow;
			binY = 0;
		}
		else {
			cout << "Setting the bins to the appopriate month & day" << endl;
			binX = Month;
			binY = Day;
		}
		/*cout << "Checking to see if Month and Day tuple is 0" << endl;
		cout << "If tuple isn't 0 then we check to see if the new event has a higher priority or an earlier time than the head" << endl;
		cout << "If either of those conditions aren't met then we cycle through the list to find the appopriate	place to put the new event" << endl;*/
		if (_eventSet[binX][binY] == 0) {
			cout << "Tuple is 0, adding the event to the head of the list" << endl;
			_eventSet[binX][binY] = e;
		}
		else if (timeOfDay < _eventSet[binX][binY]->_timeOfDay) {
			cout << "New event has an earlier time than the head updating the list" << endl;
			e->_nextEvent = _eventSet[binX][binY];
			_eventSet[binX][binY] = e;
		}
		else {
			Event* curr = _eventSet[binX][binY];
			cout << "Searching the list on where to place the event based on time and priority" << endl;
			while ((curr->_nextEvent != 0) ? (e->_timeOfDay >= curr->_timeOfDay && !(e->_timeOfDay < curr->_nextEvent->_timeOfDay)) : false) {
				if (e->_timeOfDay == curr->_nextEvent->_timeOfDay) {
					if (e->_priority < curr->_nextEvent->_priority) {
						break;
					}
					else
						curr = curr->_nextEvent;
				}
				else
					curr = curr->_nextEvent;
			}
			if (curr->_nextEvent == 0 && e->_timeOfDay >= curr->_timeOfDay) {
				curr->_nextEvent = e;
			}
			else {
				e->_nextEvent = curr->_nextEvent;
				curr->_nextEvent = e;
			}
		}
		if (timeOfDay >= 10)
			cout << "Added Event to the Event Set on " << ConvertMonth(Month) << " " << Day + 1 << " at " << timeOfDay << "00 in " << e->_year << endl;
		else
			cout << "Added Event to the Event Set on " << ConvertMonth(Month) << " " << Day + 1 << " at 0" << timeOfDay << "00 in " << e->_year << endl;
		cout << "***********************************************************************" << endl;
	}

	/**
	* Adds an event to the event list
	*/
	void AddEvent(int priority, EventAction* ea, double distributionValue, string eaName) {
		cout << "***********************************************************************" << endl;
		Time Month = 0.0;
		Time Day = 0.0;
		Time timeOfDay = 0.0;
		int year = _year;
		cout << "Adding Event to the Event List" << endl;
		_numEvents++;
		cout << "Number of Events increased to " << _numEvents << endl;
		cout << "Converting Distribution to Appropriate Time" << endl;
		TimeConverter::ConvertDistributionToMonthDay(Month, Day, timeOfDay, year, distributionValue, _baseX, _baseY, _endOfMonth);
		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year, eaName);
		e->PrintEvent();
		int binX;
		int binY;
		cout << "Hashing year to see the appopriate place to add the event" << endl;
		if (e->_year > _year) {
			cout << "Event isn't in this calendar year, adding it to the overflow bin" << endl;
			binX = _overflow;
			binY = 0;
		}
		else {
			cout << "Setting the bins to the appopriate month & day" << endl;
			binX = Month;
			binY = Day;
		}
		/*cout << "Checking to see if Month and Day tuple is 0" << endl;
		cout << "If tuple isn't 0 then we check to see if the new event has a higher priority or an earlier time than the head" << endl;
		cout << "If either of those conditions aren't met then we cycle through the list to find the appopriate	place to put the new event" << endl;*/
		if (_eventSet[binX][binY] == 0) {
			cout << "Tuple is 0, adding the event to the head of the list" << endl;
			_eventSet[binX][binY] = e;
		}
		else if (timeOfDay < _eventSet[binX][binY]->_timeOfDay) {
			cout << "New event has an earlier time than the head updating the list" << endl;
			e->_nextEvent = _eventSet[binX][binY];
			_eventSet[binX][binY] = e;
		}
		else {
			Event* curr = _eventSet[binX][binY];
			cout << "Searching the list on where to place the event based on time and priority" << endl;
			while ((curr->_nextEvent != 0) ? (e->_timeOfDay >= curr->_timeOfDay && !(e->_timeOfDay < curr->_nextEvent->_timeOfDay)) : false) {
				if (e->_timeOfDay == curr->_nextEvent->_timeOfDay) {
					if (e->_priority < curr->_nextEvent->_priority) {
						break;
					}
					else
						curr = curr->_nextEvent;
				}
				else
					curr = curr->_nextEvent;
			}
			if (curr->_nextEvent == 0 && e->_timeOfDay >= curr->_timeOfDay) {
				curr->_nextEvent = e;
			}
			else {
				e->_nextEvent = curr->_nextEvent;
				curr->_nextEvent = e;
			}
		}
		if (timeOfDay >= 10)
			cout << "Added Event to the Event Set on " << ConvertMonth(Month) << " " << Day + 1 << " at " << timeOfDay << "00 in " << e->_year << endl;
		else
			cout << "Added Event to the Event Set on " << ConvertMonth(Month) << " " << Day + 1 << " at 0" << timeOfDay << "00 in " << e->_year << endl;
		cout << "***********************************************************************" << endl;
	}

	/**
	* Returns the time of day
	*/
	Time GetTimeOfDay() {
		cout << "Returning time of day" << endl;
		return _eventSet[_baseX][_baseY]->_timeOfDay;
	}

	/**
	* Returns the month
	*/
	Time GetMonth() {
		cout << "Returning Month" << endl;
		return _eventSet[_baseX][_baseY]->_timeMonth;
	}

	/**
	* Returns the day of the month
	*/
	Time GetDay() {
		cout << "Returning Day of the Month" << endl;
		return _eventSet[_baseX][_baseY]->_timeDay;
	}

	/**
	* Returns the year
	*/
	int GetYear() {
		cout << "Returning Year" << endl;
		return _eventSet[_baseX][_baseY]->_year;
	}

	/**
	* Converts a month into a string
	*/
	string ConvertMonth(Time month) {
		cout << "Converting month into a string" << endl;
		switch ((int)month) {
		case 0:
			return "January";
			break;
		case 1:
			return "February";
			break;
		case 2:
			return "March";
			break;
		case 3:
			return "April";
			break;
		case 4:
			return "May";
			break;
		case 5:
			return "June";
			break;
		case 6:
			return "July";
			break;
		case 7:
			return "August";
			break;
		case 8:
			return "September";
			break;
		case 9:
			return "October";
			break;
		case 10:
			return "November";
			break;
		case 11:
			return "December";
			break;
		default: cerr << "Invalid Month" << endl;
			return NULL;
		}
	}

	/**
	* Gets the event action of each event in the event list to execute
	*/
	EventAction* GetEventAction() {
		cout << "Getting Event Action" << endl;
		if (_numEvents > 0) {
			while (_eventSet[_baseX][_baseY] == 0) {
				if (_baseY == _endOfMonth[_baseX]) {
					AdvanceMonth();
				}
				else
					AdvanceDay();
			}
			Event* next = _eventSet[_baseX][_baseY];
			cout << "Executing Event on " << ConvertMonth(GetMonth()) << " " << GetDay() << " at " << GetTimeOfDay();
			_eventSet[_baseX][_baseY] = next->_nextEvent;
			EventAction* ea = next->_ea;
			delete next;
			_numEvents--;
			return ea;
		}
		else {
			return 0;
		}
	}

	/**
	* Checks to see if the number of events is greater than 0
	*/
	bool HasEvent() {
		cout << "Checking to see if number of events is greater than 0" << endl;
		return _numEvents > 0;
	}

	/**
	* Prints the events based on month and year
	*/
	void PrintCalendar() {
		for (int i = 0; i < _numBins; ++i) {
			for (int j = 0; j < _endOfMonth[i] + 1; ++j) {
				Event* e = _eventSet[i][j];
				int count = 0;
				while (e != 0) {
					e = e->_nextEvent;
					count++;
				}
				cout << count << " ";
				if (i == 1 && j == 27 && _year % 4 == 0) {
					Event* eLeap = _eventSet[i][j + 1];
					while (eLeap != 0) {
						eLeap = eLeap->_nextEvent;
						count++;
					}
					cout << count << " ";
				}
			}
			cout << endl;
		}
	}
	//
private:
	int _numBins, _baseX, _overflow, _baseY;
	int _year;
	int _numEvents;
	int* _endOfMonth;
	Event*** _eventSet;
	//
	//	//Will add an if statement on December 31 to increment the year by 1.
	/**
	* Advances the month and updates the overflow bin
	*/
	void AdvanceMonth() {
		cout << "Advancing Month, and updating overflow bin" << endl;
		if (_baseX == December - 1 && _baseY == 30) {
			_year++;
			_baseX = 0;
			_baseY = 0;
		}
		else {
			_baseX++;
			_baseY = 0;
		}

		int previousBase;
		if (_baseX == 0)
			previousBase = December;
		else
			previousBase = _baseX - 1;

		Event* currEvent = _eventSet[_overflow][_baseY];
		int eventDay = 0;
		while (currEvent != 0) {
			if (currEvent->_timeMonth == previousBase)
				eventDay = currEvent->_timeDay;
			if (_eventSet[previousBase][eventDay] == 0)
				_eventSet[previousBase][eventDay] = currEvent;
			else {
				/*Sort it into the list of events*/
				if (currEvent->_timeOfDay < _eventSet[previousBase][eventDay]->_timeOfDay) {
					currEvent->_nextEvent = _eventSet[previousBase][eventDay];
					_eventSet[previousBase][eventDay] = currEvent;
				}
				else {
					Event* curr = _eventSet[previousBase][eventDay];
					while ((curr->_nextEvent != 0) ? (currEvent->_timeOfDay >= curr->_timeOfDay && !(currEvent->_timeOfDay < curr->_nextEvent->_timeOfDay)) : false) {
						if (currEvent->_timeOfDay == curr->_nextEvent->_timeOfDay) {
							if (currEvent->_priority < curr->_nextEvent->_priority) {
								break;
							}
						}
						else
							curr = curr->_nextEvent;
					}
					if (curr->_nextEvent == 0 && currEvent->_timeOfDay >= curr->_timeOfDay) {
						curr->_nextEvent = currEvent;
					}
					else {
						currEvent->_nextEvent = curr->_nextEvent;
						curr->_nextEvent = currEvent;
					}
				}
			}
			currEvent = currEvent->_nextEvent;
		}
	}

	/**
	* Advances the day
	*/
	void AdvanceDay() {
		cout << "Advancing Day" << endl;
		_baseY++;
	}
};

SimExec::EventSet SimExec::_eventSet;
SimExec::CondEventSet SimExec::_conditionalSet;
SimulationTime SimExec::_simulationTime;
InputReader SimExec::_inputReader;
bool SimExec::_simulationFlag;

/**
* Sets the simulation time to default values
*/
void SimExec::InitializeSimulation(int numBins, int* days) {
	cout << "Setting Simulation time to 0" << endl;
	_simulationTime._timeOfDay = 0;
	_simulationTime._month = 0;
	_simulationTime._day = 0;
	_simulationTime._year = 2020;
	_simulationFlag = true;
	_eventSet.InitEventSet(numBins, days);
}

/**
* Returns the simulation time
*/
SimulationTime SimExec::GetSimulationTime() {
	cout << "Returning Simulation Time" << endl;
	return _simulationTime;
}

/**
* Returns the input reader
*/
InputReader SimExec::GetInputReader()
{
	return _inputReader;
}

/**
* Sets the input reader
*/
void SimExec::SetInputReader(InputReader inputReader)
{
	_inputReader = inputReader;
}

/**
* Schedules to add an event to the event set
*/
void SimExec::ScheduleEventAt(int priority, EventAction* ea, double distributionValue, string eaName) {
	cout << "Scheduling Event" << endl;
	_eventSet.AddEvent(priority, ea, distributionValue, eaName);
}

/**
* Schedules to add a calendar event to the event set
*/
void SimExec::ScheduleEventAtCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName) {
	cout << "Scheduling Calendar Event" << endl;
	_eventSet.AddEventCalendar(Month, Day, timeOfDay, year, priority, ea, eaName);
}

/**
* Schedules to add a recurring event to the event set
*/
void SimExec::ScheduleEventAtRecurring(int priority, EventAction* ea, double distributionValue, string eaName, int recurring)
{
	cout << "Scheduling Recurring Event" << endl;
	_eventSet.AddEventRecurring(priority, ea, distributionValue, recurring, eaName);
}

/**
* Schedules to add a conditional event to the event set
*/
void SimExec::ScheduleConditionalEvent(int priority, CondEventAction* cea)
{
	cout << "Scheduling Conditional Event";
	_conditionalSet.AddConditionalEvent(priority, cea);
}

/**
* Returns a converted month from the event set
*/
string SimExec::ConvertDate(Time month)
{
	return _eventSet.ConvertMonth(month);
}

/**
* Checks the resources and parts of a conditional event while there is an event in the event set
*/
void SimExec::CheckConditionalEvents(Resource* resource, Parts* parts)
{
	while (_conditionalSet.CheckConditionalEvents(resource, parts));
}

/**
* Prints the event set
*/
void SimExec::PrintEventSet()
{
	_eventSet.PrintCalendar();
}

/**
* Executes events from the event set if the simulation flag is true and there is an event in the event set
*/
//void SimExec::RunSimulation() {
int SimExec::RunSimulation() {
	//cout << "Running Simulation" << endl;
	cout << "Executing Event" << endl;
	//while (_eventSet.HasEvent() && _simulationFlag == true) {
	//Commented out the while loop for tyler going to try moving the loop into unity.
	if (_simulationFlag == true && _eventSet.HasEvent()) {
		_simulationTime._timeOfDay = _eventSet.GetTimeOfDay();
		_simulationTime._month = _eventSet.GetMonth();
		_simulationTime._day = _eventSet.GetDay();
		_simulationTime._year = _eventSet.GetYear();
		EventAction* ea = _eventSet.GetEventAction();
		if (ea == 0) {
			FlipSimulationFlag();
			return 3;
		}
		ea->Execute();
		delete ea;
		/*if (_eventSet.HasEvent() ? (_eventSet.GetTimeOfDay() != _simulationTime._timeOfDay || _eventSet.GetDay() != _simulationTime._day
			|| _eventSet.GetYear() != _simulationTime._year || _eventSet.GetMonth() != _simulationTime._month) : true)
			while (_conditionalSet.CheckConditionalEvents());*/
		if (!_eventSet.HasEvent())
			FlipSimulationFlag();
		return 0;
	}
	else if (_simulationFlag == false) {
		cout << "Simulation Terminating" << endl;
		if (_simulationTime._timeOfDay >= 10) {
			cout << "Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
				<< " at " << _simulationTime._timeOfDay << "00 in " << _simulationTime._year << endl;
		}
		else if (_simulationTime._timeOfDay) {
			cout << "Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
				<< " at 0" << _simulationTime._timeOfDay << "00 in " << _simulationTime._year << endl;
		}
		return 3;
	}
}

/**
* Executes events from the event set if the simulation flag is true and there is an event in the event set
*/
//void SimExec::RunSimulation(Time month, Time day, Time timeOfDay, int year) {
int SimExec::RunSimulation(Time month, Time day, int year) {
	//cout << "Running Simulation" << endl;
	cout << "Executing Event" << endl;
	//while (_eventSet.HasEvent() && _simulationFlag == true) {
	if (_eventSet.HasEvent() && _simulationFlag == true) {
		if (_simulationTime._month >= (int)month && _simulationTime._day >= (int)day && _simulationTime._timeOfDay >= 0
			&& _simulationTime._year >= year) {
			FlipSimulationFlag();
			return 3;
			//break;
		}
		else {
			_simulationTime._timeOfDay = _eventSet.GetTimeOfDay();
			_simulationTime._month = _eventSet.GetMonth();
			_simulationTime._day = _eventSet.GetDay();
			_simulationTime._year = _eventSet.GetYear();
			EventAction* ea = _eventSet.GetEventAction();
			if (ea == 0) {
				FlipSimulationFlag();
				return 3;
			}
			ea->Execute();
			delete ea;
			/*if (_eventSet.HasEvent() ? (_eventSet.GetTimeOfDay() != _simulationTime._timeOfDay || _eventSet.GetDay() != _simulationTime._day
				|| _eventSet.GetYear() != _simulationTime._year || _eventSet.GetMonth() != _simulationTime._month) : true)
				while (_conditioinalSet.CheckConditionalEvents());*/
			return 0;
		}
	}
	else if (_simulationFlag == false) {
		if (_simulationTime._timeOfDay >= 10) {
			cout << "Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
				<< " at " << _simulationTime._timeOfDay << "00 in " << _simulationTime._year << endl;
		}
		else {
			cout << "Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
				<< " at 0" << _simulationTime._timeOfDay << "00 in " << _simulationTime._year << endl;
		}
		return 3;
	}
}
/**
* If the simulation flag is true, sets it to false; else, sets it to true
*/
void SimExec::FlipSimulationFlag()
{
	if (_simulationFlag == true)
		_simulationFlag = false;
	else
		_simulationFlag = true;
}

/**
* Returns the simulation flag
*/
bool SimExec::GetSimulationFlag()
{
	return _simulationFlag;
}

/**
* Calls the function FlipSimulationFlag()
*/
void StopSimulation()
{
	SimExec::FlipSimulationFlag();
}