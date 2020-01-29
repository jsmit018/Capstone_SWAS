#include "SimExec.h"

SimExec::SimExec() : SimObj(){
	
}

struct SimExec::Event {
	Event(Time time, EventAction* ea) {
		_time = time;
		_ea = ea;
		_nextEvent = 0;
	}
	Time _time;
	EventAction* ea;
	Event* _nextEvent;
};

class SimExec::EventSet {
public:
	EventSet() {

	}

	void InitEventSet(int numBins, Time timeRange, int* days) {
		_numEvents = 0;
		_numBins = numBins;
		_base = 0;
		_year = 2020;
		_overflow = numBins;
		_timeRange = timeRange;
		_binSize = timeRange / numBins;
		_baseT = 0.0;
		_eventSet = new Event * [numBins + 1];
		for (int i = 0; i < numBins; ++i){
			if (year % 4 == 0 && days[i] == February)
				_eventSet[i] = new Event* [days[i] + 1];
			else
				_eventSet[i] = new Event*[days[i]];
		}
		for (int i = 0; i < numBins + 1; ++i) {
			for (int j = 0; j < i - 1; ++j){
				_eventSet[i][j] = 0;
			}
		}
	}

	void AddEvent(Time time, EventAction* ea) {
		_numEvents++;
		Event* e = new Event(time, ea);
		int bin;
		if (time > _baseT + _timeRange) {
			bin = _overflow;
		}
		else {
			bin = (base + (int)floor((time - base) / _binSize)) % (_numBins + 1);
		}
		if (_eventSet[bin] == 0) {
			_eventSet[bin] = e;
		}
		else if (time < _eventSet[bin]->_time) {
			e->_nextEvent = _eventSet[bin];
			_eventSet[bin] = e;
		}
		else {
			Event* curr = _eventSet[bin];
			while ((curr->_nextEvent != 0) ? (e->_time >= curr->_nextEvent->_time) : false) {
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
		int bin = _base;
		while (_eventSet[bin] == 0) {
			bin = (bin + 1) % (_numBins + 1);
		}
		return _eventSet[bin]->_time;
	}

	EventAction* GetEventAction() {
		if (_numEvents > 0) {
			while (_eventSet[_base] == 0) {
				AdvanceBase();
			}
			Event* next = _eventSet[_base];
			_eventSet[_base] = next->_nextEvent;
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

	void PrintHistogram() {
		int bin = _base;
		cout << "Ladder: ";
		for (int i = 0; i < _numBins + 1; ++i) {
			Event* e = _eventSet[bin];
			int count = 0;
			while (e != 0) {
				e = e->_nextEvent;
				count++;
			}
			cout << count << " ";
			bin = (bin + 1) % (_numBins + 1);
		}
		cout << endl;
	}

private:
	int _numBins, _base, _overflow;
	Time _timeRange;
	Time _year;
	Time _binSize;
	Time _baseT;
	int _numEvents;
	Event** _eventSet;

	void AdvanceBase() {
		if (_numEvents > 0) {
			while (_eventSet[_base] == 0) {
				_base = (base + 1) % (_numBins + 1);
				_base += _binSize;
				int oldOverflow = _overflow;
				_overflow = (_overflow + 1) % (_numBins + 1);
				if (_eventSet[oldOverflow] != 0) {
					Time overflowT = _baseT + _timeRange;
					if (_eventSet[oldOverflow]->_time >= overflowT) {
						_eventSet[_overflow] = _eventSet[oldOverflow];
						_eventSet[oldOverflow] = 0;
					}
					else {
						Event* prev = _eventSet[oldOverflow];
						while ((prev->_nextEvent == 0) ? false : (prev->_nextEvent->_time < overflowT)) {
							prev = prev->_nextEvent;
						}
						_eventSet[_overflow] = prev->_nextEvent;
						prev->_nextEvent = 0;
					}
				}
			}
		}
	}
};

SimExec::EventSet SimExec::_eventSet;
Time SimulationExecutive::_simTime = 0.0;

void SimExec::InitializeSimulation(int numBins, Time timeRange) {
   _simulationTime = 0.0;
   _eventSet.InitEventSet(numBins, timeRange);
}

Time SimExec::GetSimulationTime() {
    return _simulationTime;
}

//This is handled by the advancing of the Simulation.. not needed.
/*void SimExec::SetSimulationTime() {

}*/

/*EventAction* SimExec::GetEventAction() {

}*/

//We will never set an event action this is done through scheduling.
/*void SetEventAction(EventAction* ea) {

}*/

/*EventSet* SimExec::GetEventSet() {
    return _eventSet;
}*/

void SimExec::ScheduleEventAt(Time time, EventAction* ea) {
	_eventSet.AddEvent(_simTime + time, ea);
}

void SimExec::ScheduleEventIn(Time time, EventAction* ea) {
	_eventSet.AddEvent(_simTime + time, ea);
}

void SimExec::RunSimulation() {
	while (_eventSet.HasEvent()) {
		_simulationTime = _eventSet.GetTime();
		EventAction* ea = _eventSet.GetEventAction();
		ea->Execute();
		delete ea;
	}
}

void SimExec::RunSimulation(Time time) {
	while (_eventSet.HasEvent() && _simTime <= time) {
		_simulationTime = _eventSet.GetTime();
		EventAction* ea = _eventSet.GetEventAction();
		ea->Execute();
		delete ea;
	}
}

