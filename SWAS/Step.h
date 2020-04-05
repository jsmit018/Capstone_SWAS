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

/**
 * Base class for Steps to be inherited by the unique Event Actions.
 */
class Step : public Task
{
public:
	/**
	 * Step constructor.
	 */
	Step(Distribution* serviceTime, string name);
	/**
	 * Iterates through vectors/nonstatic maps to use resource and parts and aircraft object copy constructors.
	 */
	void CopyMapStep(const Step& mapStep);
	/**
	 * Gives the current step to the next Event Method.
	 */
	void Execute(Aircraft* aircraft);
	/**
	 * Adds a resource to the current resource pool.
	 */
	static void AddToResPool(Resource* resource, string resourceName);
	/**
	 * Adds parts to the current parts pool.
	 */
	static void AddToPartsPool(Parts* parts, string partsName);
	/**
	 * Sets the name for the repair step.
	 */
	void SetName(string name);
	/**
	 * Sets the type of repair step.
	 */
	void SetType(string type);
	/**
	 * Gets the inspection failure probability. 
	 * Secod half is tuened into numbers of type double. 
	 * Gets the first half and find the distribution type and sets new probability. 
	 */
	void SetInspecFailProb(string failureProb);
	/**
	 * Pulls in the service time and splits input into halves. 
	 * Second half is turned into numbers of type double. 
	 * First half is used to find distribution type and sets new probability. 
	 */
	void SetServiceTime(string serviceTime); //change to distribution
	/**
	 * Iterates through "Steps" map of resources, finds all matching resources, and the attributes of each. 
	 * Sets the number of resources needed. 
	 */
	void SetReqResource(string reqResource);
	//void SetResNum(int numResNeeded); ///TODO read in and split
	/**
	 * Iterates through "Steps" map of parts, finds all matching parts, and the attributes of each. 
	 * Sets the number of parts needed. 
	 */
	void SetReqParts(string reqParts);
	/**
	 * Sets as the repair step that is going to be returned to.
	 * Used when inspection fails and needs to return to repair step.
	 */
	void SetReturnStep(/*int stepId*/ int returnStep);
	/**
	 * Sets a step as a indoor requirement.
	 */
	void SetStepIndoorReq(char indoorReq);
	/**
	 * Sets the steps ID.
	 */
	void SetStepID(int stepID);
	/**
	* Sets the Repair Job Priority
	*/
	void SetRJPriority(int RJpriority);
	/**
	 * Sets the Repair Jobs Name
	 */
	void SetMyRJName(string myRJ);
	/**
	 * Schedules the first repair job step once the aircraft enters the simulation. 
	 */
	void ScheduleFirstStep(Step* step, Aircraft* aircaft);
	/**
	 * Adds the Resource to the Resource Map.
	 * Sets the number of resources needed for the step and links it to the resource name. 
	 */
	void AddResource(Resource* resource, string resourceName, int numNeeded);
	/**
	 * Add parts tot he parts map.
	 */
	void AddParts(Parts* parts, string partsName);
	/**
	 * Prints the map of parts.
	 */
	void PrintParts();
	/**
	 * Prints the map of resources.
	 */
	void PrintResources();
	/**
	 * Prints the map of different resource pools.
	 */
	void PrintPools();
	/**
	 * Prints event aircraft underwent.
	 */
	void PrintEvent();
	/**
	* Checks the availability of a bay apon arrival. 
	 * There is currently a note stating "DISCONTINUED LOGIC".
	 */
	void InitialArrivalBayCheck();
	/**
	 * Locates the resource information on the resource map. 
	 */
	map<string, Resource*>::iterator FindResource(string resource);
	/**
	 * Locates the parts information on the parts map.
	 */
	map<string, Parts*>::iterator FindParts(string parts);
	/**
	 * Condition YES/NO if looking at the end of the resource map. 
	 */
	bool IsResourceMapEnd(map<string, Resource*>::iterator it);
	/**
	 * Condition YES/NO is the inspection failed. 
	 */
	bool IsInpectionFail(Distribution* inspecFailProb);
	/**
	 * Condition YES/NO if looking at the end of the parts map. 
	 */
	bool IsPartsMapEnd(map<string, Parts*>::iterator it);
	/**
	 * Condition YES/NO if resource is released. 
	 */
	bool IsResourceReleased(map<string, Resource*>::const_iterator iter, int newCount);
	/**
	 * Returns name
	 */
	string GetName();
	//Time GetServiceTime();
	/**
	 * Returns the current number in queue
	 */
	int GetNumberInQueue();
	/**
	 * returns the repair job priority. 
	 */
	int GetRJPriority();
	/**
	 * Returns repair job indoor requirement.
	 */
	int GetRJIndoorReq();
	/**
	 * Returns a service time.
	 */
	Distribution* GetServiceTime();
	/**
	 * Call to find and return the resource.
	 */
	Resource* GetResourceObj(string name);
	/**
	 * Returns teh current repair job name.
	 */
	string GetMyRJName();
	/**
	 * Returns the start of the Parts map.
	 */
	map<string, Parts*>::iterator GetPartsMapBegin();
	/**
	 * Returns the end of the Parts map.
	 */
	map<string, Parts*>::iterator GetPartsMapEnd();
	/**
	 * Returns the start of the Resource map.
	 */
	map<string, Resource*>::iterator GetResourceMapBegin();
	/**
	 * Returns the end of the Resource map. 
	 */
	map<string, Resource*>::iterator GetResourceMapEnd();

	//For Testing
	//----------------------
	void ScheduleFirstRecurringStep(Step* step, Aircraft* aircraft);
	void SceduleCalendarStep(Step* step, Aircraft* aircraft, CalendarObj* calobj);
	//----------------------

	/*void AcquireBayEM();					// check bay avail, grab bay if avail - effectively decrementing bay - give reference of bay resource
	void AddQueueEM();						// if bay not avial, increment queue
	void ScheduleDoneStepEM();*/			// if done with step, see if there's another step, if there is, check resources. if any same, keep, if not, release. if next step in, keep bay, if out, release bay
	
	/**
	 * Print function to display information on the specific repair step.
	 */
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