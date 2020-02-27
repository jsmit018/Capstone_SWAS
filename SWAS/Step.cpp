#include "Step.h"
#include "Distribution.h"
#include "SimExec.h"
#include "Aircraft.h"
#include <iterator>
#include <map>

Step::Step(string serviceTime, string name) : Task(name)
{
	_servTime = serviceTime;
	_name = name;
}

class Step::AcquireServerEA : public EventAction {
public:
	AcquireServerEA(Step* step, Aircraft* aircraft) {
		_step = step;
		_aircraft = aircraft;
	}

	void Execute() {
		_step->AcquireServerEM(_aircraft);
	}
private:
	Step* _step;
	Aircraft* _aircraft;
};

class Step::ScheduleDoneServiceEA : public EventAction {
public:

private:
};

class Step::AddQueueEA : public EventAction {
public:
	AddQueueEA(Step* step, Aircraft* aircraft) {
		_step = step;
		_aircraft = aircraft;
	}

	void Execute() {
		_step->AddQueueEM(_aircraft);
	}

private:
	Step* _step;
	Aircraft* _aircraft;
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
int Step::GetNumberInQueue()
{
	//return _numberInQueue;
	return 0;
}

//We won't ever need this function as this will update throughout the simulation.
/*void Step::SetNumberInQueue(int NumberInQueue)
{
}*/

//Aircraft ID & Priority number


//Acquiring a bay
void Step::AcquireServerEM(Aircraft* aircraft)
{
	//_iterator = _PriorityQueue.begin();
	//_PriorityQueue.erase(_iterator);
	//_numberInQueue--;
	//	_bays->Acquire(1);
		//Will need an EA & Method that starts the maintenance
		//SimExec::ScheduleEventIn(0.0, new WhateverEAIsCalled(this, aircraft));

}

void Step::AddQueueEM(Aircraft* aircraft)
{
	//	_PriorityQueue.insert(pair<int, Aircraft*>(aircraft->GetAircraftPriority(), aircraft));

	//_numberInQueue++;
	if (_bays > 0) {
		//SimExec::ScheduleEventIn(0.0, new AcquireServerEA(this, aircraft));
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

void Step::Execute(Aircraft* aircraft) {
	//SimExec::ScheduleEventIn(0.0, new AddQueueEA(this, aircraft));
}



////////////////// Setters /////

void Step::SetName(string name)
{
	_name = name;
}

void Step::SetType(string type)
{
	_type = type;
}

void Step::SetInspecFailProb(string failureProb)
{
	_inspecFailProb = failureProb;
}

void Step::SetServiceTime(string serviceTime)
{
	_servTime = serviceTime;
}

void Step::SetReqResource(string reqResource)
{
	_reqRes = reqResource;
}


void Step::SetReqParts(string reqParts)
{
	_reqParts = reqParts;
}


void Step::SetReturnStep(int stepId)
{
	_returnStep = stepId; 
}

void Step::Print()
{
	cout << "Step name: " << _name << endl; 
}