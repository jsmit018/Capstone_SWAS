#include "Step.h"
#include "Aircraft.h" //need to remove and replace priority values with 
#include <iterator>
#include "InputInterface.h"
#include <map>

map<string, Resource*> Step::_resourcePool;
map<string, Parts*> Step::_partsPool;

Step::Step(Distribution* serviceTime, string name) : Task(name)
{
	_servTime = serviceTime;
	_name = name;
	_priorityQueue = new PriorityQueue<Aircraft>("Priority Queue");
}

void Step::CopyMapStep(const Step& mapStep)
{
	_myRJ = mapStep._myRJ;
	//_serviceTime = mapStep._serviceTime;
	_name = mapStep._name;
	_indoorReq = mapStep._indoorReq;
	_RJpriority = mapStep._RJpriority;
	_stepID = mapStep._stepID;
	_numInQueue = mapStep._numInQueue;
	_nextStep = mapStep._nextStep;
	_type = mapStep._type;
	//_inspecFailProb = mapStep._inspecFailProb->CopyThis();
	_servTime = mapStep._servTime;
	_reqRes = mapStep._reqRes;
	_reqParts = mapStep._reqParts;
	_returnStep = mapStep._returnStep;

	//iterate through vectors/nonstatic maps to use resource and parts and aircraft object copy constructors

	//iterating through old required resource map, inserting its first and second into the new required resource map
	map<string, Resource*>::const_iterator reqResIter = mapStep._reqResourceMap.begin();
	while (reqResIter != mapStep._reqResourceMap.end())
	{
		Resource* newRes = new Resource();
		newRes->CopyMapResource(*reqResIter->second);
		_reqResourceMap.insert(pair<string, Resource*>(reqResIter->first, newRes));

		reqResIter++;
	}

	map<string, Parts*>::const_iterator reqPartsIter = mapStep._reqPartsMap.begin();
	while (reqPartsIter != mapStep._reqPartsMap.end())
	{
		Parts* newParts = new Parts();
		newParts->CopyMapParts(*reqPartsIter->second);
		_reqPartsMap.insert(pair<string, Parts*>(reqPartsIter->first, newParts));

		reqPartsIter++;
	}

	//not needed to copy because theyre not used til its already been copied:
	//vector<string> _acquiredResources;	//vector of acquired resources to be checked at the end of service
	//PriorityQueue<Aircraft>* _priorityQueue;
}


////////////////////////////////////////////
/////////EVENT ACTIONS AND METHODS//////////
////////////////////////////////////////////

class Step::WaitForResourceEA : public CondEventAction {
public:
	WaitForResourceEA(Step* step, Resource* resource, Aircraft* aircraft, int amountNeeded, vector<string> acqResources) {
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

class Step::DoneServiceEA : public EventAction {
public:
	DoneServiceEA(Step* step, Aircraft* aircraft, vector<string> acqResources)
	{
		_step = step;
		_aircraft = aircraft;
		_acqResources = acqResources;
	}

	void Execute()
	{
		_step->DoneServiceEM(_aircraft, _acqResources);
	}
private:
	Step* _step;
	Aircraft* _aircraft;
	vector<string> _acqResources;
};

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

class Step::AcquireResourceEA : public EventAction {
public:
	AcquireResourceEA(Step* step, Resource* resource)
	{
		_step = step;
		_resource = resource;
	}

	void Execute()
	{
		_step->AcquireResourceEM(_resource);
	}
private:
	Step* _step;
	Resource* _resource;
};

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

class Step::RestoreResourceEA : public EventAction {
public:
	RestoreResourceEA(Step* step, Resource* resource) {
		_step = step;
		_resource = resource;
	}

	void Execute() {
		_step->RestoreResourceEM(_resource);
	}

private:
	Step* _step;
	Resource* _resource;
};

// PLACE ORDER/ REPLENISH order process
	// schedule an event that will act as "place order"   -> placeorderEA does placeOrderEM
	// "place order" execution will schedule a "order arrived/ parts replenish event"	 placeOrderEM schedules partsArrivalEA
	// "order arrived/ parts replenish event" execution will simply do _partsCount + Number ordered -> partsArrivalEA does partsArrivalEM

void Step::PlaceOrderEM(Parts* parts)
{
	//Schedule Order Arrivals EA at now + lead time
	cout << "Parts total has fallen below its identified threshold, placing an order" << endl;
	outputRecorder->RecordPartRequest(parts->GetPartsName(), parts->GetPartsCount(), true);
	SimExec::ScheduleEventAt(1, new OrderArrivalEA(this, parts), parts->GetLeadTime()->GetRV(), "OrderArrivalEA");
}

void Step::OrderArrivalEM(Parts* parts)
{
	//Logic: new count = initial count -- same as new count = current count + (initial count - current count)
	int newCount;

	map<string, Parts*>::const_iterator iter = _reqPartsMap.find(parts->GetPartsName());
	newCount = iter->second->GetInitPartsCount();

	parts->SetPartsCount(newCount);
	outputRecorder->RecordRestock(parts->GetPartsName(), SimExec::GetSimulationTime()._timeOfDay);
	SimExec::CheckConditionalEvents(0, parts);
}

void Step::StartServiceEM(Aircraft* aircraft, vector<string> _acquiredResources)
{
	cout << "Step " << this->GetID() << " of " << this->GetMyRJName() << " started on "
		<< aircraft->GetAircraftType() << " of ID " << aircraft->GetAircraftID() << endl;


	//check if acquired resource vector has bay or spot
	bool hasResource = false;

	for (int i = 0; i < _acquiredResources.size(); i++)
	{
		if (_indoorReq == 'Y' || _indoorReq == 'y') {
			if (_acquiredResources[i] == "bay") {
				hasResource = true;
				break;
			}
		}
		/*IF TARMAC HAS LIMITED SPOTS*/
		/*
		else {
			if (_acquiredResources[i] == "outspot") {
				hasResource = true;
				break;
			}
		}
		*/
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
				cout << "Getting a Bay" << endl;
				// call decrement function, push it to acquired vector 
				_acquiredResources.push_back(it->first);
			}
			else
				cout << "we have to wait for a bay \n";
			// WAITING
//			cout << "Adding the Aircraft to the Conditional Event List until a Bay is available" << endl;
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
			outputRecorder->RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), it->second->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
		}

		/*IF TARMAC HAS LIMITED SPOTS */
	/*	
		else
		{ 
			map<string, Resource*>::iterator it = _resourcePool.find("outspot");
			if (it->second->GetResourceCount() > 0)
			{

				//acquire resource
				SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new AcquireResourceEA(this, it->second), 0.0, "AcquireResourceEA");

				//push it to acquired vector 
				_acquiredResources.push_back(it->first);
			}
			else
				cout << "we have to wait for an outspot \n";
			// WAITING
			cout << "Outspot unavailable, Adding Plane Step to Conditional Event List until it is." << endl;
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
		}
	*/
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
//			cout << it->first << " is unavailable, adding Aircraft to the Conditional Event List until it is available." << endl;
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
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
				cout << "There are not enough parts scheduling to place a new order" << endl;
				SimExec::ScheduleEventAt(1, new PlaceOrderEA(this, it->second), 0.0, "PlaceOrderEA");
				// WAITING
				cout << "Adding Aircraft to the Conditional Event list until " << it->first << " becomes available" << endl;
				SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedPartsEA(this, it->second, aircraft, it->second->GetNumPartsNeeded(), _acquiredResources));
			}

			iter2++;
		}

		cout << "Aircraft has finished a maintenance step, scheduling a DoneService" << endl;
		DoneServiceEA* doneEA = new DoneServiceEA(this, aircraft, _acquiredResources);
		SimExec::ScheduleEventAt(1, doneEA, this->_servTime->GetRV(), "DoneServiceEA");
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
			cout << it->first << " is unavailable adding aircraft, " << aircraft->GetAircraftType() << " to the Conditional Event List until it is available." << endl;
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));

			//TO DO:
			//if inspection results in failure = true
			if (IsInpectionFail(_inspecFailProb) == true)
			{
				//IS THIS RIGHT		
				cout << "Inspection failed, Rescheduling appropriate maintenance." << endl;
				SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(aircraft->GetMyRepairJobObj(_myRJ)->GetStep(_returnStep), aircraft, _acquiredResources), 0, "StartServiceEA");
				//reschedule step of id = to return step id and all following steps
			}

			else {
				cout << "Aircraft maintenance passed inspection, scheduling DoneService." << endl;
				SimExec::ScheduleEventAt(1, new DoneServiceEA(this, aircraft, _acquiredResources), _servTime->GetRV(), "DoneServiceEA");
			}

			iter++;
		}


	}

}
//
//void Step::PrintEvent(Aircraft* aircraft,  RepairJob* repairjob, Step* step, Resource* resource, Parts* parts)
//{
//	//Need to have sim exec get event name
//	cout << "EVENT: ";
//	//SimExec::GetEventName();
//	cout << "	occurred at: ";
//	SimExec::GetSimulationTime();
//	cout << endl;
//
//	cout << "On Aircraft: " << aircraft->GetAircraftType() << "	with ID: " << aircraft->GetAircraftID() << endl;
//	cout << "On Resource: " << resource->GetResourceName() << endl;
//
//	//add if statement to differentiate aircraft from resource events
//	cout << "	Repair Job: " << repairjob->GetName() << endl;
//	cout << "		Step: " << step->GetName() << endl;
//	
//}

void Step::SetMyRJName(string myRJ)
{
	_myRJ = myRJ;
}

string Step::GetMyRJName()
{
	return _myRJ;
}

void Step::DoneServiceEM(Aircraft* aircraft, vector<string> acquiredResources)
{
	//increment stepid
	int nextId = _stepID + 1;
	//if stepid <= containerSize;
	if (nextId <= aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize())
	{
		//schedule next step
		cout << "Current Maintenance step has finished, scheduling the next maintenance step." << endl;
		SimExec::ScheduleEventAt(GetRJPriority(), new StartServiceEA(aircraft->GetMyRepairJobObj(_myRJ)->GetStep(_stepID++), aircraft, _acquiredResources), 0.0, "StartServiceEA");

		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		//for all resources in next step's required list
		for (int i = 0; i < acquiredResources.size(); i++)
		{
			//if resource name is found in acquired vector
			if (_nextStep->ResourceInReqResource(acquiredResources[i]))
				//go to next resource
				continue;

			//schedule resource release ea
			cout << "Release appropriate resource not needed for the next maintenance step" << endl;
			SimExec::ScheduleEventAt(_RJpriority, new ReleaseResourceEA(this, iter->second), 0.0, "ReleaseResourceEA");

			//empty appropriate acquired vector index
			_acquiredResources.erase(acquiredResources.begin() + i);
		}


	}
	//else if repair job is done, get the next one
	else if (nextId = aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize())
	{
		//if no more jobs, we're done
		if (aircraft->GetNextRepairJob(_myRJ) == NULL)
		{
			//this aircraft is done, so it will depart (according to the setnext task in SWAS)
			return;
		}

		//next id is the next repairjob's first step
		nextId = aircraft->GetNextRepairJob(_myRJ)->GetFirstStep()->GetID();
		SimExec::ScheduleEventAt(GetRJPriority(), new StartServiceEA(aircraft->GetNextRepairJob(_myRJ)->GetStep(nextId), aircraft, _acquiredResources), 0.0, "StartServiceEA");
	}



	//vector<string> resourceList;
	///**Discontinued Scheduling Statement
	//SimExec::ScheduleEventAt(1, new StartServiceEA(this, _priorityQueue->GetEntity(), _acquiredResources), 0.0, "StartServiceEA");
}

void Step::AddQueueEM(Aircraft* aircraft)
{
	//_priorityQueue->AddEntity(aircraft, aircraft->GetAircraftPriority());
	//_numInQueue++;
	//InitialArrivalBayCheck();
}

void Step::AcquireResourceEM(Resource* resource)
{
	int newCount;

	map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() - iter->second->GetNumResNeeded();
	resource->SetResourceCount(newCount);
}

void Step::ReleaseResourceEM(Resource* resource)
{
	int newCount;

	map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());
	newCount = iter->second->GetResourceCount() + iter->second->GetNumResNeeded();

	resource->SetResourceCount(newCount);
	IsResourceReleased(iter, newCount);

	/////////******For Andrea is this where we want to put this? I feel it may be best!
	SimExec::CheckConditionalEvents(resource, 0);

}

void Step::FailResourceEM(Resource* resource)
{
	int newCount;
	//Mark added this i'm not sure we need to create a new instance, but i'm just going to put a priority of 1 - Jordan
	//RepairJob* newJob;

	map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() - 1;
	resource->SetResourceCount(newCount);

	//SimExec::ScheduleEventAt(newJob->GetPriority(), new FailResourceEA(this, resource), iter->second->GetFailureDistr()->GetRV(), "New Repair Job");
	//This Event action should actually be scheduling a restore resource instead of a fail one.
	cout << "Resource has failed, scheduling a restore resource" << endl;
	SimExec::ScheduleEventAt(1, new RestoreResourceEA(this, resource), this->_servTime->GetRV(), "RestoreResourceEA");
}

void Step::RestoreResourceEM(Resource* resource)
{
	cout << "Resource has been restored, updating amount and checking conditional events" << endl;
	resource->RestoreResource();
	SimExec::CheckConditionalEvents(resource, 0);
}


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

bool Step::IsInpectionFail(Distribution* inspecFailProb)
{
	//is this how we're handling the distributions? [to check with Yang]
	if (inspecFailProb->GetRV() >= 0.51)
	{
		return true;
	}
	else return false;
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

map<string, Resource*>::iterator Step::GetResourceMapBegin()
{
	return _reqResourceMap.begin();
}

map<string, Resource*>::iterator Step::GetResourceMapEnd()
{
	return _reqResourceMap.end();
}

void Step::ScheduleFirstRecurringStep(Step* step, Aircraft* aircraft)
{
	SimExec::ScheduleEventAtRecurring(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "StartServiceEA");
	cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
}

void Step::ScheduleCalendarStep(Step* step, Aircraft* aircraft, CalendarObj* calobj)
{
	for (int i = 0; i < calobj->GetNumEvents(); ++i) {
		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), "StartServiceEA");
	}
	cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
}

map<string, Parts*>::iterator Step::GetPartsMapBegin()
{
	return _reqPartsMap.begin();
}

map<string, Parts*>::iterator Step::GetPartsMapEnd()
{
	return _reqPartsMap.end();
}

int Step::GetRJPriority()
{
	return _RJpriority;
}

int Step::GetRJIndoorReq()
{
	return _indoorReq;
}

Distribution* Step::GetServiceTime()
{
	return _servTime;
}

////////////////////////////////////////////
//////////////    SETTERS    ///////////////
////////////////////////////////////////////

void Step::SetStepID(int stepID)
{
	_stepID = stepID;
	//	cout << "step id " << stepID << " " << _stepID << endl;
}

void Step::SetName(string name)
{
	_name = name;
}

void Step::SetType(string type)
{
	_type = type;
}

void Step::SetRJPriority(int RJpriority)
{
	_RJpriority = RJpriority;
	//cout << "&&&&&    *** JOB PRIORITY" << _RJpriority << endl;

}

void Step::SetStepIndoorReq(char indoorReq)
{
	_indoorReq = indoorReq;
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
//	_inspecFailProb->PrintDistribution();
}

void Step::SetServiceTime(string serviceTime)
{
	//the string being passed in is now split into two strings
	istringstream serveDist(serviceTime);
	string firstHalf;
	string secHalf;

	//they're split based on the ( and ) symbols. The parenthesis are treated as a delimiter.
	getline(serveDist, firstHalf, '(');
	getline(serveDist, secHalf, ')');
	//	cout << "first: " << firstHalf << endl;
	//	cout << "sec: " << secHalf << endl;

		//this is used for the second half to turn the numbers into the doubles
	istringstream nums(secHalf);

	//if statements for determining which distribution it is
	if (firstHalf == "Triangular" || firstHalf == "Tri")
	{
		double min, expected, max;
		//the first part of the string segment called "nums" is set a the double min. second one is double expected, etc. 
		nums >> min;
		nums >> expected;
		nums >> max;
		_servTime = new Triangular(min, expected, max);
	}

	else if (firstHalf == "Exponential")
	{
		double mean;
		nums >> mean;

		_servTime = new Exponential(mean);
	}

	else if (firstHalf == "Uniform")
	{
		double min, max;
		nums >> min >> max;

		_servTime = new Uniform(min, max);
	}

	else if (firstHalf == "Normal")
	{
		double mean, stdev;
		nums >> mean >> stdev;

		_servTime = new Normal(mean, stdev);
	}

	else if (firstHalf == "Poisson")
	{
		double mean;
		nums >> mean;

		_servTime = new Poisson(mean);
	}

	else if (firstHalf == "Constant" || firstHalf == "Fixed")
	{
		double mean;
		nums >> mean;

		_servTime = new Constant(mean);
	}

	else if (firstHalf == "Weibull")
	{
		double scale, shape;
		nums >> scale >> shape;

		_servTime = new Weibull(scale, shape);
	}

	//Calls the print function added to each distribution - Determines correct distribution and prints
//	_servTime->PrintDistribution();
}

void Step::SetReqResource(string reqResource)
{
	//change this to check for while finding &
	Resource* newResource;
	istringstream res(reqResource);
	string first;
	string sec;
	string third;

	string line;


	while (getline(res, line, '&'))
	{
		//	cout << "LINE: " << line << endl;

		istringstream ss(line);
		string resource;
		string numString;
		int num;

		getline(ss, resource, '(');
		getline(ss, numString, ')');

		//	cout << "	R: " << resource << "	N: " << numString << endl;

		istringstream ssNum(numString);
		ssNum >> num;

		//	cout << "	R: " << resource << "	N: " << num << endl;


		newResource = new Resource();
		AddResource(newResource, resource, num);

	}

	//ITERATE THROUGH STEP'S MAP OF RESOURCES, FIND MATCHING RESOURCE, ADD ATTRIBUTES

	if (sec != "")
	{
		newResource = new Resource();
		//AddResource(newResource, sec, /**/);
	}
	if (third != "")
	{
		newResource = new Resource();
		//AddResource(newResource, third, /**/);
	}
	else
		return;
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

//void Step::SetNumPartsNeeded(string numPartsNeeded)
//{
//
//}
//
//void Step::SetResNum(int numResNeeded)
//{
//	map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
//
//	while (iter != _reqResourceMap.end())
//	{
//		iter->second->SetNumResNeeded(numResNeeded);
//	}
//
//}

void Step::SetReturnStep(/*int stepId*/ int returnStep)
{
	//_returnStep = stepId;
	_returnStep = returnStep;
}

//jordan: is this for intiial arrival/first step? if so, can we rename FirstBayCheck or something similar so its clear?
//we're also checking bays in the startstepservice so i just want to differentiate them clearly
void Step::InitialArrivalBayCheck()
{
	/////***Discontinued logic - Opted instead for Virtual Queue
	/*vector<string> resourceList;
	map<string, Resource*>::iterator it = _resourcePool.find("bay");
	if (it->second->GetResourceCount() > 0)
		SimExec::ScheduleEventAt(1, new StartServiceEA(this, _priorityQueue->GetEntity(), resourceList), 0.0, "StartServiceEA");*/
}


////////////////////////////////////////////
/////////////     OTHER      ///////////////
////////////////////////////////////////////

//now exclusive to unplanned
void Step::ScheduleFirstStep(Step* step, Aircraft* aircraft)
{
	///todo: will replace mymap with myunplannedjobsmap in future
//	map<string, RepairJob*>::const_iterator iter = aircraft->GetMyUnplannedRJMapBegin();
//
//	while (iter != aircraft->GetMyUnplannedRJMapEnd())
//	{
//		if (iter->second->GetSchedType() == "Unplanned")
//		{
//			/* roll dice on unplanned probability to see if we're going to schedule it */
//			if (iter->second->WillSchedule() == true)
//			{
//				cout << aircraft->GetAircraftType() << "	will have a Random Repair Job called: " << iter->second->GetName() << endl;
//
//
//				SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "StartServiceEA");
//				cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
//
//				iter++;
//			}
//
//			else
//				cout << aircraft->GetAircraftType() << "	will not have a Random Repair Job called: " << iter->second->GetName() << endl;
//		}
//	}
////
	SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "StartServiceEA");
	cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
	////	SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "AddToQueueEA");
}



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
	////****Discontinued Logic
	//SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new AddQueueEA(this, aircraft), 0.0, "AddQueueEA");
}

void Step::AddResource(Resource* resource, string resourceName, int numNeeded)
{
	_reqResourceMap[resourceName] = resource;
	_reqResourceMap[resourceName]->SetNumResNeeded(numNeeded);
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
	cout << "		Step ID: " << _stepID << endl;
	cout << "		Step Type: " << _type << endl;
	cout << "		Indoor Req: " << _indoorReq << endl;
	cout << "		RJ Priority: " << _RJpriority << endl;
	cout << "		Inspection Failure Probability: ";
	if (_inspecFailProb == nullptr)
		cout << "None \n";
	//else
	//{
		//cout << "Service time is ";
	//	_inspecFailProb->PrintDistribution();
	//	cout << endl;
	//}
	cout << "		Service Time Distribution: ";
	if (_servTime == nullptr)
		cout << "None \n";
	//else
	//{
	//	cout << "Service time is ";
	//	_servTime->PrintDistribution();
	//	cout << endl;
	//}
	//cout << "		Required Resources: " << _reqRes << endl;
	//cout << "		Required Parts: " << _reqParts << endl;
	cout << "		Return Step If Inspection Fails: " << _returnStep << endl;

	cout << "			Parts - " << endl;
	PrintParts();
	cout << endl;

	cout << "			Resource - " << endl;
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
		cout << "MAP SIZE: " << _resourcePool.size() << endl;
	while (it != _resourcePool.end())
	{
		it->second->PrintResProperties();
		it++;
	}

	cout << endl;

	cout << "PARTS POOL:" << endl;
	map<string, Parts*>::iterator it2 = _partsPool.begin();
		cout << "MAP SIZE: " << _partsPool.size() << endl;
	while (it2 != _partsPool.end())
	{
		it2->second->PrintPartsProperties();
		it2++;
	}

}