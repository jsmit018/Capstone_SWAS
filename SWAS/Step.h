//Step.h: Andrea Robey
/*The step class is responsible for all system flow logic once an aircraft's first step of its first 
repair job begins. Step are broken up in to start service and done service events. 

The first step start service invoked on each aircraft will initiate acquiring of an appropriate bay if 
it's necessary. When successful, the acquisition of all required resources is initiated. If any required
resource or part is unavailabe, this start service event will be placed in a conditional event list in 
simulation executive and await the release of that particular type of resource or part before re starting 
the step. If it had acquired any other resources before finding another required resource or a part is 
unavailable, it will have a higher priority for grabbing a released resource. 

The action of releasing a resource or an arrival of parts event triggers the conditional event list to 
execute the start service with the highest priority that is waiting on the type of resource that was 
released or part that was replenished. 

Current counts of available resources and parts are tracked and updated in the resource pool and parts
pool, respectively. When the count of parts reaches the threshold, an order parts event is scheduled.
This event schedules an order arrival event in the designated lead time. The counts of resources is also
affected by changes in shifts. 

To easily emulate the buffer time needed in real life where shifts begin
but resources (personnel) are not readily available right away, the input sheet could be modified to 
start shifts at any time. For example, if the first shift's personnel are usually more punctual, resources 
could be available at 600, and if the second shift typically has some lag, the shift start (which really just
equates to the change in number available) could start at 630. To emulate a flux, another "shift" time can be
added to the input sheet in between typical working shifts as an intermediate if the desire is to model when
some personnel are typically late. 

All acquired resource names are put into a list passed from start service to the done service and back to 
the start service of the next step. If it is not the first start service for the first step for the first 
job of the aircraft (nothings been acquired), start service checks the list of acquired resources that was 
passed from the previous step and compares it to the current step's list of required resources. Acquisition 
(only acquisition) of the necessary types (including bays) or amounts of currently acquired resources is 
handled. 

If the step is an inspection, the dice are rolled for whether or not it will fail. If it fails, a flag is 
set. Alternatively, if its a passed inspection or if a process step is complete, a done service will be 
scheduled. 

Done service handles scheduling the next step or departing the aircraft and handling resource releases 
accordingly. If the current step is an inspection that failed, the next step will be the designated return 
step. This loop will continue until the inspection is passed. Alternatively, if the current step is an inspection 
that has passed, or is a completed process, the logic branches into three
possibilities:

1. This is the last step of the last repair job in the aircraft's to-do list. If this is the case, all 
resources are released (including the bay) and the aircraft departs the system.

2. This is the last step of a repair job but there are more repair jobs, the next step's start service is 
scheduled. In this case, the next step is the first step of the next repair job based on highest priority).
If the next repair job requires a bay and so did the current job, the bay is retained but all other resources 
are released. Otherwise, the bay is also released.

3. This is not the last step in the repair job. In this case, the next step's start service is scheduled. 
Releasing (only releasing) of the unnecessary types or amounts (if the type but not number is needed) is
handled. Future development may involve modifying the check to compare the acquired resource list to the
required list of steps further down the line to retain specific resources.

Steps for repair jobs scheduled by the failures of resources are handled very similarly.
*/

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

	void ScheduleFirstStep(Step* step, Aircraft* aircaft);
	void ReleaseBay(StepResource*, string, string, int);

	void AddResource(StepResource* resource, string resourceName, int numNeeded);
	void AddParts(Parts* parts, string partsName, int numNeeded);

	void PrintParts();
	void PrintResources();

	static void ReadResources();

	static void ResetPools();

	map<string, StepResource*>::iterator FindResource(string resource);
	map<string, Parts*>::iterator FindParts(string parts);
	static map<string, StepResource*>::iterator GetReqResMapBegin();
	static map<string, StepResource*>::iterator GetReqResMapEnd();

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


	void ScheduleFirstRecurringStep(Step* step, Aircraft* aircraft);
	void ScheduleCalendarStep(Step* step, Aircraft* aircraft, CalendarObj* calobj, int i);

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
	void DoneServiceEM(Aircraft* aircraft, map<string, int> acquiredResources);
	void DoneResourceServiceEM(StepResource* resource, map<string, int> acquiredResources);
	void AcquireResourceEM(StepResource* resource, int numNeeded);
	void ReleaseResourceEM(StepResource* resource, int numRelease);
	void FailResourceEM(StepResource* resource);
	void RestoreResourceEM(StepResource* resource);
	void AcquireParts(Parts* parts, int newCount);
};