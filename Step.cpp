#include "step.h"
#include <iterator>
#include <map>

Step::Step(Distribution* serviceTime, string name)
{
	_serviceTime = serviceTime;
	_name = name;
}

class Step::AcquireServerEA : public EventAction {
public:
	AcquireServerEA(Step* step, Aircraft* aircraft){
		_step = step;
		_aircraft = aircraft;
	}
	
	void Execute(){
		_step->AcquireServerEM(_aircraft);
	}
private:
	Step* _step;
	Aircraft* _aircraft;
};

class Step::ScheduleDoneServiceEA : public EventAction{
public:

private:
};

class Step::AddQueueEA : public EventAction{
public:
	AddQueueEA(Step* step, Aircraft* aircraft) {
		_step = step;
		_aircraft = aircraft;
	}
	
	void Execute(){
		_step->AddQueueEM(_aircraft);
	}
	
private:
	Step* _step;
	Aircraft* aircraft;
};

//Since Service Time is now a distribution no need to get and retrieve.
/*Time Step::GetServiceTime()
{
	return Time();
}

void Step::SetServiceTime(Time serviceTime)
{
}*/

string Step::GetName()
{
	return _name;
}

void Step::SetName(string name)
{
	_name = name;
}

int Step::GetNumberInQueue()
{
	return _numberInQueue;
}

//We won't ever need this function as this will update throughout the simulation.
/*void Step::SetNumberInQueue(int NumberInQueue)
{
}*/

//Aircraft ID & Priority number
//PIFO (Priority In First Out)

//Acquiring a bay
void Step::AcquireServerEM(Aircraft* aircraft)
{
	iterator = _PIFOQueue.begin();
	_PIFOQueue.erase(iterator);
	_numberInQueue--;
	_bays--;
	//Will need an EA & Method that starts the maintenance
	SimulationExecutive::ScheduleEventIn(0.0, new WhateverEAIsCalled(this, aircraft));
	
}

void Step::AddQueueEM(Aircraft* aircraft)
{
	_PIFOQueue.insert(pair<int, Aircraft*>(aircraft->_priority, aircraft);
	_numberInQueue++;
	if (_bays > 0){
		SimulationExecutive::ScheduleEventIn(0.0, new AcquireServerEA(this, aircraft));
	}
}

void Step::ScheduleDoneServiceEM(Aircraft* aircraft)
{
}

//@TODO figure out what these are supposed to be... EM?
void Step::ReleaseServer()
{
}

void Step::DoneService()
{
}

void Step::Execute(Aircraft* aircraft){
	SimulationExecutive::ScheduleEventIn(0.0, new AddQueueEA(this, aircraft));
}
