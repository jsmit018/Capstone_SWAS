#pragma once
#include "SimObj.h"

using namespace std;

class Task
{
public:
	void SetNextTask(Task *nextTask);
	void Arrive(Entity *entity);
	void SetName(string name);
	string GetName();
	int GetID();
protected:
	Task * _nextTask;
	string _taskType;
	Task(string name);
	void Depart(Entity *entity);
	virtual void Execute(Entity *entity) = 0; 	
private:
	string _name;
	int _id;
	static int _nextID;
	Task* _nextTask;
};
