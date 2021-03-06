//Task.cpp: Provided by Dr. James Leathrum

#include "task.h"
#include "SimExec.h"
#include "Aircraft.h"

void Task::SetNextTask(Task* nextTask)
{
	_nextTask = nextTask;
}

void Task::Arrive(Aircraft* aircraft)
{
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
	aircraft->SetSource(this->GetID());
	_nextTask->Arrive(aircraft);
}