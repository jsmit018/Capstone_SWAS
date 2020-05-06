//Task.h: Provided by Dr. James Leathrum

#pragma once
#include "SimExec.h"

class Aircraft; 

using namespace std; 

class Task
{
public:
	void SetNextTask(Task* nextTask); 
	void Arrive(Aircraft* aircraft); 
	void SetName(string name); 
	string GetName(); 
	int GetID(); 
protected:
	Task* _nextTask; 
	string _taskType; 
	Task(string name); 
	void Depart(Aircraft* aircraft); 
	virtual void Execute(Aircraft* aircraft) = 0; 
private:
	string _name; 
	int _id; 
	static int _nextID; 
}; 