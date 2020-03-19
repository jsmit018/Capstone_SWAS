#include "Step.h"
#include "Distribution.h"
#include "SimExec.h"
//#include "Aircraft.h"
#include <iterator>
#include <map>


map<string, Resource*> Step::_resourcePool;
map<string, Parts*> Step::_partsPool;

Step::Step(string serviceTime, string name) : Task(name)
{
	_servTime = serviceTime;
	_name = name;
	_priorityQueue = new PriorityQueue<Aircraft>("Priority Queue");
}

////////////////////////////////////////////
/////////EVENT ACTIONS AND METHODS//////////
////////////////////////////////////////////

class Step::NeedResourcesEA : public CondEventAction {
public:
	NeedResourcesEA(Step* step, Resource* resource, Aircraft* aircraft, int amountNeeded, vector<string> acqResources) {
		_step = step;
		_resource = resource;
		_aircraft = aircraft;
		_amountNeeded = amountNeeded;
		_acqResources = acqResources;
	}

	bool Condition(Resource* resource, Parts* parts) {
		if (resource == _resource) {
			if (_resource->GetResourceCount() > _amountNeeded)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	void Execute() {
		_step->StartServiceEM(_aircraft, _acqResources);
	}

private:
	Resource* _resource;
	Step* _step;
	Aircraft* _aircraft;
	int _amountNeeded;
	string _funcName;
	vector<string> _acqResources;

};

class Step::NeedPartsEA : public CondEventAction {
public:
	NeedPartsEA(Step* step, Parts* parts, Aircraft* aircraft, int amountNeeded, vector<string> acqResources) {
		_step = step;
		_parts = parts;
		_aircraft = aircraft;
		_amountNeeded = amountNeeded;
		_acqResources = acqResources;
	}

	bool Condition(Resource* resource, Parts* parts) {
		if (parts == _parts) {
			if (_parts->GetPartsCount() > _amountNeeded)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	void Execute() {
		_step->StartServiceEM(_aircraft, _acqResources);
	}

private:
	Parts* _parts;
	Step* _step;
	Aircraft* _aircraft;
	int _amountNeeded;
	vector<string> _acqResources;
};

// PLACE ORDER/ REPLENISH order process
	// schedule an event that will act as "place order"   -> placeorderEA does placeOrderEM
	// "place order" execution will schedule a "order arrived/ parts replenish event"	 placeOrderEM schedules partsArrivalEA
	// "order arrived/ parts replenish event" execution will simply do _partsCount + Number ordered -> partsArrivalEA does partsArrivalEM

class Step::PlaceOrderEA : public EventAction
{
public:
	PlaceOrderEA(Step* step, Parts* parts) {
		_parts = parts;
		_step = step;
	}

	void Execute() {
		_step->PlaceOrderEM(_parts);
	}
private:
	Step* _step;
	Parts* _parts;
};

class Step::OrderArrivalEA : public EventAction
{
public:
	OrderArrivalEA(Step* step, Parts* parts)
	{
		_parts = parts;
		_step = step;
	}
	void Execute()
	{
		_step->OrderArrivalEM(_parts);
	}
private:
	Step* _step;
	Parts* _parts;
};

void Step::PlaceOrderEM(Parts* parts)
{
	//is creating an instance of the event action how i pass the 
	//event action in the schedule at function?
//	PlaceOrderEA* ea;
	SimExec::ScheduleEventAt(1, new OrderArrivalEA(this, parts), parts->GetLeadTime()->GetRV(), "OrderArrivalEA");
}

void Step::OrderArrivalEM(Parts* parts)
{
	//Logic: new count = current parts count + (initial count - current parts count)
	int newCount;

	map<string, Parts*>::const_iterator iter = _reqPartsMap.find(parts->GetPartsName());
	newCount = iter->second->GetPartsCount();
	
	parts->SetPartsCount(newCount);
	SimExec::CheckConditionalEvents(0, parts);
}


class Step::StartServiceEA : public EventAction {
public:
	StartServiceEA(Step* step, Aircraft* aircraft, vector<string> acqResources) {
		_step = step;
		_aircraft = aircraft;
		_acqResources = acqResources;
	}

	void Execute() {
		_step->StartServiceEM(_aircraft, _acqResources);
	}
private:
	Step* _step;
	Aircraft* _aircraft;
	vector<string> _acqResources;
};

class Step::ScheduleDoneServiceEA : public EventAction {
public:
	ScheduleDoneServiceEA(Aircraft* aircraft)
	{
		_aircraft = aircraft;
	}

	void Execute()
	{
	}
private:
	Aircraft* _aircraft;
};

void Step::StartServiceEM(Aircraft* aircraft, vector<string> _acquiredResources)
{
	//check if acquired resource vector has bay or spot
	bool hasResource = false;

	for (int i = 0; i < _acquiredResources.size(); i++)
	{
		if (_indoorReq == 'Y' || _indoorReq == 'y')
		{
			if (_acquiredResources[i] == "bay")
			{
				hasResource = true;
				break;
			}
		}
		else
		{
			if (_acquiredResources[i] == "outspot")
			{
				hasResource = true;
				break;
			}
		}
	}

	// mechanics

	// reading in a number of initial count
	// whenever a step start service happens I decrement that number
	// 1 instance of mechanic -> restrict, only allow one instance (singleton)

	// all resources can only have one instance 

	// One instance of each resource

	//aircraft arrives, look up repair jobs or get them, look at the mastermap I have these steps,
	//what resources do I need, get the resources for each step, execute step, go to next step

	// if each resource that can ever be used has one instance that is created in main, how do you access
	// and how do you modify these values of that Resource;

	//step has a static container of resources and pools that is populated at initialization
	// container is a container of resource object pointers, one for each possible resource

	// if i don't have a bay or outside spot
	if (hasResource == false)
	{
		// if inside step
		if (_indoorReq == 'Y' || _indoorReq == 'y')
		{
			map<string, Resource*>::iterator it = _resourcePool.find("bay");
			if (it->second->GetResourceCount() > 0)
			{
				// call decrement function, push it to acquired vector 
				_acquiredResources.push_back(it->first);
			}
			else
				cout << "we have to wait for a bay \n";
			// INSERT CODE FOR WAITING
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedResourcesEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
		}
		else
		{
			map<string, Resource*>::iterator it = _resourcePool.find("outspot");
			if (it->second->GetResourceCount() > 0)
			{
				// call decrement function, push it to acquired vector 
				_acquiredResources.push_back(it->first);
			}
			else
				cout << "we have to wait for an outspot \n";
			// INSERT CODE FOR WAITING
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedResourcesEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
		}
	}

	// number of bays initialized with simulation initialization from GUI, where are they stored (aka how does Step
	// get access to this information), how do I check if a bay is available -- determined by integration 

	if (_name == "process")
	{
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		//for all resources listed in required map
		while (iter != _reqResourceMap.end())
		{
			//compare key to acquired resources vector
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				//if it's already acquired, break
				if (iter->first == _acquiredResources[i])
					break;
			}

			map<string, Resource*>::iterator it = _resourcePool.find(iter->first);

			//otherwise if not acquired yet
			//if resource count is greater than number needed
			if (it->second->GetResourceCount() >= it->second->GetNumResNeeded())
			{
				//decrement appropriately
				int newCount;
				newCount = it->second->GetResourceCount() - it->second->GetNumResNeeded();
				it->second->SetResourceCount(newCount);

				//store in acquired resource vector
				_acquiredResources.push_back(it->first);
			}
			else
				cout << " we have to wait for a/an " << it->first << endl;
			//INSERT WAITING LOGIC
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedResourcesEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
			iter++;
		}

		map<string, Parts*>::const_iterator iter2 = _reqPartsMap.begin();
		
		//for all parts listed in required map
		while (iter2 != _reqPartsMap.end())
		{
			map<string, Parts*>::iterator it = _partsPool.find(iter2->first);

			if (it->second->AreEnoughParts() == true) {
				cout << "decrementing parts" << endl;

				int newCount;
				newCount = it->second->GetPartsCount() - it->second->GetNumPartsNeeded();

				it->second->SetPartsCount(newCount);
			}

			else if (it->second->AreEnoughParts() == false) {
				SimExec::ScheduleEventAt(1, new PlaceOrderEA(this, it->second), it->second->GetLeadTime()->GetRV(), "PlaceOrderEA");
				//INSERT WAITING LOGIC
				SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedPartsEA(this, it->second, aircraft, it->second->GetNumPartsNeeded(), _acquiredResources));
			}

				iter2++;
		}

		//SimExec::ScheduleEventAt(1, /*done step ea*/, /*distribution for step duration*/)
		SimExec::ScheduleEventAt(1, new ScheduleDoneServiceEA(aircraft), this->_serviceTime->GetRV(), "ScheduleDoneServiceEA");
	}

	else if (_name == "inspection")
	{
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		//for all resources listed in required map
		while (iter != _reqResourceMap.end())
		{
			//compare key to acquired resources vector
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				//if it's already acquired, break
				if (iter->first == _acquiredResources[i])
					break;
			}

			map<string, Resource*>::iterator it = _resourcePool.find(iter->first);

			//otherwise if not acquired yet
			//if resource count is greater than number needed
			if (it->second->GetResourceCount() >= it->second->GetNumResNeeded())
			{
				//decrement appropriately
				int newCount;
				newCount = it->second->GetResourceCount() - it->second->GetNumResNeeded();
				it->second->SetResourceCount(newCount);

				//store in acquired resource vector
				_acquiredResources.push_back(it->first);
			}
			else
				cout << " we have to wait for a/an " << it->first << endl;
			//INSERT WAITING LOGIC
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedResourcesEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
			
			//TO DO:
			//if inspection results in failure = true
				//schedule service EA
			
			//else
				//SimExec::ScheduleEventAt(1, new ScheduleDoneServiceEA(aircraft), _serviceTime->GetRV(), "ScheduleDoneServiceEA"); **This will be the line to uncomment

			iter++;
		}


	}

}

void Step::ScheduleDoneServiceEM(Aircraft* aircraft)
{

	//TO DO:
	//check if there are more steps
		//if yes
			//get next step
	
			//if resources I have are needed for next step
				//keep
			//else
				//schedule release resource ea

			//if inside step
				//check if next step is inside
					//if no
						//release bay
					//if yes
						//keep bay, add to acquired resource vector

			//if outside step
				//check if next step is outside
					//if no
						//release spot
					//if yes
						//keep spot, add to acquired resource vector

		//if no
			//check if there are more repair jobs
				//if yes
					//get steps
				//if no
					//schedule aircraft departure ea
	//This is just a placeholder you may be able to clear an old iterator and just throw it there instead.
	vector<string> resourceList;
	SimExec::ScheduleEventAt(1, new StartServiceEA(this, _priorityQueue->GetEntity(), resourceList), 0.0, "ScheduleStartServiceEA");
	//I'm under the assumption that you'll be using an interator if not let me know when you finish and i'll come back and add the appropriate line
	//SimExec::CheckConditionalEvents(it->second, 0);
}


class Step::AddQueueEA : public EventAction {
public:
	AddQueueEA(Step* step, Aircraft* aircraft) {
		_step = step;
		_aircraft = aircraft;
	}

	void Execute() {
		_step->AddQueueEM(_aircraft);
	}

private:
	Step* _step;
	Aircraft* _aircraft;
};

void Step::AddQueueEM(Aircraft* aircraft)
{
	_priorityQueue->AddEntity(aircraft, aircraft->GetAircraftPriority());
	_numInQueue++;
	CheckBays();
}

class Step::ReleaseResourceEA : public EventAction {
public:
	ReleaseResourceEA(Step* step, Resource* resource) {
		_step = step;
		_resource = resource;
	}

	void Execute() {
		_step->ReleaseResourceEM(_resource);
	}
private:
	Step* _step;
	Resource* _resource;
};

void Step::ReleaseResourceEM(Resource* resource) {

	int newCount;

	map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() + iter->second->GetNumResNeeded();
	
	resource->SetResourceCount(newCount);

	IsResourceReleased(iter, newCount);

}

class Step::FailResourceEA : public EventAction {
public:
	FailResourceEA(Step* step, Resource* resource) {
		_step = step;
		_resource = resource;
	}

	void Execute() {
		_step->FailResourceEM(_resource);
	}
private:
	Step* _step;
	Resource* _resource;
};

void Step::FailResourceEM(Resource* resource) {
	int newCount;
	//Mark added this i'm not sure we need to create a new instance, but i'm just going to put a priority of 1 - Jordan
	//RepairJob* newJob;

	map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() - 1;
	resource->SetResourceCount(newCount);

	//SimExec::ScheduleEventAt(newJob->GetPriority(), new FailResourceEA(this, resource), iter->second->GetFailureDistr()->GetRV(), "New Repair Job");
	SimExec::ScheduleEventAt(1, new FailResourceEA(this, resource), this->_serviceTime->GetRV(), "FailResourceEA");
};
////////////////////////////////////////////
//////////////   BOOLEANS   ////////////////
////////////////////////////////////////////

// Checking if iterator points to the end of the map // 
// Utilized to check if an element exists in the map // 
bool Step::IsResourceMapEnd(map<string, Resource*>::iterator it)
{
	if (it == _reqResourceMap.end())
		return true;

	return false; 
}

// Checking if iterator points to the end of the map // 
// Utilized to check if an element exists in the map // 
bool Step::IsPartsMapEnd(map<string, Parts*>::iterator it)
{
	if (it == _reqPartsMap.end())
		return true;

	return false;
}

bool Step::IsResourceReleased(map<string, Resource*>::const_iterator iter, int newCount)
{
	if (iter->second->GetResourceCount() == newCount)
		return true;

	return false;
}


////////////////////////////////////////////
//////////////    GETTERS    ///////////////
////////////////////////////////////////////

string Step::GetName()
{
	return _name;
}

int Step::GetNumberInQueue()
{
	//return _numberInQueue;
	return 0;
}

Resource* Step::GetResourceObj(string name)
{
	map<string, Resource*>::iterator it = _reqResourceMap.find(name);
	if (it == _reqResourceMap.end())
		return nullptr;
	return it->second;
}

////////////////////////////////////////////
//////////////    SETTERS    ///////////////
////////////////////////////////////////////

void Step::SetName(string name)
{
	_name = name;
}

void Step::SetType(string type)
{
	_type = type;
}

void Step::SetInspecFailProb(string failureProb)
{

	istringstream failProb(failureProb);
	string firstHalf;
	string secHalf;

	getline(failProb, firstHalf, '(');
	getline(failProb, secHalf, ')');

	if (failureProb == "")
		return;

	//cout << "whole thing " << failureProb << endl;
	//cout << "first: " << firstHalf << endl;
	//cout << "second: " << secHalf << endl;

	istringstream nums(secHalf);
	if (firstHalf == "Triangular" || firstHalf == "Tri")
	{
		double min, expected, max;
		nums >> min;
		nums >> expected;
		nums >> max;
		_inspecFailProb = new Triangular(min, expected, max);
	}

	else if (firstHalf == "Exponential")
	{
		double mean;
		nums >> mean;

		_inspecFailProb = new Exponential(mean);
	}

	else if (firstHalf == "Uniform")
	{
		double min, max;
		nums >> min >> max;

		_inspecFailProb = new Uniform(min, max);
	}

	else if (firstHalf == "Normal")
	{
		double mean, stdev;
		nums >> mean >> stdev;

		_inspecFailProb = new Normal(mean, stdev);
	}

	else if (firstHalf == "Poisson")
	{
		double mean;
		nums >> mean;

		_inspecFailProb = new Poisson(mean);
	}

	else if (firstHalf == "Constant" || firstHalf == "Fixed")
	{
		double mean;
		nums >> mean;

		_inspecFailProb = new Constant(mean);
	}

	else if (firstHalf == "Weibull")
	{
		double scale, shape;
		nums >> scale >> shape;

		_inspecFailProb = new Weibull(scale, shape);
	}

	//Determines correct distribution and prints
	_inspecFailProb->PrintDistribution();
}

void Step::SetServiceTime(string serviceTime)
{
	_servTime = serviceTime;
}

void Step::SetReqResource(string reqResource)
{
	//change this to check for while finding &
	Resource* newResource;
	istringstream res(reqResource);
	string first;
	string sec;
	string third;

	getline(res, first, '&');
	getline(res, sec, '&');
	getline(res, third, '&');

	//map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();

	newResource = new Resource();
	AddResource(newResource, first);


	//ITERATE THROUGH STEP'S MAP OF RESOURCES, FIND MATCHING RESOURCE, ADD ATTRIBUTES
	//for every aircraft object in the master map
	//while (iter != _reqResourceMap.end())
	//{
	//	map<string, Resource*>::const_iterator iter = _reqResourceMap.find(first);
		//newResource = new Resource();
		//AddResource(newResource, first);

	if (sec != "")
	{
		//map<string, Resource*>::const_iterator iter = _reqResourceMap.find(sec);
		newResource = new Resource();
		AddResource(newResource, sec);
	}
	if (third != "")
	{
		//map<string, Resource*>::const_iterator iter = _reqResourceMap.find(third);
		newResource = new Resource();
		AddResource(newResource, third);
	}
	else
		return;

	//}
}

void Step::SetReqParts(string reqParts)
{
	Parts* newParts = new Parts();
	istringstream part(reqParts);
	string first;
	string sec;
	string third;

	getline(part, first, '&');
	getline(part, sec, '&');
	getline(part, third, '&');

	AddParts(newParts, first);

	if (sec != "")
	{
		AddParts(newParts, sec);
	}
	if (third != "")
	{
		AddParts(newParts, third);
	}
}

void Step::SetReturnStep(/*int stepId*/ int returnStep)
{
	//_returnStep = stepId;
	_returnStep = returnStep;
}

void Step::CheckBays()
{
	vector<string> resourceList;
	map<string, Resource*>::iterator it = _resourcePool.find("bay");
	if (it->second->GetResourceCount() > 0)
		SimExec::ScheduleEventAt(1, new StartServiceEA(this, _priorityQueue->GetEntity(), resourceList), 0.0, "StartServiceEA");
}


////////////////////////////////////////////
/////////////     OTHER      ///////////////
////////////////////////////////////////////

void Step::AddToResPool(Resource* resource, string resourceName)
{
	_resourcePool[resourceName] = resource;
}

void Step::AddToPartsPool(Parts* parts, string partsName)
{
	_partsPool[partsName] = parts;
}

map<string, Resource*>::iterator Step::FindResource(string resource)
{
	return _reqResourceMap.find(resource); 
}

map<string, Parts*>::iterator Step::FindParts(string resource)
{
	return _reqPartsMap.find(resource);
}

void Step::Execute(Aircraft* aircraft) {
	SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new AddQueueEA(this, aircraft), 0.0, "AddQueueEA");
}

void Step::AddResource(Resource* resource, string resourceName)
{
	_reqResourceMap[resourceName] = resource;
	//_reqResourceVec.push_back(resource);
}

void Step::AddParts(Parts* parts, string partsName)
{
	_reqPartsMap[partsName] = parts;
}

void Step::AcquireParts(Parts* parts)
{
	int newCount;

	//new count of resource pool is = to current count - num needed from req resource map
}

////////////////////////////////////////////
//////////////   PRINTERS   ////////////////
////////////////////////////////////////////

void Step::Print()
{
	cout << "		Step Name: " << _name << endl;
	cout << "		Step Type: " << _type << endl;
	cout << "		Inspection Failure Probability: "; 
	if (_inspecFailProb == nullptr)
		cout << "None \n";
	else
	{
		_inspecFailProb->PrintDistribution();
		cout << endl;
	}
	cout << "		Service Time Distribution: " << _servTime << endl;
	//cout << "		Required Resources: " << _reqRes << endl;
	//cout << "		Required Parts: " << _reqParts << endl;
//	cout << "		Return Step If Inspection Fails: " << _returnStep << endl;
	cout << endl;

	cout << "		Parts: " << endl;
	PrintParts();
	cout << endl;

	cout << "		Resources: " << endl;
	map<string, Resource*>::iterator it = _reqResourceMap.begin();
	for (int i = 0; i < _reqResourceMap.size(); i++)
	{
	//	cout << "			" << it->first << endl;
		it++;
	}
	PrintResources();
	cout << endl;
}

void Step::PrintParts()
{
	map<string, Parts*>::iterator it = _reqPartsMap.begin();
	//cout << "After creating the iterator to the map " << std::endl; 
	while (it != _reqPartsMap.end())
	{
		//cout << "in the loop \n";

		it->second->PrintPartsProperties();
		it++;
	}
}

void Step::PrintResources()
{
	map<string, Resource*>::iterator it = _reqResourceMap.begin();
//	cout << "MAP SIZE: " << _reqResourceMap.size() << endl;
	while (it != _reqResourceMap.end())
	{
		it->second->PrintResProperties();
		it++;
	}
}

void Step::PrintPools()
{
	cout << "RESOURCE POOL" << endl;
	map<string, Resource*>::iterator it = _resourcePool.begin();
//	cout << "MAP SIZE: " << _resourcePool.size() << endl;
	while (it != _resourcePool.end())
	{
		it->second->PrintResProperties();
		it++;
	}

	cout << endl;

	cout << "PARTS POOL:" << endl;
	map<string, Parts*>::iterator it2 = _partsPool.begin();
//	cout << "MAP SIZE: " << _partsPool.size() << endl;
	while (it2 != _partsPool.end())
	{
		it2->second->PrintPartsProperties();
		it2++;
	}

}