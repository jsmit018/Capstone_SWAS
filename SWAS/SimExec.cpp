//SimExec.cpp: Provided by Dr. James Leathrum, modified by Jordan Smith

#include "SimExec.h"
#include "Resource.h"
#include "Parts.h"
#include "InputInterface.h"
#include "SinkTask.h"

SinkBlock* SimExec::_systemSink = new SinkBlock("System Sink");

void PlayButton()
{
	SimExec::RunSimulation();
}

void PlayButton(Time month, Time day, int year)
{
	SimExec::RunSimulation(month, day, year);
}

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

	//for testing
	void PrintEvent() {
		//cout << "_________________________________________________________________________________" << endl;
		//cout << "Event Action: " << _eventActionName << endl;
		//if (_timeOfDay >= 10)
		//	cout << "Will occur on: " << _timeMonth + 1 << " " << _timeDay + 1 << " at " << _timeOfDay << "00 in " << _year << endl;
		//else
		//	cout << "Will occur on: " << _timeMonth + 1 << " " << _timeDay + 1 << " at 0" << _timeOfDay << "00 in " << _year << endl;
		//cout << "Priority: " << _priority << endl;
		//cout << "_________________________________________________________________________________" << endl;
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

struct SimExec::CondEvent {
	CondEvent(int priority, CondEventAction* cea, string eaName, string type, string resourceNeeded, int aID, string repairJobName) {
		_priority = priority;
		_cea = cea;
		_nextCondEvent = 0;
		_prevCondEvent = 0;
		_eaName = eaName;
		_type = type;
		_resourceNeeded = resourceNeeded;
		_aID = aID;
		_repairJobName = repairJobName;
	}

	int _priority;
	int _aID;
	CondEventAction* _cea;
	CondEvent* _nextCondEvent;
	CondEvent* _prevCondEvent;
	string _eaName, _type, _resourceNeeded, _repairJobName;
};

class SimExec::CondEventSet {
public:
	CondEventSet() {
		_condSet = 0;
		_numInCES = 0;
	}

	void AddConditionalEvent(int priority, CondEventAction* cea, string eaName, string type, string resourceNeeded, int aID, string repairJobName) {
		_numInCES++;
		CondEvent* c = new CondEvent(priority, cea, eaName, type, resourceNeeded, aID, repairJobName);
		if (_condSet == 0) {
			_condSet = c;
		}
		else if (_condSet->_priority > c->_priority) {
			c->_nextCondEvent = _condSet;
			c->_prevCondEvent = 0;
			_condSet->_prevCondEvent = c;
			_condSet = c;
		}
		else {
			CondEvent* curr = _condSet;
			while (curr->_nextCondEvent != 0) {
				if (c->_priority < curr->_priority)
					break;
				else
					curr = curr->_nextCondEvent;
			}
			if (curr->_nextCondEvent == 0) {
				c->_prevCondEvent = curr;
				curr->_nextCondEvent = c;
			}
			else {
				c->_nextCondEvent = curr->_nextCondEvent;
				c->_prevCondEvent = curr;
				c->_nextCondEvent->_prevCondEvent = c;
				curr->_nextCondEvent = c;
			}
		}
	}

	bool CheckConditionalEvents(StepResource* resource, Parts* parts) 
	{
		CondEvent* curr = _condSet;
		while (curr != 0) 
		{
			//Check to see if the current nodes conditions are met
			if (curr->_cea->Condition(resource, parts)) 
			{
				//If met Execute the associated Event Method
				cout << "Executed from CES, ID: " << curr->_aID <<endl;
				curr->_cea->Execute();

				//Event has Executed, see if the current node is the head node
				if (curr == _condSet) 
				{

					//If it is advance the head node to the next node
					_condSet = _condSet->_nextCondEvent;

					//If there is no next node and head node is now null
					if (_condSet == NULL) 
					{
						//reinitialize it to 0 and delete the placeholder
						_condSet = 0;
						//delete curr;
					}
					else 
					{
						//Set the previous node to 0 as there is no other node infront of head and delete the placeholder
						_condSet->_prevCondEvent = 0;
						//delete curr;
					}
				}
				else 
				{
					//Curr is not head node so connect the last node to the next node and then delete placeholder
					curr->_prevCondEvent->_nextCondEvent = curr->_nextCondEvent;
					if (curr->_nextCondEvent == NULL) 
					{
						curr->_prevCondEvent->_nextCondEvent = 0;
						//continue;
					}
					else
						curr->_nextCondEvent->_prevCondEvent = curr->_prevCondEvent;
					//delete curr;
				}
				//Return true that the event executed
				delete curr;
				_numInCES--;
				return true;
			}
			//If curr's condition isn't met check the next node
			curr = curr->_nextCondEvent;
		}
		//No Conditions events were met for resources.
		return false;
	}

	CondEvent* GetConditionalSet() {
		return _condSet;
	}

	int GetNumInCES() {
		return _numInCES;
	}

private:
	CondEvent* _condSet;
	int _numInCES;
};

class SimExec::EventSet {
public:
	EventSet() {

	}

	void InitEventSet(int numBins, int* days) {

		_numEvents = 0;
		_numBins = numBins;
		_baseX = 0;
		_baseY = 0;
		_year = 2020;
		_overflow = 12;
		_eventSet = new Event * *[numBins + 1];
		_endOfMonth = new int[numBins];
		for (int i = 0; i < numBins + 1; ++i) {
			if (_year % 4 == 0 && i == February) {
				_eventSet[i] = new Event * [days[i] + 1];
			}
			else if (i == 12)
				_eventSet[i] = new Event * [1];
			else
				_eventSet[i] = new Event * [days[i]];

		}
		//Initializing the linked list of each Month & Day
		for (int i = 0; i < numBins + 1; ++i) {
			if (i == 12) {
				//Overflow Initialzed
				_eventSet[i][0] = 0;
			}
			for (int j = 0; j < days[i]; ++j) {
				//ConvertMonth(i)
				if (i == February && j == 27 && _year % 4 == 0) {

					_eventSet[i][j + 1] = 0;
				}
				_eventSet[i][j] = 0;
				if (i == 12 && j == 1) {
					//"Overflow Initialzed breaking initialization loop"
					break;
				}
			}
		}
		//Setting the end of month dates" 
		for (int i = 0; i < numBins; ++i) {
			_endOfMonth[i] = days[i] - 1;
		}
	}

	void AddEventCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName) {

		//Adding Event to the Event List
		_numEvents++;
		if (eaName == "ShiftChangeEA" || eaName == "Shift1Change" || eaName == "Shift2Change" || eaName == "Shift3Change")
			_numEvents--;
		//Number of Events increased to numevents
		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year, eaName);
		e->PrintEvent();
		int binX;
		int binY;
		//Hashing year to see the appopriate place to add the event
		if (e->_year > _year) {
			//Event isn't in this calendar year, adding it to the overflow bin
			binX = _overflow;
			binY = 0;
		}
		else {
			//Setting the bins to the appopriate month & day
			binX = Month;
			binY = Day;
		}
		//Checking to see where to place the new event
		//Checking to see if Month and Day tuple is 0
		//If tuple isn't 0 then we check to see if the new event has a higher priority or an earlier time than the head
		//If either of those conditions aren't met then we cycle through the list to find the appopriate place to put the new event
		if (_eventSet[binX][binY] == 0) {
			//Tuple is 0, adding the event to the head of the list
			_eventSet[binX][binY] = e;
		}
		else if (timeOfDay < _eventSet[binX][binY]->_timeOfDay) {
			//New event has an earlier time than the head updating the list
			e->_nextEvent = _eventSet[binX][binY];
			_eventSet[binX][binY] = e;
		}
		else {
			Event* curr = _eventSet[binX][binY];
			//Searching the list on where to place the event based on time and priority
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
	}

	void AddEventRecurring(int priority, EventAction* ea, double distributionValue, int recurring, string eaName) {
		Time Month = 0.0;
		Time Day = 0.0;
		Time timeOfDay = 0.0;
		int year = _year;
		_numEvents++;
		if (eaName == "FailResourceEA" || eaName == "RestoreResourceEA")
			_numEvents--;

		//Converting Distribution to Appropriate Time
		TimeConverter::ConvertDistributionToMonthDay(Month, Day, timeOfDay, year, distributionValue, _baseX, _baseY, _endOfMonth, _simulationTime._timeOfDay, 0, recurring);
		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year, eaName);
		e->PrintEvent();
		int binX;
		int binY;
		//Hashing year to see the appopriate place to add the event
		if (e->_year > _year) {
			//Event isn't in this calendar year, adding it to the overflow bin
			binX = _overflow;
			binY = 0;
		}
		else {
			//Setting the bins to the appopriate month & day
			binX = Month;
			binY = Day;
		}
		//Checking to see if Month and Day tuple is 0
		//If tuple isn't 0 then we check to see if the new event has a higher priority or an earlier time than the head
		//If either of those conditions aren't met then we cycle through the list to find the appopriate place to put the new event
		if (_eventSet[binX][binY] == 0) {
			//		Tuple is 0, adding the event to the head of the list
			_eventSet[binX][binY] = e;
		}
		else if (timeOfDay < _eventSet[binX][binY]->_timeOfDay) {
			//		New event has an earlier time than the head updating the list
			e->_nextEvent = _eventSet[binX][binY];
			_eventSet[binX][binY] = e;
		}
		else {
			Event* curr = _eventSet[binX][binY];
			//		Searching the list on where to place the event based on time and priority" ;
			if (year == 2022)

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
	}

	void AddEvent(int priority, EventAction* ea, double distributionValue, string eaName, int daysOrHours) {
		Time Month = 0.0;
		Time Day = 0.0;
		Time timeOfDay = 0.0;
		int year = _year;
		//	Adding Event to the Event List" 
		_numEvents++;
		if (eaName == "ShiftChangeEA" || eaName == "Shift1Change" || eaName == "Shift2Change" || eaName == "Shift3Change")
			_numEvents--;
		//	Number of Events increased to " << _numEvents 
		//	Converting Distribution to Appropriate Time" 
		if (daysOrHours == 0)
			TimeConverter::ConvertDistributionToMonthDay(Month, Day, timeOfDay, year, distributionValue, _baseX, _baseY, _endOfMonth, GetSimulationTime()._timeOfDay);
		else
			TimeConverter::ConvertDistributionToMonthDay(Month, Day, timeOfDay, year, distributionValue, _baseX, _baseY, _endOfMonth, GetSimulationTime()._timeOfDay, 1);

		Event* e = new Event(ea, Month, Day, timeOfDay, priority, year, eaName);
		if (Month == 0 && Day == 0 && timeOfDay == 4)
			//Bleh" 
		e->PrintEvent();
		int binX;
		int binY;
		//	Hashing year to see the appopriate place to add the event" 
		if (e->_year > _year) {
			//		Event isn't in this calendar year, adding it to the overflow bin" 
			binX = _overflow;
			binY = 0;
		}
		else {
			//		Setting the bins to the appopriate month & day" 
			binX = Month;
			binY = Day;
		}
		/*Checking to see if Month and Day tuple is 0" 
		If tuple isn't 0 then we check to see if the new event has a higher priority or an earlier time than the head" 
		If either of those conditions aren't met then we cycle through the list to find the appopriate	place to put the new event" */
		if (_eventSet[binX][binY] == 0) {
			//	Tuple is 0, adding the event to the head of the list" 
			_eventSet[binX][binY] = e;
		}
		else if (timeOfDay < _eventSet[binX][binY]->_timeOfDay) {
			//	New event has an earlier time than the head updating the list" 
			e->_nextEvent = _eventSet[binX][binY];
			_eventSet[binX][binY] = e;
		}
		else {
			Event* curr = _eventSet[binX][binY];
			//if (_year == 2022)
				////Bleh";
			//	Searching the list on where to place the event based on time and priority" 
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
	}

	Time GetTimeOfDay() {
		//Returning time of day" 
		return _eventSet[_baseX][_baseY]->_timeOfDay;
	}

	Time GetMonth() {
		//	Returning Month" 
		/*if (_eventSet[_baseX][_baseY]->_timeMonth == NULL)
			return 0;
		else*/
		return _eventSet[_baseX][_baseY]->_timeMonth;
	}

	Time GetDay() {
		//	Returning Day of the Month" 
		/*if (_eventSet[_baseX][_baseY]->_timeDay == NULL)
			return 0;*/
			/*	else*/
		return _eventSet[_baseX][_baseY]->_timeDay;
	}

	int GetYear() {
		//	Returning Year" 
		/*i*//*f (_eventSet[_baseX][_baseY]->_year == NULL)
			return 0;
		else*/
		return _eventSet[_baseX][_baseY]->_year;
	}

	int GetNumEvents() {
		/*if (_numEvents == 2196) {
			PrintCalendar();
		}*/
		return _numEvents;
		
	}

	string ConvertMonth(Time month) {
		//	Converting month into a string" 
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

	void ReinitializeSim() {
		while (true) {
			_eventSet[_baseX][_baseY] = 0;
			if (_baseY == _endOfMonth[_baseX]) {
				_baseX++;
				_baseY = 0;
			}
			else
				_baseY++;

			if (_baseX == December && _baseY == _endOfMonth[_baseX]) {
				_eventSet[_overflow][0] = 0;
				_baseX = 0;
				_baseY = 0;
				_simulationTime._day = 0;
				_simulationTime._month = 0;
				_simulationTime._year = 2020;
				_simulationTime._timeOfDay = 6.0;
				CondEvent* ces = _conditionalSet.GetConditionalSet();
				ces = 0;
				break;
			}

		}
	}

	EventAction* GetEventAction() {
		//	Getting Event Action" 
		if (_numEvents > 0) {
			while (_eventSet[_baseX][_baseY] == 0) {
				
				if (_baseY == _endOfMonth[_baseX]) {
					AdvanceMonth();
				}
				else {
					AdvanceDay();
				}

			}
			Event* next = _eventSet[_baseX][_baseY];

			_eventSet[_baseX][_baseY] = _eventSet[_baseX][_baseY]->_nextEvent;
			
				_simulationTime._timeOfDay = next->_timeOfDay;
				_simulationTime._month = next->_timeMonth;
				_simulationTime._day = next->_timeDay;
				_simulationTime._year = next->_year;

			EventAction* ea = next->_ea;
			_numEvents--;
			if (next->_eventActionName == "FailResourceEA" || next->_eventActionName == "RestoreResourceEA" || next->_eventActionName == "ShiftChangeEA" 
				|| next->_eventActionName == "Shift1Change" || next->_eventActionName == "Shift2Change" || next->_eventActionName == "Shift3Change")
				_numEvents++;
			delete next;
			return ea;
		}
		else {
			if (_simulationFlag)
			{
				Event* currEvent = _eventSet[_overflow][0];
				while (currEvent != 0) {
					if (currEvent->_year > _year + 2)
						currEvent = currEvent->_nextEvent;
					else {
						int baseX, baseY;
						baseX = currEvent->_timeMonth;
						baseY = currEvent->_timeDay;
						if (_eventSet[baseX][baseY] == 0)
							_eventSet[baseX][baseY] = currEvent;
						else if (currEvent->_timeOfDay < _eventSet[baseX][baseY]->_timeOfDay) {
							currEvent->_nextEvent = _eventSet[baseX][baseY];
							_eventSet[baseX][baseY] = currEvent;
						}
						else {
							Event* eventSet = _eventSet[baseX][baseY];
							while ((currEvent->_nextEvent != 0) ? (currEvent->_timeOfDay >= eventSet->_timeOfDay && !(currEvent->_timeOfDay < eventSet->_nextEvent->_timeOfDay)) : false) {
								if (currEvent->_timeOfDay == eventSet->_nextEvent->_timeOfDay) {
									if (currEvent->_priority < eventSet->_nextEvent->_priority) {
										break;
									}
									else
										eventSet = eventSet->_nextEvent;
								}
								else
									eventSet = eventSet->_nextEvent;
							}
							if (eventSet->_nextEvent == 0 && currEvent->_timeOfDay >= eventSet->_timeOfDay) {
								eventSet->_nextEvent = currEvent;
							}
							else {
								currEvent->_nextEvent = eventSet->_nextEvent;
								eventSet->_nextEvent = currEvent;
							}
						}
					}
				}
				return GetEventAction();
			}
			else
				return 0;
		}
	}

	bool HasEvent() {
		//	Checking to see if number of events is greater than 0" 
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
		//	Advancing Month, and updating overflow bin" 
		if (_baseX == December && _baseY == 30) {
			_eventSet[_baseX][_baseY] = 0;
			_year++;
			_baseX = 0;
			_baseY = 0;
			_totalDaysPassed++;
			
		}
		else {
			_eventSet[_baseX][_baseY] = 0;
			_baseX++;
			_baseY = 0;
			_totalDaysPassed++;
			
		}

		int previousBase;
		if (_baseX == 0)
			previousBase = December;
		else
			previousBase = _baseX - 1;
		if (_eventSet[_overflow][_baseY] == NULL)
			return;
		bool tracker = true;
		int eventDay = 0;
		Event* currEvent = _eventSet[_overflow][_baseY];
		bool isSame = false;
		if (previousBase == December) {
			while (_eventSet[_overflow][_baseY] != 0) {
				if (isSame) {}
				else 
					currEvent = _eventSet[_overflow][_baseY];
				if (currEvent->_year == _year) {
					Event* nodeChecker = _eventSet[_overflow][_baseY];
					Event* hold = new Event(0, 0, 0, 0, 0, 0, "hold");
						eventDay = currEvent->_timeDay;
						previousBase = currEvent->_timeMonth;
						int breaker = 0;
						if (_eventSet[previousBase][eventDay] == 0) {
							hold = currEvent->_nextEvent;
							//currEvent->_nextEvent = 0;
							_eventSet[previousBase][eventDay] = currEvent;
							_eventSet[previousBase][eventDay]->_nextEvent = 0;
							if (isSame == false) {
								_eventSet[_overflow][_baseY] = hold;
								continue;
							}
							//if (_eventSet[_overflow][_baseY] == currEvent) {
							//	_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
							//	/*	Event* deletion = currEvent;
							//		currEvent = currEvent->_nextEvent;
							//		delete deletion;
							//		continue;*/
							//}
							//continue;
						}
						else {
							/*Sort it into the list of events*/
							if (currEvent->_timeOfDay < _eventSet[previousBase][eventDay]->_timeOfDay) {
								//breaker = 1;
								hold = currEvent->_nextEvent;
								currEvent->_nextEvent = _eventSet[previousBase][eventDay];
								_eventSet[previousBase][eventDay] = currEvent;
								if (isSame == false) {
									_eventSet[_overflow][_baseY] = hold;
									continue;
								}
								//while (nodeChecker->_nextEvent != currEvent) {
								//	if (nodeChecker == currEvent) {
								//		_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
								//		/*Event* deletion = currEvent;
								//		currEvent = currEvent->_nextEvent;
								//		delete deletion;
								//		breaker++;
								//		break;*/
								//	}
								//	else
								//		nodeChecker = nodeChecker->_nextEvent;
								//}
								/*if (breaker == 1)
									continue;*/
									/*nodeChecker->_nextEvent = currEvent->_nextEvent;
									Event* deletion = currEvent;
									currEvent = currEvent->_nextEvent;
									delete deletion;
									continue;*/
							}
							else {
								Event* curr = _eventSet[previousBase][eventDay];
								while ((curr->_nextEvent != 0) ? (currEvent->_timeOfDay >= curr->_timeOfDay && !(currEvent->_timeOfDay < curr->_nextEvent->_timeOfDay)) : false) {
									if (currEvent->_timeOfDay == curr->_nextEvent->_timeOfDay) {
										if (currEvent->_priority < curr->_nextEvent->_priority) {
											break;
										}
										else
											curr = curr->_nextEvent;
									}
									else
										curr = curr->_nextEvent;
								}
								if (curr->_nextEvent == 0 && currEvent->_timeOfDay >= curr->_timeOfDay) {
									hold = currEvent->_nextEvent;
									currEvent->_nextEvent = 0;
									curr->_nextEvent = currEvent;
									if (isSame == false) {
										_eventSet[_overflow][_baseY] = hold;
										continue;
									}
								}
								else {
									hold = currEvent->_nextEvent;
									currEvent->_nextEvent = curr->_nextEvent;
									curr->_nextEvent = currEvent;
									if (isSame == false) {
										_eventSet[_overflow][_baseY] = hold;
										continue;
									}
								}
								/*while (nodeChecker->_nextEvent != currEvent) {
									if (nodeChecker == currEvent) {
										_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
										Event* deletion = currEvent;
										currEvent = currEvent->_nextEvent;
										delete deletion;
										breaker++;
										break;
									}
									else
										nodeChecker = nodeChecker->_nextEvent;
								}
								if (breaker == 1)
									continue;
								nodeChecker->_nextEvent = currEvent->_nextEvent;
								Event* deletion = currEvent;
								currEvent = currEvent->_nextEvent;
								delete deletion;
								continue;*/
							}
						}
						//currEvent = currEvent->_nextEvent;
						//nodeChecker = nodeChecker->_nextEvent;
						//Event* nodeChecker = _eventSet[_overflow][_baseY];
						Event* checker = _eventSet[_overflow][_baseY];
						if (checker == currEvent) {
							_eventSet[_overflow][_baseY]->_nextEvent = hold;
							_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
							//currEvent = _eventSet[_overflow][_baseY];
							/*if (_eventSet[_overflow][_baseY] == NULL)
								tracker = false;*/
						}
						else {
							while (checker->_nextEvent != currEvent) {
								checker = checker->_nextEvent;
							}
							checker->_nextEvent = hold;
							isSame = false;
							//currEvent = _eventSet[_overflow][_baseY];
						}
				}
				else {
				currEvent = currEvent->_nextEvent;
				isSame = true;
				}
			}
		}
		else {
			while (tracker && currEvent != 0) {
				//currEvent = nodeChecker;

				Event* nodeChecker = _eventSet[_overflow][_baseY];
				Event* hold = new Event(0, 0, 0, 0, 0, 0, "hold");
				if (currEvent->_timeMonth == previousBase) {
					eventDay = currEvent->_timeDay;
					int breaker = 0;
					if (_eventSet[previousBase][eventDay] == 0) {
						hold = currEvent->_nextEvent;
						_eventSet[previousBase][eventDay] = currEvent;
						_eventSet[previousBase][eventDay]->_nextEvent = 0;
						//if (_eventSet[_overflow][_baseY] == currEvent) {
						//	_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
						//	/*	Event* deletion = currEvent;
						//		currEvent = currEvent->_nextEvent;
						//		delete deletion;
						//		continue;*/
						//}
					}
					else {
						/*Sort it into the list of events*/
						if (currEvent->_timeOfDay < _eventSet[previousBase][eventDay]->_timeOfDay) {
							//breaker = 1;
							hold = currEvent->_nextEvent;
							currEvent->_nextEvent = _eventSet[previousBase][eventDay];
							_eventSet[previousBase][eventDay] = currEvent;
							//while (nodeChecker->_nextEvent != currEvent) {
							//	if (nodeChecker == currEvent) {
							//		_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
							//		/*Event* deletion = currEvent;
							//		currEvent = currEvent->_nextEvent;
							//		delete deletion;
							//		breaker++;
							//		break;*/
							//	}
							//	else
							//		nodeChecker = nodeChecker->_nextEvent;
							//}
							/*if (breaker == 1)
								continue;*/
								/*nodeChecker->_nextEvent = currEvent->_nextEvent;
								Event* deletion = currEvent;
								currEvent = currEvent->_nextEvent;
								delete deletion;
								continue;*/
						}
						else {
							Event* curr = _eventSet[previousBase][eventDay];
							while ((curr->_nextEvent != 0) ? (currEvent->_timeOfDay >= curr->_timeOfDay && !(currEvent->_timeOfDay < curr->_nextEvent->_timeOfDay)) : false) {
								if (currEvent->_timeOfDay == curr->_nextEvent->_timeOfDay) {
									if (currEvent->_priority < curr->_nextEvent->_priority) {
										break;
									}
									else
										curr = curr->_nextEvent;
								}
								else
									curr = curr->_nextEvent;
							}
							if (curr->_nextEvent == 0 && currEvent->_timeOfDay >= curr->_timeOfDay) {
								hold = currEvent->_nextEvent;
								curr->_nextEvent = currEvent;
							}
							else {
								hold = currEvent->_nextEvent;
								currEvent->_nextEvent = curr->_nextEvent;
								curr->_nextEvent = currEvent;
							}
							/*while (nodeChecker->_nextEvent != currEvent) {
								if (nodeChecker == currEvent) {
									_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
									Event* deletion = currEvent;
									currEvent = currEvent->_nextEvent;
									delete deletion;
									breaker++;
									break;
								}
								else
									nodeChecker = nodeChecker->_nextEvent;
							}
							if (breaker == 1)
								continue;
							nodeChecker->_nextEvent = currEvent->_nextEvent;
							Event* deletion = currEvent;
							currEvent = currEvent->_nextEvent;
							delete deletion;
							continue;*/
						}
					}
					//currEvent = currEvent->_nextEvent;
					//nodeChecker = nodeChecker->_nextEvent;
					//Event* nodeChecker = _eventSet[_overflow][_baseY];
					Event* checker = _eventSet[_overflow][_baseY];
					if (checker == currEvent) {
						_eventSet[_overflow][_baseY]->_nextEvent = hold;
						_eventSet[_overflow][_baseY] = _eventSet[_overflow][_baseY]->_nextEvent;
						currEvent = _eventSet[_overflow][_baseY];
						if (_eventSet[_overflow][_baseY] == NULL)
							tracker = false;
					}
					else {
						while (checker->_nextEvent != currEvent) {
							checker = checker->_nextEvent;
						}
						checker->_nextEvent = hold;
						currEvent = _eventSet[_overflow][_baseY];
					}
				}
				else if (currEvent == 0)
					tracker = false;
				else
					currEvent = currEvent->_nextEvent;
			}
			/*if (_eventSet[_overflow][0] == NULL)
				_eventSet[_overflow][0] = 0;
			delete currEvent;*/
		}
	}

	void AdvanceDay() {
		//Advancing Day" 
		_eventSet[_baseX][_baseY] = 0;
		_baseY++;
		_totalDaysPassed++;
	}
};

SimExec::EventSet SimExec::_eventSet;
SimExec::CondEventSet SimExec::_conditionalSet;
SimulationTime SimExec::_simulationTime;
InputReader SimExec::_inputReader;
bool SimExec::_simulationFlag;
int SimExec::_totalDaysPassed = 0;


void SimExec::InitializeSimulation(int numBins, int* days) {

	//	Setting Simulation time to 0" 

	_simulationTime._timeOfDay = 6.0;

	_simulationTime._month = 0;
	_simulationTime._day = 0;
	_simulationTime._year = 2020;
	_simulationFlag = true;
	_eventSet.InitEventSet(numBins, days);
}

void SimExec::ReinitalizeSimulation()
{
	_eventSet.ReinitializeSim();
}

SimulationTime SimExec::GetSimulationTime() {
	//	Returning Simulation Time" 
	return _simulationTime;
}

InputReader SimExec::GetInputReader()
{
	return _inputReader;
}

void SimExec::SetInputReader(InputReader inputReader)
{
	_inputReader = inputReader;
}

void SimExec::ScheduleEventAt(int priority, EventAction* ea, double distributionValue, string eaName, int daysOrHours) {
		//Scheduling Event" 
	_eventSet.AddEvent(priority, ea, distributionValue, eaName, daysOrHours);
}

void SimExec::ScheduleEventAtCalendar(Time Month, Time Day, Time timeOfDay, int year, int priority, EventAction* ea, string eaName) {
		//Scheduling Calendar Event" 
	_eventSet.AddEventCalendar(Month, Day, timeOfDay, year, priority, ea, eaName);
}


void SimExec::ScheduleEventAtRecurring(int priority, EventAction* ea, double distributionValue, string eaName, int recurring)
{
	//	Scheduling Recurring Event" 
	_eventSet.AddEventRecurring(priority, ea, distributionValue, recurring, eaName);
}

void SimExec::ScheduleConditionalEvent(int priority, CondEventAction* cea, string eaName, string type, string resourceNeeded, int aID, string repairJobName)
{
	//	Scheduling Conditional Event";
	_conditionalSet.AddConditionalEvent(priority, cea, eaName, type, resourceNeeded, aID, repairJobName);
}

void SimExec::SetSystemSink(SinkBlock* sinkBlock)
{
	_systemSink = sinkBlock;
}

SinkBlock* SimExec::GetSystemSink()
{
	return _systemSink;
}

string SimExec::ConvertDate(Time month)
{
	return _eventSet.ConvertMonth(month);
}

void SimExec::CheckConditionalEvents(StepResource* resource, Parts* parts)
{
	while (_conditionalSet.CheckConditionalEvents(resource, parts));
}

void SimExec::PrintEventSet()
{
	_eventSet.PrintCalendar();
}

//void SimExec::RunSimulation() {
int SimExec::RunSimulation() {
	//Running Simulation" 
	//Executing Event" 
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
		if (!_eventSet.HasEvent())
			FlipSimulationFlag();
		return 0;
	}
	else if (_simulationFlag == false) {
		//	Simulation Terminating" 
		if (_simulationTime._timeOfDay >= 10) {

			//for testing
			//	Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
			//	<< " at " << _simulationTime._timeOfDay << "00 in " << _simulationTime._year 
		}
		else if (_simulationTime._timeOfDay) {

			//for testing
			//Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
			//	<< " at 0" << _simulationTime._timeOfDay << "00 in " << _simulationTime._year 
		}
		return 3;
	}
}

//void SimExec::RunSimulation(Time month, Time day, Time timeOfDay, int year) {
int SimExec::RunSimulation(Time month, Time day, int year) {
	//Running Simulation" 
	//Executing Event" << endl;
	//While (_eventSet.HasEvent() && _simulationFlag == true) {
	if (_eventSet.HasEvent() && _simulationFlag == true) {
		if (_simulationTime._month >= (int)month && _simulationTime._day >= (int)day && _simulationTime._timeOfDay >= 0
			&& _simulationTime._year >= year) {
			FlipSimulationFlag();
			if (_simulationTime._timeOfDay >= 10) {
			
				//for testing
				//cout << "Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
				//<< " at " << _simulationTime._timeOfDay << "00 in " << _simulationTime._year << endl;
			}
		else {
				//for testing
				//cout << "Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
				//<< " at 0" << _simulationTime._timeOfDay << "00 in " << _simulationTime._year << endl;
		}
			return 3;
		}
		else {

			EventAction* ea = _eventSet.GetEventAction();
			if (ea == 0) {
				FlipSimulationFlag();
				return 3;
			}
			ea->Execute();
			delete ea;
			return 0;
		}
	}
	else if (_simulationFlag == false || !_eventSet.HasEvent()) {
		if (_simulationTime._timeOfDay >= 10) {
			cout << "Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
				<< " at " << _simulationTime._timeOfDay << "00 in " << _simulationTime._year << endl;
		}
		else {
			cout << "Simulation Terminated at time " << _eventSet.ConvertMonth(_simulationTime._month) << " " << _simulationTime._day + 1
				<< " at 0" << _simulationTime._timeOfDay << "00 in " << _simulationTime._year << endl;
		}
		_simulationFlag = false;
		return 3;
	}
}

void SimExec::FlipSimulationFlag()
{
	if (_simulationFlag == true)
		_simulationFlag = false;
	else
		_simulationFlag = true;
}

bool SimExec::GetSimulationFlag()
{
	return _simulationFlag;
}

Time SimExec::GetTotalSimulationTime()
{
	return (_totalDaysPassed * 24) + _simulationTime._timeOfDay;
}

int SimExec::PrintNumInCondES()
{
	int condEventTracker = 0;
	CondEvent* start = _conditionalSet.GetConditionalSet();
	while (start != 0) {
		cout << "Event: " << start->_eaName << ", Aircraft: " << start->_type << ", Needs: " << start->_resourceNeeded << ", ID: " << start->_aID << 
			", Repair Job: " << start->_repairJobName << endl;
		start = start->_nextCondEvent;
		condEventTracker++;
	}
	cout << _eventSet.GetNumEvents() << endl;
	cout << "Total number of Aircraft that have left the Simulation: " << GetSystemSink()->GetTerminatedAircraft() << endl;
	return condEventTracker;
}

void StopSimulation()
{
	SimExec::FlipSimulationFlag();
}

