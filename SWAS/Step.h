#pragma once
#include "SimExec.h"
#include "Task.h"
#include <map>
#include "Resource.h"
#include "Parts.h"
#include "PriorityQueue.h"
#include "Scribe.h"

class Step : public Task
{
public:
	Step(Distribution* serviceTime, string name);
	void CopyMapStep(const Step& mapStep);
	void Execute(Aircraft* aircraft);

	static void AddToResPool(StepResource* resource, string resourceName);
	static void AddToPartsPool(Parts* parts, string partsName);
	static void SetResPoolCount(string, double);
	static void SetPartPoolCount(string, int);
	static void PrintPools();

	static map<string, Parts*> GetPartsPool();
	static map<string, StepResource*> GetResPool();
	static map<string, StepResource*>::iterator GetResourcePoolBegin();
	static map<string, StepResource*>::iterator GetResourcePoolEnd();
	static int GetPartsPoolSize();
	static int GetResPoolSize();

	void SetName(string name);
	void SetType(string type);
	void SetInspecFailProb(string failureProb);
	void SetServiceTime(string serviceTime);
	void SetReqResource(string reqResource);
	void SetReqParts(string reqParts, int numNeeded);
	void SetReturnStep(int returnStep);
	void SetStepIndoorReq(char indoorReq);
	void SetStepBayReq(char baySizeReq);
	void SetStepID(int stepID);
	void SetRJPriority(int RJpriority);
	void SetMyRJName(string myRJ);

	void InitialArrivalBayCheck();
	void ScheduleFirstStep(Step* step, Aircraft* aircaft);
	void ReleaseBay(StepResource*, string, string, int);

	void AddResource(StepResource* resource, string resourceName, int numNeeded);
	void AddParts(Parts* parts, string partsName, int numNeeded);

	void PrintParts();
	void PrintResources();

	map<string, StepResource*>::iterator FindResource(string resource);
	map<string, Parts*>::iterator FindParts(string parts);

	bool IsResourceMapEnd(map<string, StepResource*>::iterator it);
	bool IsInpectionFail(Distribution* inspecFailProb);
	bool IsPartsMapEnd(map<string, Parts*>::iterator it);
	bool IsResourceReleased(map<string, StepResource*>::const_iterator iter, int newCount);
	bool IsMyBaySizeAvailable(string baySize);
	bool AreThereBaysAvailable(string baySize);
	bool WasBayAcquired(string bayName);
	static bool isFirstShiftChange();

	string GetName();
	char GetRJIndoorReq();
	int GetNumberInQueue();
	int GetRJPriority();
	int GetStepID();
	int GetResMapSize();
	int GetPartsMapSize();
	int GetReturnStep();

	static void UpdateShift();
	static void ScheduleFirstWartimeShifts();
	static void ScheduleFirstPeacetimeShifts();
	static void SetFirstShiftUpdateFlag(int flag);
	
	Distribution* GetServiceTime();
	StepResource* GetResourceObj(string name);
	Parts* GetPartsObj(string name);
	
	string GetMyRJName();
	string AcquireBay(StepResource*, int);

	map<string, Parts*>::iterator GetPartsMapBegin();
	map<string, Parts*>::iterator GetPartsMapEnd();
	map<string, StepResource*>::iterator GetResourceMapBegin();
	map<string, StepResource*>::iterator GetResourceMapEnd();
	map<string, StepResource*>::iterator FindResourceinReqResMap(string resource);


	//For Testing
	//----------------------
	void ScheduleFirstRecurringStep(Step* step, Aircraft* aircraft);
	void ScheduleCalendarStep(Step* step, Aircraft* aircraft, CalendarObj* calobj, int i);
	//----------------------

	void Print();
private:

	static int _firstShiftUpdateFlag;

	string _myRJ;
	string _name;
	string _type;
	string _reqRes;										//requiredResources
	string _reqParts;									//requiredParts
	string _baySizeReq;

	char _indoorReq;

	int _returnStep;
	int _RJpriority;
	int _stepID;
	int _numInQueue;
	int _failCount = 0;

	Step* _nextStep;
	Distribution* _inspecFailProb;
	Distribution* _servTime;
	
	map<string, StepResource*> _reqResourceMap;			//map of parts required by this step
	map<string, Parts*> _reqPartsMap;					//map of parts required by this step
	map<string, int> _acquiredResources;				//map of acquired resources to be checked at the end of service
	
	PriorityQueue<Aircraft>* _priorityQueue;

	bool ResourceInReqResource(string resource)
	{
		if (_reqResourceMap.find(resource) == _reqResourceMap.end())
			return false;
		else if (_reqResourceMap.find(resource) != _reqResourceMap.end())
			return true;
	}

	static map<string, StepResource*> _resourcePool;	//static map of all unique resources for keeping track of counts available
	static map<string, Parts*> _partsPool;				//static map of all unique parts for keeping track of counts available
		
	//event actions
	class StartServiceEA;
	class StartRepairServiceEA;
	class AddQueueEA;
	class DoneServiceEA;
	class DoneResourceServiceEA;
	class PlaceOrderEA;
	class OrderArrivalEA;
	class AcquireResourceEA;
	class ReleaseResourceEA;
	class FailResourceEA;
	class RestoreResourceEA;
	class WaitForResourceEA;
	class ResWaitForResEA;
	class NeedPartsEA;
	class ResNeedPartsEA;
	class NeedBaysEA;
	class ShiftChangeEA;

	//event methods
	void PlaceOrderEM(Parts* parts);
	void OrderArrivalEM(Parts* parts);
	void StartServiceEM(Aircraft* aircraft, map<string, int> acquiredResources);
	void StartRepairServiceEM(StepResource* resource, map<string, int> acquiredResources);
	void AddQueueEM(Aircraft* aircraft);
	void DoneServiceEM(Aircraft* aircraft, map<string, int> acquiredResources);
	void DoneResourceServiceEM(StepResource* resource, map<string, int> acquiredResources);
	void AcquireResourceEM(StepResource* resource, int numNeeded);
	void ReleaseResourceEM(StepResource* resource, int numRelease);
	void FailResourceEM(StepResource* resource);
	void RestoreResourceEM(StepResource* resource);
	void AcquireParts(Parts* parts, int newCount);
};