#include "step.h"

Step::Step(Distribution* serviceTime, string name)
{
	_serviceTime = serviceTime;
	_name = name;
}

class Step::AcquireServerEA : public EventAction {
public:

private:
};

class Step::ScheduleDoneServiceEA : public EventAction{
public:

private:
};

class Step::AddQueueEA : public EventAction{
public:
	
private:
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

void Step::AcquireServerEM(Airplane* airplane)
{
}

void Step::AddQueueEM(Airplane* airplane)
{
}

void Step::ScheduleDoneServiceEM(Airplane* airplane)
{
}

//@TODO figure out what these are supposed to be... EM?
void Step::ReleaseServer()
{
}

void Step::DoneService()
{
}

void Step::Execute(Airplane* airplane){
	
}
