#include "SimExec.h"


/*SimExec::SimExec() : SimObj(){
	
}*/

struct SimExec::Event {
	Event(EventAction* ea, Time timeMonth, Time timeDay, Time timeOfDay, int year) {
		_ea = ea;
		_nextEvent = 0;
		_timeMonth = timeMonth;
		_timeDay = timeDay;
		_year = year;
		_timeOfDay = timeOfDay;
	}
	Time _timeOfDay;
	Time _timeMonth;
	Time _timeDay;
	EventAction* _ea;
	Event* _nextEvent;
	int _year;
};

class SimExec::EventSet {
public:
	EventSet() {

	}

	void InitEventSet(int numBins, Time timeRange, int* days) {
		_numEvents = 0;
		_numBins = numBins;
		_baseX = 0;
		_baseY = 0;
		_year = 2020;
		_overflow = 13;
		_timeRange = timeRange;
		_binSize = timeRange / numBins;
		_baseT = 0.0;
		_eventSet = new Event ** [numBins + 1];
		_endOfMonth = new int[numBins];
		for (int i = 0; i < numBins; ++i){
			if (_year % 4 == 0 && i == February)
				_eventSet[i] = new Event*[days[i] + 1];
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

	void AddEvent(Time Month, Time Day, Time, timeOfDay, int year, EventAction* ea) {
		_numEvents++;
		Event* e = new Event(ea, Month, Day, timeOfDay, year);
		int binX;
		int binY; 
		if (e->_year > _year) {
			binX = _overflow;
			binY = 0;
		}
		else {
			binX = timeMonth;
			binY = timeDay;
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
	}

	Time GetTime() {
		int binX = _baseX;
		int binY = 0;
		int checkX = binX;
		while (_eventSet[binX][binY] == 0) {
			if (checkX == binX) {
				binY++;
			}
			else
				binX = (binX + 1) % (_numBins + 1);
			if (binX != checkX)
				binY = 0;
		}
		return _eventSet[binX][binY]->_time;
	}

	EventAction* GetEventAction() {
		if (_numEvents > 0) {
			//will need to populate a baseY for days
			while (_eventSet[_baseX][_baseY] == 0) {
				//Likely endOfMonth will need to be an array to make sure appropriate days are matched as endOfMonth
				if (_baseY == _endOfMonth[_baseX]) {
					AdvanceMonth();
				}
				else
					AdvanceDay();
			}
			Event* next = _eventSet[_baseX][_baseY];
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
	Time _timeRange;
	int _year;
	Time _binSize;
	Time _baseT;
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
		int eventDay;
		if (_baseX == 0)
			previousBase = December;
		else
			previousBase = _baseX - 1;
		
		Event* currEvent = _eventSet[_overflow][_baseY];
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
    return _simulationTime;
}

//void SimExec::ScheduleEventAt(Time timeMonth, Time timeDay, Time timeOfDay, int year, EventAction* ea) {
//	_eventSet.AddEvent(timeMonth, timeDay, timeOfDay, year, ea);
//}
//
//May Actually not be able to use this function because its all calendar date scheduling
//void SimExec::ScheduleEventIn(Time timeMonth, Time timeDay, int year, EventAction* ea) {
//	_eventSet.AddEvent(_simTime + time, ea);
//}
//
void SimExec::RunSimulation() {
	while (_eventSet.HasEvent()) {
		_simulationTime = _eventSet.GetTime();
		EventAction* ea = _eventSet.GetEventAction();
		ea->Execute();
		delete ea;
	}
}

void SimExec::RunSimulation(Time time) {
	while (_eventSet.HasEvent() && _simulationTime <= time) {
		_simulationTime = _eventSet.GetTime();
		EventAction* ea = _eventSet.GetEventAction();
		ea->Execute();
		delete ea;
	}
}
