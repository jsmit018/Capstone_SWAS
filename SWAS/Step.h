#pragma once
#include "SimObj.h"
#include "Task.h"
#include <map>
#include "Distribution.h"
#include "Resource.h"

class Step : public Task
{
public:
	Step(Distribution* serviceTime, string name, Resource* bays);
	void Execute(Aircraft* aircraft);
	//Time GetServiceTime();
	//void SetServiceTime(Time serviceTime);
	string GetName();
	void SetName(string name);
	int GetNumberInQueue();
	//void SetNumberInQueue(int NumberInQueue);
	/*void AcquireServerEM();
	void AddQueueEM();
	void ScheduleDoneServiceEM();*/
	void ReleaseServer();
	void DoneService();
private:
	Distribution* _serviceTime;
	map<int, Aircraft*, greater<int>> _PriorityQueue;
	map<int, Aircraft*, greater<int>>::iterator _iterator;
	string _name;
	int _numberInQueue;
	Step* _nextStep;
	Resource* _bays;

	class AcquireServerEA;
	class AddQueueEA;
	class ScheduleDoneServiceEA;
	void AcquireServerEM(Aircraft* aircraft);
	void AddQueueEM(Aircraft* aircraft);
	void ScheduleDoneServiceEM(Aircraft* aircraft);
};
