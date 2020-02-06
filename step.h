#pragma once
#include "SimObj.h"
#include "Task.h"

class Step : public Task
{
public:
	Step(Distribution* serviceTime, string name);
	void Execute(Airplane* airplane);
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
	map<int, Aircraft*, greater<int>> _PIFOQueue;
	//map<int, Aircraft*, greater<int>>::iterator _iterator;
	Distribution* _serviceTime;
	string _name;
	int _numberInQueue;
	Step* _nextStep;
	
	class AcquireServerEA;
	class AddQueueEA;
	class ScheduleDoneServiceEA;
	void AcquireServerEM(Airplane* airplane);
	void AddQueueEM(Airplane* airplane);
	void ScheduleDoneServiceEM(Airplane* airplane);
};
