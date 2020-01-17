#pragma once
#include "SimObj.h"
#include "Task.h"

class Step : public Task
{
public:
	Step();
	VirtualExecutive();
	Time GetServiceTime();
	void SetServiceTime(Time serviceTime);
	string GetName();
	void SetName(string name);
	int GetNumberInQueue();
	void SetNumberInQueue(int NumberInQueue);
	void AcquireServerEM();
	void AddQueueEM();
	void ScheduleDoneServiceEM();
	void ReleaseServer();
	void DoneService();
private:
	Time serviceTime;
	string _name;
	int NumberInQueue;
};