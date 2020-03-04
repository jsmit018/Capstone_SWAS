#include "SimExec.h"


/*SimExec::SimExec() : SimObj(){

}*/

struct SimExec::Event {
	Event(EventAction* ea, Time timeMonth, Time timeDay, Time timeOfDay, int priority, int year) {
		_ea = ea;
		_nextEvent = 0;
		_timeMonth = timeMonth;
		_timeDay = timeDay;
		_year = year;
		_timeOfDay = timeOfDay;
		_priority = priority;
	}
	Time _timeOfDay;
	Time _timeMonth;
	Time _timeDay;
	EventAction* _ea;
	Event* _nextEvent;
	int _year;
	int _priority;
};

class SimExec::EventSet {
public:
	EventSet() {

	}

	void InitEventSet(int numBins, Time timeRange, int* days) {
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
		//_overflow = 13; // Depending on whether or not we are using the 0th index
		_overflow = 12;
		cout << "Creating the Event List" << endl;
		_eventSet = new Event **[numBins + 1];
		cout << "Creating the End Of the Month dates array" << endl;
		_endOfMonth = new int[numBins];
		cout << "Initializing the size of each month" << endl;
		for (int i = 0; i < numBins; ++i) {
			if (_year % 4 == 0 && i == February) {
				cout << "February is a leap year in " << _year << endl;
				_eventSet[i] = new Event *[days[i] + 1];
			}
			else
				_eventSet[i] = new Event *[days[i]];
		}
		cout << "Initializing the linked list of each Month & Day" << endl;
		for (int i = 0; i < numBins + 1; ++i) {
			for (int j = 0; j < days[i]; ++j) {
				cout << ConvertMonth(i) << " " << j + 1 << "initialized" << endl;
				_eventSet[i][j] = 0;
			}
		}
		cout << "Setting the end of month dates" << endl;
		for (int i = 0; i < numBins; ++i) {
			_endOfMonth[i] = days[i] - 1;
		}
	}

	void AddEventCalendar(Time Month, Time Day, int year, int priority, EventAction* ea) {
		cout << "Adding Event to the Event List" << endl;
		_numEvents++;
		cout << "Number of Events increased to " << _numEvents << endl;
		cout << "Converting Distribution to Appropriate Time" << endl;
		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year);
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
		cout << "If tuple isn't 0 then we check to see if the new event has a higher priority\n or an earlier time
			than the head" << endl;
		cout << "If either of those conditions aren't met then we cycle through the list to find the appopriate\n
			place to put the new event << endl;
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
		cout << "Added Event to the Event Set on " << ConvertMonth(Month) << " " << Day << " " << timeOfDay << endl;
	} 
	
	void AddEvent(int priority, EventAction* ea, double distributionValue) {
		Time Month = 0.0;
		Time Day = 0.0;
		Time timeOfDay = 0.0;
		int year = _year;
		cout << "Adding Event to the Event List" << endl;
		_numEvents++;
		cout << "Number of Events increased to " << _numEvents << endl;
		cout << "Converting Distribution to Appropriate Time" << endl;
		TimeConverter::ConvertDistributiontoMonthDay(Month, Day, timeOfDay, year, distributionValue, _baseX, 
							     _baseY, _endOfMonth);
		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year);
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
		cout << "Added Event to the Event Set on " << ConvertMonth(Month) << " " << Day << " " << timeOfDay << endl;
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

void SimExec::InitializeSimulation(int numBins, Time timeRange, int* days) {
	cout << "Setting Simulation time to 0" << endl;
	_simulationTime = 0.0;
	_eventSet.InitEventSet(numBins, timeRange, days);
}

Time SimExec::GetSimulationTime() {
	cout << "Returning Simulation Time" << endl;
	return _simulationTime;
}

void SimExec::ScheduleEventAt(int priority, EventAction* ea, double distributionValue) {
	cout << "Scheduling Event" << endl;
	_eventSet.AddEvent(priority, ea, distributionValue);
}

void SimExec::ScheduleEventAtCalendar(Time Month, Time Day, int year, int priority, EventAction* ea) {
//	_eventSet.AddEvent(Month, Day, year, priority, ea);
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
