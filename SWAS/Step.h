#pragma once
#include "SimObj.h"
#include "Task.h"
#include <map>
#include "Distribution.h"
#include "Resource.h"
#include "Parts.h"

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
	void SetReturnStep(/*int stepId*/ int returnStep);
	void AddResource(Resource* resource, string resourceName);
	void AddParts(Parts* parts, string partsName);


	string GetName();
	//Time GetServiceTime();
	int GetNumberInQueue();
	Resource* GetResourceObj(string name);

	/*void AcquireBayEM();					// check bay avail, grab bay if avail - effectively decrementing bay - give reference of bay resource
	void AddQueueEM();						// if bay not avial, increment queue
	void ScheduleDoneStepEM();*/			// if done with step, see if there's another step, if there is, check resources. if any same, keep, if not, release. if next step in, keep bay, if out, release bay
	void Print();
private:
	Distribution* _serviceTime;
	//map<int, Aircraft*, greater<int>> _PriorityQueue;	//priority queue map -- maybe vector if priorities are same
	string _name;
	int _numInQueue;
	Step* _nextStep;
//	Resource* _bays;		//determined by Warehouse GUI
	string _type;
	string _inspecFailProb;
	string _servTime;
	string _reqRes;			//break up into two parts, store into map
	string _reqParts;		//break up into two parts, store into map
	int _returnStep;		// Maybe this should be a pointer to the step instead of its "id"
	map<string, Resource*> _reqResourceMap;		//map of required resources
	map<string, Parts*> _reqPartsMap;		//map of required parts
	//vector<Resource*> _reqResourceVec;
	//vector<Parts*> _reqPartsVec;
	vector<string> _acquiredResources;	//vector of acquired resources to be checked at the end of service

	bool haveAllResources();	//check for whether acquired resources can be released
	

	class StartServiceEA;
	class AddQueueEA;
	class ScheduleDoneServiceEA;
	class AcquireResourceEA;
	class ReleaseResourceEA;

	void StartServiceEM(Aircraft* aircraft, vector<string> _acquiredResources);
	void AddQueueEM(Aircraft* aircraft);
	void ScheduleDoneServiceEM(Aircraft* aircraft);
	void AcquireResourceEM(Resource* resource);
	void ReleaseResourceEM(Resource* resource);
};