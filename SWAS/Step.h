#pragma once
//#include "SimObj.h"
#include "SimExec.h"
#include "Task.h"
#include <map>
//#include "Distribution.h"
#include "Resource.h"
#include "Parts.h"
#include "PriorityQueue.h"
//#include "Aircraft.h"

class Step : public Task
{
public:
	Step(Distribution* serviceTime, string name);
	void CopyMapStep(const Step& mapStep);
	void Execute(Aircraft* aircraft);

	static void AddToResPool(Resource* resource, string resourceName);
	static void AddToPartsPool(Parts* parts, string partsName);
	void SetName(string name);
	void SetType(string type);
	void SetInspecFailProb(string failureProb);
	void SetServiceTime(string serviceTime); //change to distribution
	void SetReqResource(string reqResource);
	//void SetResNum(int numResNeeded); ///TODO read in and split
	void SetReqParts(string reqParts);
	void SetReturnStep(/*int stepId*/ int returnStep);
	void SetStepIndoorReq(char indoorReq);
	void SetStepID(int stepID);
	void SetRJPriority(int RJpriority);
	Step* SetNextStep();
	void SetMyRJName(string myRJ);
	void ScheduleFirstStep(Step* step, Aircraft* aircaft);

	void AddResource(Resource* resource, string resourceName, int numNeeded);
	void AddParts(Parts* parts, string partsName);
	void PrintParts();
	void PrintResources();
	void PrintPools();
	void PrintEvent();
	void InitialArrivalBayCheck();
	map<string, Resource*>::iterator FindResource(string resource);
	map<string, Parts*>::iterator FindParts(string parts);
	bool IsResourceMapEnd(map<string, Resource*>::iterator it);
	bool IsInpectionFail(Distribution* inspecFailProb);
	bool IsPartsMapEnd(map<string, Parts*>::iterator it);
	bool IsResourceReleased(map<string, Resource*>::const_iterator iter, int newCount);
	string GetName();
	//Time GetServiceTime();
	int GetNumberInQueue();
	int GetRJPriority();
	int GetRJIndoorReq();
	Distribution* GetServiceTime();
	Resource* GetResourceObj(string name);
	string GetMyRJName();
	//	Step* GetNextStep(Aircraft * currAir, int currStep);
	map<string, Parts*>::iterator GetPartsMapBegin();
	map<string, Parts*>::iterator GetPartsMapEnd();
	map<string, Resource*>::iterator GetResourceMapBegin();
	map<string, Resource*>::iterator GetResourceMapEnd();

	/*void AcquireBayEM();					// check bay avail, grab bay if avail - effectively decrementing bay - give reference of bay resource
	void AddQueueEM();						// if bay not avial, increment queue
	void ScheduleDoneStepEM();*/			// if done with step, see if there's another step, if there is, check resources. if any same, keep, if not, release. if next step in, keep bay, if out, release bay
	void Print();
private:
	string _myRJ;
	Distribution* _serviceTime;
	//map<int, Aircraft*, greater<int>> _PriorityQueue;	//priority queue map -- maybe vector if priorities are same
	string _name;
	char _indoorReq; /// this is not populated right now
	int _RJpriority;
	int _stepID;
	int _numInQueue;
	Step* _nextStep;	// NOT POPULATED
	//	Resource* _bays;		//determined by Warehouse GUI
	string _type;
	Distribution* _inspecFailProb;
	Distribution* _servTime;
	string _reqRes;			//break up into two parts, store into map
	string _reqParts;		//break up into two parts, store into map
	int _returnStep;		// Maybe this should be a pointer to the step instead of its "id"
	map<string, Resource*> _reqResourceMap;		//map of required resources
	map<string, Parts*> _reqPartsMap;		//map of required parts
	vector<string> _acquiredResources;	//vector of acquired resources to be checked at the end of service
	PriorityQueue<Aircraft>* _priorityQueue;

	bool haveAllResources();	//check for whether acquired resources can be released
	bool ResourceInReqResource(string resource)
	{
		if (_reqResourceMap.find(resource) == _reqResourceMap.end())
			return false;
		return true;
	}

	/// to do //
	static map<string, Resource*> _resourcePool;
	static map<string, Parts*> _partsPool;

	class StartServiceEA;
	class AddQueueEA;
	class DoneServiceEA;
	class PlaceOrderEA;
	class OrderArrivalEA;
	class AcquireResourceEA;
	class ReleaseResourceEA;
	class FailResourceEA;
	class RestoreResourceEA;
	class WaitForResourceEA;
	class NeedPartsEA;

	void PlaceOrderEM(Parts* parts);
	void OrderArrivalEM(Parts* parts);
	void StartServiceEM(Aircraft* aircraft, vector<string> acquiredResources);
	void AddQueueEM(Aircraft* aircraft);
	void DoneServiceEM(Aircraft* aircraft, vector<string> acquiredResources);
	void AcquireResourceEM(Resource* resource);
	void ReleaseResourceEM(Resource* resource);
	void FailResourceEM(Resource* resource);
	void RestoreResourceEM(Resource* resource);

	void AcquireParts(Parts* parts);
};