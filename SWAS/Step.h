#pragma once
//#include "SimObj.h"
#include "SimExec.h"
#include "Task.h"
#include <map>
//#include "Distribution.h"
#include "Resource.h"
#include "Parts.h"
#include "PriorityQueue.h"
#include "Scribe.h"
//#include "Aircraft.h"

class Step : public Task
{
public:
	Step(Distribution* serviceTime, string name); 
	void CopyMapStep(const Step& mapStep); 
	void Execute(Aircraft* aircraft); 

	static void AddToResPool(StepResource* resource, string resourceName); 
	static void AddToPartsPool(Parts* parts, string partsName); 
	static map<string, Parts*> GetPartsPool(); 
	static map<string, StepResource*> GetResPool(); 
	static int GetPartsPoolSize(); 
	static int GetResPoolSize(); 
	static void SetResPoolCount(string, double); 
	static void SetPartPoolCount(string, int); 
	void SetName(string name); 
	void SetType(string type); 
	//void SetNumOfParts(string partsName, int numNeeded); 
	void SetInspecFailProb(string failureProb); 
	void SetServiceTime(string serviceTime);  //change to distribution
	void SetReqResource(string reqResource/*, Resource* resource*/); 
	//void SetReqParts(string reqParts); 
	void SetReqParts(string reqParts, int numNeeded); 
	void SetReturnStep(/*int stepId*/ int returnStep); 
	void SetStepIndoorReq(char indoorReq); 
	void SetStepBayReq(char baySizeReq); 
	void SetStepID(int stepID); 
	void SetRJPriority(int RJpriority); 
	void SetMyRJName(string myRJ); 
	void ScheduleFirstStep(Step* step, Aircraft* aircaft); 
	void ReleaseBay(StepResource*, string, string, int); 

	void AddResource(StepResource* resource, string resourceName, int numNeeded); 
	void AddParts(Parts* parts, string partsName, int numNeeded); 
	void PrintParts(); 
	void PrintResources(); 
	static void PrintPools(); 
	//void PrintEvent(); 
	void InitialArrivalBayCheck(); 
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
	//Time GetServiceTime(); 
	int GetNumberInQueue(); 
	int GetRJPriority(); 
	char GetRJIndoorReq(); 
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
	//void ScheduleResourceFailure(); 


	//For Testing
	//----------------------
	void ScheduleFirstRecurringStep(Step* step, Aircraft* aircraft); 
	void ScheduleCalendarStep(Step* step, Aircraft* aircraft, CalendarObj* calobj, int i); 
	//----------------------

	/*void AcquireBayEM(); 					// check bay avail, grab bay if avail - effectively decrementing bay - give reference of bay resource
	void AddQueueEM(); 						// if bay not avial, increment queue
	void ScheduleDoneStepEM(); */			// if done with step, see if there's another step, if there is, check resources. if any same, keep, if not, release. if next step in, keep bay, if out, release bay
	void Print(); 
private:
	static int _firstShiftUpdateFlag; 
	string _myRJ; 
	Distribution* _serviceTime; 
	//map<int, Aircraft*, greater<int>> _PriorityQueue; 	//priority queue map -- maybe vector if priorities are same
	string _name; 
	char _indoorReq; 
	string _baySizeReq; 
	int _RJpriority; 
	int _stepID; 
	int _numInQueue; 
	int _failCount = 0; 
	Step* _nextStep; 	// NOT POPULATED
	//	Resource* _bays; 		//determined by Warehouse GUI
	string _type; 
	Distribution* _inspecFailProb; 
	Distribution* _servTime; 
	string _reqRes; 			//break up into two parts, store into map
	string _reqParts; 		//break up into two parts, store into map
	int _returnStep; 		// Maybe this should be a pointer to the step instead of its "id"
	map<string, StepResource*> _reqResourceMap; 		//map of required resources
	map<string, Parts*> _reqPartsMap; 		//map of required parts

	//vector<string> _acquiredResources; 	//vector of acquired resources to be checked at the end of service
	map<string, int> _acquiredResources; 	//vector of acquired resources to be checked at the end of service
	PriorityQueue<Aircraft>* _priorityQueue; 
	//Scribe* outputRecorder = new Scribe(); 

	bool haveAllResources(); 	//check for whether acquired resources can be released
	bool ResourceInReqResource(string resource)
	{
		if (_reqResourceMap.find(resource) == _reqResourceMap.end())
			return false; 
		else if (_reqResourceMap.find(resource) != _reqResourceMap.end())
			return true; 
	}

	/// to do //
	static map<string, StepResource*> _resourcePool; 
	static map<string, Parts*> _partsPool; 

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
	//void ShiftChangeEM(); 
	void AcquireParts(Parts* parts, int newCount); 
}; 