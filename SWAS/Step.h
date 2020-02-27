#pragma once
#include "SimObj.h"
#include "Task.h"
#include <map>
#include "Distribution.h"
#include "Resource.h"

class Step : public Task
{
public:
	Step(string serviceTime, string name);
	void Execute(Aircraft* aircraft);

	void SetName(string name);
	void SetType(string type);
	void SetInspecFailProb(string failureProb);
	void SetServiceTime(string serviceTime);
	void SetReqResource(string reqResource);
	void SetReqParts(string reqParts);
	void SetReturnStep(int stepId);
	//void SetServiceTime(Time serviceTime);
	//void SetNumberInQueue(int numInQueue);

	string GetName();
	//Time GetServiceTime();
	int GetNumberInQueue();

	/*void AcquireServerEM();
	void AddQueueEM();
	void ScheduleDoneServiceEM();*/
	void ReleaseServer();
	void DoneService();
	void Print();
private:
	Distribution* _serviceTime;
	//map<int, Aircraft*, greater<int>> _PriorityQueue;
	//map<int, Aircraft*, greater<int>>::iterator _iterator;
	string _name;
	int _numInQueue;
	Step* _nextStep;
	Resource* _bays;	//determined by Warehouse GUI
	string _type;
	string _inspecFailProb;
	string _servTime;
	string _reqRes;
	string _reqParts;
	int _returnStep;		// Maybe this should be a pointer to the step instead of its "id"


	class AcquireServerEA;
	class AddQueueEA;
	class ScheduleDoneServiceEA;
	void AcquireServerEM(Aircraft* aircraft);
	void AddQueueEM(Aircraft* aircraft);
	void ScheduleDoneServiceEM(Aircraft* aircraft);
};
