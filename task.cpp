#include "task.h"

void Task::SetNextTask(Task* nextTask)
{
	_nextTask = nextTask;
}

void Task::Arrive(Aircraft* aircraft)
{
	cout << "time = " << GetSimulationTime() << ", " << _name << ", " << "arrive, Entity: " << entity->GetID()/*entityID here*/ << endl;
	Execute(aircraft);
}

void Task::SetName(string name)
{
	_name = name;
}

string Task::GetName()
{
	return string();
}

int Task::GetID()
{
	return 0;
}

Task::Task(string name)
{
	_name = name;
}

void Task::Depart(Aircraft* aircraft)
{
	cout << "time = " << GetSimulationTime() << ", " << _name << ", " << "depart, Entity: " << entity->GetID()/*Entity ID Here*/ << endl;
	_nextTask->Arrive(aircraft);
}
