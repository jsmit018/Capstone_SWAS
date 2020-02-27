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
		_numEvents = 0;
		_numBins = numBins;
		_baseX = 0;
		_baseY = 0;
		_year = 2020;
		_overflow = 13;
		_eventSet = new Event ** [numBins + 1];
		_endOfMonth = new int[numBins];
		for (int i = 0; i < numBins; ++i){
			if (_year % 4 == 0 && i == February) {
				cout << "February is a leap year in " << _year << endl;
				_eventSet[i] = new Event * [days[i] + 1];
			}
			else
				_eventSet[i] = new Event*[days[i]];
		}
		for (int i = 0; i < numBins + 1; ++i) {
			for (int j = 0; j < days[i]; ++j){
				_eventSet[i][j] = 0;
			}
		}
		for (int i = 0; i < numBins; ++i) {
			_endOfMonth[i] = days[i];
		}
	}

	void AddEvent(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea) {
		cout << "Adding Event to the Event List" << endl;
		_numEvents++;
		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year);
		int binX;
		int binY; 
		if (e->_year > _year) {
			binX = _overflow;
			binY = 0;
		}
		else {
			binX = Month;
			binY = Day;
		}
		if (_eventSet[binX][binY] == 0) {
			_eventSet[binX][binY] = e;
		}
		else if (timeOfDay < _eventSet[binX][binY]->_timeOfDay) {
			e->_nextEvent = _eventSet[binX][binY];
			_eventSet[binX][binY] = e;
		}
		else {
			Event* curr = _eventSet[binX][binY];
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
		return _eventSet[_baseX][_baseY]->_timeOfDay;
	}

	Time GetMonth() {
		return _eventSet[_baseX][_baseY]->_timeMonth;
	}

	Time GetDay() {
		return _eventSet[_baseX][_baseY]->_timeDay;
	}
	
	string ConvertMonth(Time month){
		switch((int) month){
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
		if (_baseX == December && _baseY == 31) {
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
			_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
		}	
	}

	void AdvanceDay() {
		_baseY++;
	}
};

SimExec::EventSet SimExec::_eventSet;
Time SimExec::_simulationTime = 0.0;

void SimExec::InitializeSimulation(int numBins, Time timeRange, int* days) {
   _simulationTime = 0.0;
   _eventSet.InitEventSet(numBins, timeRange, days);
}

Time SimExec::GetSimulationTime() {
	cout << "Returning Simulation Time" << endl;
    return _simulationTime;
}

void SimExec::ScheduleEventAt(Time timeMonth, Time timeDay, Time timeOfDay, int year, int priority, EventAction* ea) {
	cout << "Scheduling Event" << endl;
	_eventSet.AddEvent(timeMonth, timeDay, timeOfDay, year, priority, ea);
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
