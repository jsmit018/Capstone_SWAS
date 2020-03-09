#include "SimExec.h"


/*SimExec::SimExec() : SimObj(){

}*/

struct SimExec::Event {
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

class SimExec::EventSet {
public:
	EventSet() {

	}

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
				_eventSet[i] = new Event * [days[i] + 1];
			}
			else if (i == 12)
				_eventSet[i] = new Event * [1];
			else
				_eventSet[i] = new Event * [days[i]];

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
					if (e->_priority > curr->_nextEvent->_priority) {
						break;
					}
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
		TimeConverter::ConvertDistributionToMonthDay(Month, Day, timeOfDay, year, distributionValue, _baseX, _baseY, _endOfMonth, recurring, _simulationTime);
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
		cout << "Checking to see if Month and Day tuple is 0" << endl;
		cout << "If tuple isn't 0 then we check to see if the new event has a higher priority or an earlier time than the head" << endl;
		cout << "If either of those conditions aren't met then we cycle through the list to find the appopriate	place to put the new event" << endl;
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
			while ((curr->_nextEvent != 0) ? (e->_timeOfDay >= curr->_timeOfDay) : false) {
				if (e->_timeOfDay == curr->_nextEvent->_timeOfDay) {
					if (e->_priority > curr->_nextEvent->_priority) {
						break;
					}
				}
				else
					curr = curr->_nextEvent;
			}
			if (curr->_nextEvent == 0) {
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
		cout << "Checking to see if Month and Day tuple is 0" << endl;
		cout << "If tuple isn't 0 then we check to see if the new event has a higher priority or an earlier time than the head" << endl;
		cout << "If either of those conditions aren't met then we cycle through the list to find the appopriate	place to put the new event" << endl;
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
			while ((curr->_nextEvent != 0) ? (e->_timeOfDay >= curr->_timeOfDay) : false) {
				if (e->_timeOfDay == curr->_nextEvent->_timeOfDay) {
					if (e->_priority > curr->_nextEvent->_priority) {
						break;
					}
				}
				else
					curr = curr->_nextEvent;
			}
			if (curr->_nextEvent == 0) {
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

	Time GetTimeOfDay() {
		cout << "Returning time of day" << endl;
		return _eventSet[_baseX][_baseY]->_timeOfDay;
	}

	Time GetMonth() {
		cout << "Returning Month" << endl;
		return _eventSet[_baseX][_baseY]->_timeMonth;
	}

	Time GetDay() {
		cout << "returning Day of the Month" << endl;
		return _eventSet[_baseX][_baseY]->_timeDay;
	}

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
			_simulationTime = GetTimeOfDay();
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

	bool HasEvent() {
		cout << "Checking to see if number of events is greater than 0" << endl;
		return _numEvents > 0;
	}

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
					while ((curr->_nextEvent != 0) ? (curr->_timeOfDay
						>= currEvent->_timeOfDay) : false) {
						curr = curr->_nextEvent;
					}
					if (curr->_nextEvent == 0) {
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

	void AdvanceDay() {
		cout << "Advancing Day" << endl;
		_baseY++;
	}
};

SimExec::EventSet SimExec::_eventSet;
Time SimExec::_simulationTime = 0.0;

void SimExec::InitializeSimulation(int numBins, int* days) {
	cout << "Setting Simulation time to 0" << endl;
	_simulationTime = 0.0;
	_eventSet.InitEventSet(numBins, days);
}

Time SimExec::GetSimulationTime() {
	cout << "Returning Simulation Time" << endl;
	return _simulationTime;
}

void SimExec::ScheduleEventAt(int priority, EventAction* ea, double distributionValue, string eaName) {
	cout << "Scheduling Event" << endl;
	_eventSet.AddEvent(priority, ea, distributionValue, eaName);
}

void SimExec::ScheduleEventAtCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName) {
	cout << "Scheduling Calendar Event" << endl;
	_eventSet.AddEventCalendar(Month, Day, timeOfDay, year, priority, ea, eaName);
}

void SimExec::ScheduleEventAtRecurring(int priority, EventAction* ea, double distributionValue, string eaName, int recurring)
{
	cout << "Scheduling Recurring Event" << endl;
	_eventSet.AddEventRecurring(priority, ea, distributionValue, recurring, eaName);
}

void SimExec::PrintEventSet()
{
	_eventSet.PrintCalendar();
}

void SimExec::RunSimulation() {
	cout << "Running Simulation" << endl;
	while (_eventSet.HasEvent()) {
		//_simulationTime = _eventSet.GetTime();
		EventAction* ea = _eventSet.GetEventAction();
		ea->Execute();
		delete ea;
	}
	cout << "Simulation Terminating" << endl;
}

void SimExec::RunSimulation(Time time) {
	cout << "Running Simulation" << endl;
	while (_eventSet.HasEvent() && _simulationTime <= time) {
		//_simulationTime = _eventSet.GetTime();
		EventAction* ea = _eventSet.GetEventAction();
		ea->Execute();
		delete ea;
	}
	cout << "Simulation Terminated at time " << time << endl;
}
