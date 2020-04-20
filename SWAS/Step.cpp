#include "Step.h"
#include "Aircraft.h" //need to remove and replace priority values with 
#include <iterator>
#include "InputInterface.h"
#include "SinkTask.h"
#include <map>

map<string, Resource*> Step::_resourcePool;
map<string, Parts*> Step::_partsPool;
map<string, RepairJob*> RepairJob::_resourceRepairMap;
bool isReturnStep = false;

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
	if (mapStep._inspecFailProb == nullptr)
		_inspecFailProb = nullptr;
	else
	{
		_inspecFailProb = mapStep._inspecFailProb->CopyThis();
		//cout << "COPYING INSPEC FAIL ";
		//_inspecFailProb->PrintDistribution();
		//cout << endl;
	}
	_servTime = mapStep._servTime; // does this need to have copythis()
	_reqRes = mapStep._reqRes;
	_reqParts = mapStep._reqParts;
	if (mapStep._returnStep == NULL)
		_returnStep = NULL;
	else
		_returnStep = mapStep._returnStep;


	//cout << ".....IN STEP NEXT INDOOR REQ IS " << _indoorReq << endl;

	//iterate through vectors/nonstatic maps to use resource and parts and aircraft object copy constructors

	//iterating through old required resource map, inserting its first and second into the new required resource map
	map<string, Resource*>::const_iterator reqResIter = mapStep._reqResourceMap.begin();
	while (reqResIter != mapStep._reqResourceMap.end())
	{
		//cout << " iN COPY STEP " << reqResIter->first << endl;
		Resource* newRes = new Resource();
		newRes->CopyMapResource(*reqResIter->second);
		_reqResourceMap.insert(pair<string, Resource*>(reqResIter->first, newRes));

		reqResIter++;
	}

	map<string, Parts*>::const_iterator reqPartsIter = mapStep._reqPartsMap.begin();
	while (reqPartsIter != mapStep._reqPartsMap.end())
	{
		Parts* newParts = new Parts();
//		cout << " iN COPY STEP, part is " << reqPartsIter->first << endl;

		//cout << " parts test in copy step ";
		//reqPartsIter->second->GetLeadTime()->PrintDistribution();
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
		//_outputRecorder = outputRecorder;
	}

	bool Condition(Resource* resource, Parts* parts) {
		if (resource == _resource) {
			if (_resource->GetResourceCount() >= _amountNeeded)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	void Execute() {
		Scribe::RecordResourceWaitEnd(_aircraft->GetAircraftID(), _resource->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
		_step->StartServiceEM(_aircraft, _acqResources);
	}

private:
	Resource* _resource;
	Step* _step;
	Aircraft* _aircraft;
	int _amountNeeded;
	string _funcName;
	vector<string> _acqResources;
	//Scribe* _outputRecorder;

};

class Step::ResWaitForResEA : public CondEventAction {
public:
	ResWaitForResEA(Step* step, Resource* resource, Resource* required, int amountNeeded, vector<string> acqResources) {
		_step = step;
		_resource = resource;
		_required = required;
		_amountNeeded = amountNeeded;
		_acqResources = acqResources;
	}

	bool Condition(Resource* required, Parts* parts)
	{
		if (required == _required)
		{
			if (required->GetResourceCount() >= _amountNeeded)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	void Execute()
	{
		_step->StartRepairServiceEM(_resource, _acqResources);
	}

private:
	Step* _step;
	Resource* _resource;
	Resource* _required;
	int _amountNeeded;
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

class Step::StartRepairServiceEA : public EventAction {
public:
	StartRepairServiceEA(Step* step, Resource* resource, vector<string> acqResources) {
		_step = step;
		_resource = resource;
		_acqResources = acqResources;
	}

	void Execute() {
		_step->StartRepairServiceEM(_resource, _acqResources);
	}
private:
	Step* _step;
	Resource* _resource;
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

class Step::DoneRepairServiceEA : public EventAction {
public:
	DoneRepairServiceEA(Step* step, Resource* resource, vector<string> acqResources)
	{
		_step = step;
		_resource = resource;
		_acqResources = acqResources;
	}

	void Execute()
	{
		_step->DoneRepairServiceEM(_resource, _acqResources);
	}
private:
	Step* _step;
	Resource* _resource;
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
		_numNeeded = 0;
	}

	void Execute()
	{
		_step->AcquireResourceEM(_resource, _numNeeded);
	}
private:
	Step* _step;
	Resource* _resource;
	int _numNeeded;
};

class Step::ReleaseResourceEA : public EventAction {
public:
	ReleaseResourceEA(Step* step, Resource* resource) {
		_step = step;
		_resource = resource;
		_numRelease = 0;
	}

	void Execute() {
		_step->ReleaseResourceEM(_resource, _numRelease);
	}
private:
	Step* _step;
	Resource* _resource;
	int _numRelease;
};

//class Step::FailResourceEA : public EventAction {
//public:
//	FailResourceEA(Step* step, Resource* resource) {
//		_step = step;
//		_resource = resource;
//	}
//
//	void Execute() {
//		_step->FailResourceEM(_resource);
//	}
//private:
//	Step* _step;
//	Resource* _resource;
//};
//
//class Step::RestoreResourceEA : public EventAction {
//public:
//	RestoreResourceEA(Step* step, Resource* resource) {
//		_step = step;
//		_resource = resource;
//	}
//
//	void Execute() {
//		_step->RestoreResourceEM(_resource);
//	}
//
//private:
//	Step* _step;
//	Resource* _resource;
//};

// PLACE ORDER/ REPLENISH order process
	// schedule an event that will act as "place order"   -> placeorderEA does placeOrderEM
	// "place order" execution will schedule a "order arrived/ parts replenish event"	 placeOrderEM schedules partsArrivalEA
	// "order arrived/ parts replenish event" execution will simply do _partsCount + Number ordered -> partsArrivalEA does partsArrivalEM

void Step::PlaceOrderEM(Parts* parts)
{
	if (parts->GetLeadTime() == nullptr)
	//	cout << "poop " << endl;
	//Schedule Order Arrivals EA at now + lead time
	cout << _myRJ << " " << _stepID << " " << parts->GetPartsName() << endl;
	cout << "Parts total has fallen below its identified threshold, placing an order" << endl;
	double restockTime = parts->GetLeadTime()->GetRV();
	Scribe::RecordRestock(parts->GetPartsName(), restockTime);
	SimExec::ScheduleEventAt(1, new OrderArrivalEA(this, parts), restockTime, "OrderArrivalEA");
}

void Step::OrderArrivalEM(Parts* parts)
{
	//Logic: new count = initial count -- same as new count = current count + (initial count - current count)
	int newCount;

	map<string, Parts*>::const_iterator iter = _reqPartsMap.find(parts->GetPartsName());
	newCount = iter->second->GetInitPartsCount();

	//parts->SetPartsCount(newCount);
	SetPartPoolCount(iter->first, newCount);

	//Scribe::RecordRestock(parts->GetPartsName(), SimExec::GetSimulationTime()._timeOfDay);
	SimExec::CheckConditionalEvents(0, parts);
}

int Step::GetReturnStep()
{
	return _returnStep;
}
void Step::StartServiceEM(Aircraft* aircraft, vector<string> acquiredResources)
{
	if (isReturnStep == true)
	{
		cout << "is return step " << _name << " for " << aircraft->GetAircraftID() << " return step is "<< aircraft->GetMyRepairJobObj(_myRJ)->GetMyReturnStep() << endl;
		SetStepID(aircraft->GetMyRepairJobObj(_myRJ)->GetMyReturnStep());
	}
	isReturnStep = false;

	_acquiredResources = acquiredResources;
	cout << "Step " << _type << " " << _name << " " << _stepID << " of " << this->GetMyRJName() << " started on "
		<< aircraft->GetAircraftType() << " of ID " << aircraft->GetAircraftID() << endl;

	Scribe::RecordRepairJob(aircraft->GetAircraftType(), aircraft->GetAircraftID(), _myRJ, SimExec::GetSimulationTime()._timeOfDay);
	Scribe::RecordServiceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), "Bay", SimExec::GetSimulationTime()._timeOfDay);

	//check if acquired resource vector has bay or spot
	bool hasResource = false;

	for (int i = 0; i < _acquiredResources.size(); i++)
	{
		if (_indoorReq == 'Y' || _indoorReq == 'y') {
			if (_acquiredResources[i] == "S Bay" || _acquiredResources[i] == "M Bay" || _acquiredResources[i] == "L Bay") {
				//if (_acquiredResources[i] == "bay") {
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

	// if i don't have a bay 
	if (hasResource == false)
	{
		// if inside step
		//cout << " HAS RESOURCE FALSE " << _indoorReq << endl;

		if (_indoorReq == 'Y' || _indoorReq == 'y')
		{
			//TODO need to check for specific bay size
			string bayReq = aircraft->GetBaySizeReq();
			map<string, Resource*>::iterator it = _resourcePool.find(bayReq);
		//	cout << "sie is " << _resourcePool.size() << endl;
			if (it != _resourcePool.end())
			{
				if (it->second->GetResourceCount() > 0)
				{
					//		cout << "Getting a Bay" << endl;
							// call decrement function, push it to acquired vector 
					_acquiredResources.push_back(it->first);
					AcquireResourceEM(it->second, 1);
					//		cout << "---------------BAY SIZE ACQUIRED IS " << it->first << endl;
				}
				else {
					//		cout << "we have to wait for a bay \n";
							// WAITING
				//			cout << "Adding the Aircraft to the Conditional Event List until a Bay is available" << endl;
					SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, 1, _acquiredResources));
					Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), it->second->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
					Scribe::UpdateResourceRequests(it->second->GetResourceName(), false);
					return;
				}
			}
		//	if (it == _resourcePool.end())
		//		cout << " i dont exist \n";
			
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

	if (_type == "process" || _type == "Process")
	{
		bool alreadyAcquired = false;
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		//for all resources listed in required map
		while (iter != _reqResourceMap.end())
		{
			//compare key to acquired resources vector
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				//if it's already acquired, break
				if (iter->first == _acquiredResources[i]) {
					alreadyAcquired = true;
					break;
				}
			}

			if (alreadyAcquired)
				cout << "ID: "<< aircraft->GetAircraftID() << "I already have " << iter->first << endl;
			else {
				map<string, Resource*>::iterator it = _resourcePool.find(iter->first);

				//otherwise if not acquired yet
				//if resource count is greater than number needed
				if (_resourcePool.find(iter->first)->second->GetResourceCount() >= iter->second->GetNumResNeeded())
					//if (it->second->GetResourceCount() >= iter->second->GetNumResNeeded())
				{
					//decrement appropriately
					/*int newCount;
					newCount = it->second->GetResourceCount() - it->second->GetNumResNeeded();
					it->second->SetResourceCount(newCount);*/
					AcquireResourceEM(it->second, iter->second->GetNumResNeeded());
					//store in acquired resource vector
					_acquiredResources.push_back(it->first);
				//	cout << "----------ACQUIRED  " << it->first << endl;

				}
				else {
				//	cout << " we have to wait for a/an " << iter->first << endl;
					if (SimExec::GetSimulationTime()._year == 2025 && SimExec::ConvertDate(SimExec::GetSimulationTime()._month) == "July")
				//		cout << "Hehe sounds like a lotta hoopla" << endl;
					//INSERT WAITING LOGIC
		//			cout << it->first << " is unavailable, adding Aircraft to the Conditional Event List until it is available." << endl;
					SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, iter->second->GetNumResNeeded(), _acquiredResources));
					iter++;
					Scribe::UpdateResourceRequests(it->second->GetResourceName(), false);
					Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), it->second->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
					return;
				}
			}
			iter++;
		}

		//for all parts listed in required map
		/*if (iter2 == _reqPartsMap.end())
			cout << "Step doesn't require parts" << endl;*/
			//else {
		if( _reqPartsMap.size() > 0)
		{ 
			map<string, Parts*>::const_iterator iterParts = _reqPartsMap.begin();
			//cout << "BLAH " << _reqPartsMap.size() << endl;
			//cout << iterParts->first << endl;
			//cout << "after \n";
			if (iterParts->second->GetPartsName() == "none needed") 
			{
				cout << "No parts needed" << endl;
				cout << _myRJ << endl;
			}
			else if (_reqPartsMap.size() > 0) {
				while (iterParts != _reqPartsMap.end())
				{
					/*if (iter2->first == "")
						iter2++;*/
						/*else {*/
					map<string, Parts*>::iterator it = _partsPool.find(iterParts->first);

					//if (it->second == NULL) {
					if (it->second->AreEnoughParts() == true) 
					{
						cout << "decrementing parts" << endl;

						/*int newCount;
						newCount = it->second->GetPartsCount() - it->second->GetNumPartsNeeded();

						it->second->SetPartsCount(newCount);*/
						cout << "number of parts needed " << iterParts->second->GetNumPartsNeeded() << endl;
						AcquireParts(iterParts->second, iterParts->second->GetNumPartsNeeded());
						Scribe::RecordPartRequest(iterParts->first, iterParts->second->GetNumPartsNeeded(), true);
					}

					else if (it->second->AreEnoughParts() == false) {
						cout << "There are not enough parts scheduling to place a new order" << endl;
						Scribe::RecordPartRequest(iterParts->first, iterParts->second->GetNumPartsNeeded(), false);
						SimExec::ScheduleEventAt(1, new PlaceOrderEA(this, it->second), 0.0, "PlaceOrderEA");
						// WAITING
						cout << "Adding Aircraft to the Conditional Event list until " << it->first << " becomes available" << endl;
						SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedPartsEA(this, it->second, aircraft, it->second->GetNumPartsNeeded(), _acquiredResources));
						return;
					}
					//}

					iterParts++;
					/*}*/
				}
		

			}
		}

		cout << "Aircraft " << aircraft->GetAircraftID() << " has finished a maintenance step called " << _name << ", scheduling a DoneService" << endl;
		//cout << "++++++++ acquired resource vector size " << _acquiredResources.size() << endl;
		DoneServiceEA* doneEA = new DoneServiceEA(this, aircraft, _acquiredResources);
		Scribe::RecordServiceWaitEnd(aircraft->GetAircraftID(), "Bay", SimExec::GetSimulationTime()._timeOfDay);
		SimExec::ScheduleEventAt(1, doneEA, this->_servTime->GetRV(), "DoneServiceEA");
	}

	else if (_type == "inspection" || _type == "Inspection")
	{

	//cout << " zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz " << endl;

		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		//for all resources listed in required map
		while (iter != _reqResourceMap.end())
		{
			//cout << " zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz "  << endl;

			//compare key to acquired resources vector
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				//if it's already acquired, break
				if (iter->first == _acquiredResources[i])
					break;
			}

			map<string, Resource*>::iterator it = _resourcePool.find(iter->first);

			if (it->second->GetResourceCount() < it->second->GetNumResNeeded())
			{ 
				cout << " we have to wait for a/an " << it->first << endl;
				//INSERT WAITING LOGIC
				cout << it->first << " is unavailable adding aircraft, " << aircraft->GetAircraftType() << " to the Conditional Event List until it is available." << endl;
				SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
				Scribe::UpdateResourceRequests(it->second->GetResourceName(), false);
				Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), it->second->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
				return;
			}
			//otherwise if not acquired yet
			//if resource count is greater than number needed
			//if (it->second->GetResourceCount() >= it->second->GetNumResNeeded())
			else if (it->second->GetResourceCount() >= it->second->GetNumResNeeded())
			{
				cout << "in else if " << endl;
				//decrement appropriately
				/*int newCount;
				newCount = it->second->GetResourceCount() - it->second->GetNumResNeeded();
				it->second->SetResourceCount(newCount);*/
				AcquireResourceEM(it->second, iter->second->GetNumResNeeded());

				//store in acquired resource vector
				_acquiredResources.push_back(it->first);

				//Scribe::UpdateResourceRequests(it->second->GetResourceName(), true);
				//Scribe::UpdateResourceUtilization(it->second->GetResourceName(), it->second->GetNumResNeeded(), SimExec::GetSimulationTime()._timeOfDay);
				//			}

							//else {
							//	cout << " we have to wait for a/an " << it->first << endl;
							//	//INSERT WAITING LOGIC
							//	cout << it->first << " is unavailable adding aircraft, " << aircraft->GetAircraftType() << " to the Conditional Event List until it is available." << endl;
							//	SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, it->second->GetNumResNeeded(), _acquiredResources));
							//	Scribe::UpdateResourceRequests(it->second->GetResourceName(), false);
							//	Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), it->second->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
							//	return;
							//}

							//if inspection results in failure
				if (IsInpectionFail(_inspecFailProb) == true)
				{
					isReturnStep = true;
					//TODO: Check if this is right
					cout << _myRJ << " return step " << _returnStep << "vec size" << aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize() << endl;
					Scribe::RecordRework(aircraft->GetAircraftType(), _myRJ, SimExec::GetSimulationTime()._timeOfDay);
					cout << aircraft->GetAircraftID() << "'s Inspection failed, Rescheduling appropriate maintenance at step " << _returnStep << endl;
					//cout << aircraft->GetAircraftID() <<  "'S NEW RETURN ID IS " << _returnStep << endl;
					//_stepID = _returnStep;

					cout << "in return step" << endl;

					SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(aircraft->GetMyRepairJobObj(_myRJ)->GetStep(_returnStep), aircraft, _acquiredResources), 0, "StartServiceEA");
					//reschedule step of id = to return step id and all following steps

					return;
				}

				else if (IsInpectionFail(_inspecFailProb) == false)
				{
					cout << "Aircraft maintenance passed inspection, scheduling DoneService." << endl;
					Scribe::RecordRepairEnd(aircraft->GetAircraftID(), _myRJ, SimExec::GetSimulationTime()._timeOfDay);
					SimExec::ScheduleEventAt(1, new DoneServiceEA(this, aircraft, _acquiredResources), _servTime->GetRV(), "DoneServiceEA");
				}
			}
			iter++;
		}


	}

}
void Step::StartRepairServiceEM(Resource* resource, vector<string> acquiredResources)
{
	
	if (isReturnStep == true)
	{
		cout << " is return step " << _name << " for " << resource->GetResourceName() << " return step is " << RepairJob::FindResRepairJobObj(resource->GetResourceName())->GetMyReturnStep();
		SetStepID(RepairJob::FindResRepairJobObj(resource->GetResourceName())->GetMyReturnStep());
	}
	else
	{
		Scribe::RecordFailure(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetTotalSimulationTime());
	}
	isReturnStep = false;

	_acquiredResources = acquiredResources;
	cout << "Step " << _type << " " << _name << " " << _stepID << " of " << resource->GetFailureName() << " started on "
		<< resource->GetResourceName();

	bool hasResource = false;

	if (_type == "process" || _type == "Process")
	{
		bool alreadyAcquired = false;
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();

		while (iter != _reqResourceMap.end())
		{
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				if (iter->first == _acquiredResources[i])
				{
					alreadyAcquired = true;
					break;
				}
			}

			if (alreadyAcquired)
			{
				cout << resource->GetResourceName() << ": I already have a " << iter->first << endl;
			}

			else
			{
				map<string, Resource*>::iterator it = _resourcePool.find(iter->first);

				if (_resourcePool.find(iter->first)->second->GetResourceCount() >= iter->second->GetNumResNeeded())
				{
					AcquireResourceEM(it->second, iter->second->GetNumResNeeded());
					_acquiredResources.push_back(it->first);
				}
				else
				{
					SimExec::ScheduleConditionalEvent(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetPriority(), new ResWaitForResEA(this, resource, it->second, iter->second->GetNumResNeeded(), _acquiredResources));
					return;
				}
			}
			iter++;
		}
		DoneRepairServiceEA* repair = new DoneRepairServiceEA(this, resource, acquiredResources);
		SimExec::ScheduleEventAt(1, repair, this->_servTime->GetRV(), "RestoreEA");
	}
	
	else if (_type == "inspection" || _type == "Inspection")
	{
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		while (iter != _reqResourceMap.end())
		{
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				if (iter->first == acquiredResources[i])
				{
					break;
				}
			}
			map<string, Resource*>::iterator it = _resourcePool.find(iter->first);

			if (it->second->GetResourceCount() >= iter->second->GetNumResNeeded())
			{
				AcquireResourceEM(it->second, iter->second->GetNumResNeeded());
				_acquiredResources.push_back(it->first);

				if (IsInpectionFail(_inspecFailProb) == true)
				{
					isReturnStep = true;
					cout << resource->GetResourceName() << ": Inspection failed, Rescheduling appropriate maintenance step " << _returnStep << endl;
					SimExec::ScheduleEventAt(_RJpriority, new StartRepairServiceEA(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(_returnStep), resource, acquiredResources), 0, "RepairResourceEA");
				}
				else if (IsInpectionFail(_inspecFailProb) == false)
				{
					cout << "Resource Maintenance passed inspection.  Scheduling restore service." << endl;
					SimExec::ScheduleEventAt(1, new DoneRepairServiceEA(this, resource, acquiredResources), 0, "RepairStepDoneEA");
				}
			}
			else
			{
				SimExec::ScheduleConditionalEvent(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetPriority(), new ResWaitForResEA(this, resource, it->second, iter->second->GetNumResNeeded(), _acquiredResources));
				return;
			}
		}
		iter++;

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

bool isNextJob = false;

void Step::DoneServiceEM(Aircraft* aircraft, vector<string> acquiredResources)
{
	//cout << aircraft->GetAircraftType() << " " << aircraft->GetAircraftID() << " ----------NUMBER OF JOBS " << aircraft->GetMyRJMapSize() << endl;
	_acquiredResources = acquiredResources;
	//if current stepID is less than the total number of steps in my job (if this is not the last step in the job)
	if (_stepID < aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize())
	{
		int nextID = _stepID + 1;
		
		//check the next step's required resources against my acquired list
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		//for all resources in next step's required list
		while (iter != _reqResourceMap.end())
		{
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				//cout << " ____ IN CHECK FOR RESOURCES IN NEXT STEP" << endl;
				//if resource name is found in acquired vector, keep it in the vector

				//cout << " THE RETURN ID IS " << _returnStep << endl;
				//cout << " THE STEP ID IS " << _stepID << endl;
				if (aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID)->ResourceInReqResource(_acquiredResources[i]))
				{
					cout <<aircraft->GetAircraftID() << " " << aircraft->GetAircraftType() << " Retaining " << _acquiredResources[i] << " for " << this->GetName() << endl;
					//go to next resource
					//continue;
				}
				//if the resource doesn't match, release it
				else
				{
					if (_acquiredResources[i] == "S Bay" || _acquiredResources[i] == "M Bay" || _acquiredResources[i] == "L Bay") {
						continue;
					}
					else {
						cout << "ID: " << aircraft->GetAircraftID() << "Releasing " << _acquiredResources[i] << endl;
						//SimExec::ScheduleEventAt(_RJpriority, new ReleaseResourceEA(this, iter->second), 0.0, "ReleaseResourceEA");
						ReleaseResourceEM(iter->second, iter->second->GetNumResNeeded());
						//empty appropriate acquired vector index
						//cout << "-------size of acquired list " << _acquiredResources.size() << endl;
						_acquiredResources.erase(_acquiredResources.begin() + i);
						i--;
					}
				}
			}
			iter++;
		}
		//schedule the next step
		cout << endl;
		cout << "AIR ID: " << aircraft->GetAircraftID()
			<< " " << _myRJ << "'s step " << _stepID << " " 
			<< _name <<" has finished, scheduling the next maintenance step: " 
			<< aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID)->GetName() << endl;
		SimExec::ScheduleEventAt(GetRJPriority(), new StartServiceEA(aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID), aircraft, _acquiredResources), 0.0, "StartServiceEA");
	}
	//else if the current step is the last step
	else if (_stepID == aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize())
	{
		//if i have no more jobs after this one
		if (aircraft->GetMyRJMapSize() == 1/*aircraft->GetNextRepairJob(_myRJ) == NULL*/)
		{	
			cout << aircraft->GetAircraftID() << " -------NO MORE JOBS " << endl;

			//release all resources
			map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
			while (iter != _reqResourceMap.end())
			{
				cout << "ID: " << aircraft->GetAircraftID() << "Releasing " << iter->first << endl;
				//SimExec::ScheduleEventAt(_RJpriority, new ReleaseResourceEA(this, iter->second), 0.0, "ReleaseResourceEA");
				ReleaseResourceEM(iter->second, iter->second->GetNumResNeeded());
			
				//empty appropriate acquired vector index
				for (int i = 0; i < _acquiredResources.size(); i++)
				{
					//if (_acquiredResources[i] == "S Bay" || _acquiredResources[i] == "M Bay" || _acquiredResources[i] == "L Bay") {
						map<string, Resource*>::const_iterator resIt = _resourcePool.find(_acquiredResources[i]);
						ReleaseResourceEM(resIt->second, 1);
						
					//}
					_acquiredResources.erase(_acquiredResources.begin() + i);
					i--;
				}
				iter++;
			}

			//depart
			Scribe::RecordRepairEnd(aircraft->GetAircraftID(), _myRJ, SimExec::GetSimulationTime()._timeOfDay);
			this->SetNextTask(SimExec::GetSystemSink());
			//cout << "________________THIS AIRCRAFT IS DEPARTING: " << aircraft->GetAircraftID() << " " << aircraft->GetAircraftType() << endl;
			Depart(aircraft);
		}
		//else if i have more jobs
		else /*if (aircraft->GetMyRJMapSize() != 0)*/
		{
	
			//stepid is 1, because its the first step of the new job
			int nextID = 1;

			//		cout << ".....CURR INDOOR REQ IS" << _indoorReq << "--NEXT INDOOR REQ IS" << aircraft->GetMyRepairJobObj(_myRJ)->GetIndoorReq() <<endl;
		
			//if next job requires a bay and i have one already, keep it and clear all else 
			//cout << " NEXT JOB NEEDS  " << aircraft->GetNextRepairJob(_myRJ)->GetIndoorReq() << " VS " <<_indoorReq << endl;

			if (aircraft->GetNextRepairJob(_myRJ)->GetIndoorReq() == 'Y' && _indoorReq == 'Y')
			{
				//cout << "here 2" << endl;

				//cout << "TYPE: "<< aircraft->GetAircraftType() << "'s next RJ also has indoor req of Y" << endl;
				//release all resources except bay
				map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
				while (iter != _reqResourceMap.end())
				{ 
					//cout << "here 3" << endl;
					if (iter->first != "S Bay" && iter->first != "M Bay" && iter->first != "L Bay")
					{
						cout << ".................ID: "<< aircraft->GetAircraftID() << "Releasing " << iter->first << endl;
						//SimExec::ScheduleEventAt(_RJpriority, new ReleaseResourceEA(this, iter->second), 0.0, "ReleaseResourceEA");
						ReleaseResourceEM(iter->second, iter->second->GetNumResNeeded());
					}
					//empty appropriate acquired vector index
					for (int i = 0; i < _acquiredResources.size(); i++)
					{
						if (_acquiredResources[i] != "S Bay" && _acquiredResources[i] != "M Bay" && _acquiredResources[i] != "L Bay") {
							_acquiredResources.erase(_acquiredResources.begin() + i);
							i--;
						}
					}
					iter++;
				}
			}
			//else, next repair job doesn't need a bay, so clear everything.
			else
			{
				cout << " here 4" << endl;
				map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
				while (iter != _reqResourceMap.end())
				{
					cout << ".................ID: " << aircraft->GetAircraftID() << "Releasing " << iter->first << endl;
					//SimExec::ScheduleEventAt(_RJpriority, new ReleaseResourceEA(this, iter->second), 0.0, "ReleaseResourceEA");
					ReleaseResourceEM(iter->second, iter->second->GetNumResNeeded());

					//empty appropriate acquired vector index
					for (int i = 0; i < _acquiredResources.size(); ++i) {
						//if (_acquiredResources[i] == "S Bay" || _acquiredResources[i] == "M Bay" || _acquiredResources[i] == "L Bay") {
							map<string, Resource*>::const_iterator resIt = _resourcePool.find(_acquiredResources[i]);
							ReleaseResourceEM(resIt->second, 1);
						//}
					}
					_acquiredResources.clear();

					iter++;
				}
			}

			//schedule first step of new job
			SimExec::ScheduleEventAt(aircraft->GetNextRepairJob(_myRJ)->GetPriority(), new StartServiceEA(aircraft->GetNextRepairJob(_myRJ)->GetStep(nextID), aircraft, _acquiredResources), 0.0, "StartServiceEA");
			
			///does not work, possibly because getting map copy not actual map
			//cout << "---------------------- NUMBER BEFORE DELETING " << _myRJ << " IS " << aircraft->GetMyRJMapSize() << endl;
			Scribe::RecordRepairEnd(aircraft->GetAircraftID(), _myRJ, SimExec::GetSimulationTime()._timeOfDay);
			string oldJob = _myRJ;
			//cout << "------------------ old job" << oldJob;
			SetMyRJName(aircraft->GetNextRepairJob(_myRJ)->GetName());
			//cout << "  new job " << _myRJ << endl;
 			aircraft->DeleteJob(oldJob);
			//cout << aircraft->GetAircraftID() <<"---------------------- NUMBER AFTER " << aircraft->GetMyRJMapSize() << endl;

		}

	}

}

void Step::DoneRepairServiceEM(Resource* resource, vector<string> acquiredResources)
{
	_acquiredResources = acquiredResources;
	if (_stepID < RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStepVecSize())
	{
		int nextID = _stepID + 1;
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();

		while (iter != _reqResourceMap.end())
		{
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				if (RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(nextID)->ResourceInReqResource(_acquiredResources[i]))
				{
					cout << resource->GetResourceName() << " Retaining " << _acquiredResources[i] << " for " << this->GetName() << endl;
				}
				else
				{
					cout << resource->GetResourceName() << " Releasing " << _acquiredResources[i] << endl;
					ReleaseResourceEM(iter->second, iter->second->GetNumResNeeded());
					_acquiredResources.erase(_acquiredResources.begin() + i);
					i--;
				}
			}
			iter++;
		}

	}
}

void Step::AddQueueEM(Aircraft* aircraft)
{
	//_priorityQueue->AddEntity(aircraft, aircraft->GetAircraftPriority());
	//_numInQueue++;
	//InitialArrivalBayCheck();
}

void Step::AcquireResourceEM(Resource* resource, int numNeeded)
{
	int newCount;
	bool acquired = false;

	map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());
	//map<string, Resource*>::const_iterator numIt = _reqResourceMap.find(resource->GetResourceName());
	if (numNeeded <= iter->second->GetResourceCount())
	{
		acquired = true;
		Scribe::UpdateResourceUtilization(resource->GetResourceName(), numNeeded, SimExec::GetSimulationTime()._timeOfDay);
		newCount = iter->second->GetResourceCount() - numNeeded;
		//resource->SetResourceCount(newCount);
		//SetResPoolCount(resource->GetResourceName(),newCount);
		SetResPoolCount(iter->second->GetResourceName(), newCount);
	}
	
	//iter->second->SetResourceCount(newCount);
	//numIt->second->SetResourceCount(newCount);

	Scribe::UpdateResourceRequests(resource->GetResourceName(), acquired);
}

void Step::ReleaseResourceEM(Resource* resource, int numRelease)
{
	int newCount;

	map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());
	//map<string, Resource*>::const_iterator numIt = _reqResourceMap.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() + numRelease;

	//resource->SetResourceCount(newCount);
	SetResPoolCount(iter->first, newCount);
	//iter->second->SetResourceCount(newCount);
	//numIt->second->SetResourceCount(newCount);
	IsResourceReleased(iter, newCount);

	int negativeCount = numRelease * (-1);  //Used to increment utilization for scribe
	Scribe::UpdateResourceUtilization(resource->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);

	/////////******For Andrea is this where we want to put this? I feel it may be best!
	//SimExec::CheckConditionalEvents(resource, 0);
	SimExec::CheckConditionalEvents(iter->second, 0);

}


//void Step::FailResourceEM(Resource* resource)
//{
//	int newCount;
//	//Mark added this i'm not sure we need to create a new instance, but i'm just going to put a priority of 1 - Jordan
//	//RepairJob* newJob;
//
//	map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());
//
//	newCount = iter->second->GetResourceCount() - 1;
//	SetResPoolCount(iter->first, newCount);
//	//resource->SetResourceCount(newCount);
//
//	//SimExec::ScheduleEventAt(newJob->GetPriority(), new FailResourceEA(this, resource), iter->second->GetFailureDistr()->GetRV(), "New Repair Job");
//	//This Event action should actually be scheduling a restore resource instead of a fail one.
//	cout << "Resource has failed, scheduling a restore resource" << endl;
//	SimExec::ScheduleEventAt(1, new RestoreResourceEA(this, resource), this->_servTime->GetRV(), "RestoreResourceEA");
//
//	//Jordan: schedule next failure in iter->second->GetFailureDistr
//
//	Scribe::RecordFailure(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetSimulationTime()._timeOfDay);
//}
//
//void Step::RestoreResourceEM(Resource* resource)
//{
//	cout << "Resource has been restored, updating amount and checking conditional events" << endl;
//	resource->RestoreResource();
//	SimExec::CheckConditionalEvents(resource, 0);
//}


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
		cout << "failure" << endl;
		return true;
	}
	else return false;
	//return true;
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

int Step::GetStepID() {

	return _stepID;

}

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
	// Testing //
	vector<string> test;
	SimExec::ScheduleEventAtRecurring(_RJpriority, new StartServiceEA(step, aircraft, test), 0.0, "StartServiceEA");
	//SimExec::ScheduleEventAtRecurring(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "StartServiceEA");
	cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
}

void Step::ScheduleCalendarStep(Step* step, Aircraft* aircraft, CalendarObj* calobj)
{
	vector<string> test;
	for (int i = 0; i < 1; ++i) {
		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _RJpriority, new StartServiceEA(step, aircraft, test), "StartServiceEA");
		//SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), "StartServiceEA");
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

char Step::GetRJIndoorReq()
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
//	cout << "JOB IS " << this->GetMyRJName() << endl;
//	cout << "STEP IS " << this->GetName() << endl;
//		cout << "step id " << stepID << " " << _stepID << endl;
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

void Step::SetStepBayReq(char baySizeReq)
{
	_baySizeReq = baySizeReq;
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

void Step::SetReqResource(string reqResource/*, Resource* newResource*/)
{
	//change this to check for while finding &
	istringstream res(reqResource);
	string first;
	string sec;
	string third;

	string line;


	while (getline(res, line, '&'))
	{
		//	cout << "LINE: " << line << endl;
		Resource* newResource = new Resource();
		istringstream ss(line);
		string resourceName;
		string numString;
		int num;

		getline(ss, resourceName, '(');
		getline(ss, numString, ')');

		//	cout << "	R: " << resource << "	N: " << numString << endl;

		istringstream ssNum(numString);
		ssNum >> num;

		//	cout << "	R: " << resource << "	N: " << num << endl;


		//newResource = new Resource();
		//AddResource(newResource, resource, num);
		AddResource(newResource, resourceName, num);

	}

}

void Step::SetReqParts(string reqParts, int numNeeded)
{
	//Parts* newParts = new Parts();
	istringstream part(reqParts);
	string first;
	string sec;
	string third;

	getline(part, first, '&');
	getline(part, sec, '&');
	getline(part, third, '&');

	if (first != "")
	{
		Parts* newParts = new Parts();
		AddParts(newParts, first, numNeeded);
	}
	if (sec != "")
	{
		Parts* newParts = new Parts();
		AddParts(newParts, sec, numNeeded);
	}
	if (third != "")
	{
		Parts* newParts = new Parts();
		AddParts(newParts, third, numNeeded);
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
	vector<string> test;
	SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, test), 0.0, "StartServiceEA");
	//SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "StartServiceEA");
	cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
	////	SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "AddToQueueEA");
}


map<string, Parts*> Step::GetPartsPool()
{
	return _partsPool;
}

map<string, Resource*> Step::GetResPool()
{
	return _resourcePool;
}

int Step::GetPartsPoolSize()
{
	return _partsPool.size();
}

int Step::GetResPoolSize()
{
	return _resourcePool.size();
}

void Step::AddToResPool(Resource* resource, string resourceName)
{
	_resourcePool[resourceName] = resource;
}

void Step::AddToPartsPool(Parts* parts, string partsName)
{
	//cout << "adding " << partsName << endl; 
//	parts->PrintPartsProperties();
	//cout << "wow \n";
	_partsPool[partsName] = parts;
}
  
void Step::SetResPoolCount(string resource, int newCount)
{
	_resourcePool.find(resource)->second->SetResourceCount(newCount);
}

void Step::SetPartPoolCount(string part, int newCount)
{
	_partsPool.find(part)->second->SetPartsCount(newCount);
	//cout << part << "'s new count is " << newCount << endl;
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
	//_reqResourceMap.insert(pair<string, Resource*>(resourceName, resource));
	_reqResourceMap[resourceName] = resource;
//	cout << " RES POINTER " << _reqResourceMap.find(resourceName)->first << endl;

	if (resourceName == "S Bay" || resourceName == "M Bay" || resourceName == "L Bay")
		numNeeded = 1;
	_reqResourceMap[resourceName]->SetNumResNeeded(numNeeded);
	//cout << "INSERTED " << _reqResourceMap[resourceName]->GetResourceName() << endl;

}

//void SchedResourceFailure()
//{
//	//schedule resource failure logic
//	map<string, Resource*>::const_iterator iter = InputReader::GetMasterResMapBegin();
//	while (iter != InputReader::GetMasterResMapEnd())
//	{
//		cout << "" << endl;
//		//schedule iter's first failure in iter->second->GetFailureDistr()
//
//		iter++;
//	}
//
//}

int Step::GetResMapSize()
{
	return _reqResourceMap.size();
}

int Step::GetPartsMapSize()
{
	return _reqPartsMap.size();
}

void Step::AddParts(Parts* parts, string partsName, int numNeeded)
{
	//cout << " IN ADD PARTS " << partsName << endl;
	//cout << " PARTS POINTER " << parts->GetPartsName() << endl;
	_reqPartsMap[partsName] = parts;
	_reqPartsMap[partsName]->SetNumPartsNeeded(numNeeded);
	//cout << "INSERTED PARTS" << _reqPartsMap[partsName]->GetPartsName() << endl;

}

void Step::AcquireParts(Parts* parts, int numNeeded)
{
	int newCount;

	//cout << " ______ PART " << parts->GetPartsName()<<" THRESHOLD " << parts->GetThreshold() << endl;

	map<string, Parts*>::const_iterator iter = _partsPool.find(parts->GetPartsName());
	//map<string, Resource*>::const_iterator numIt = _reqResourceMap.find(resource->GetResourceName());
	if (numNeeded <= iter->second->GetPartsCount())
	{
	//	cout << "now acquiring " << iter->first << endl;
	//	cout << "current count is " << iter->second->GetPartsCount() << endl;
	//	cout << "num needed " << numNeeded << endl;
		newCount = iter->second->GetPartsCount() - numNeeded;
		SetPartPoolCount(iter->first, newCount);
	//	cout << " after set parts pool count " << endl;

		if (newCount <= iter->second->GetThreshold())
		{
	//		cout << "Parts Count reached threshold, ordering more " << iter->first << endl;
			SimExec::ScheduleEventAt(1, new PlaceOrderEA(this, iter->second), 0.0, "PlaceOrderEA");
		}
	}

	//if (iter->second->GetPartsCount() <= iter->second->GetThreshold())
	//{
	//	cout << "Parts Count reached threshold, ordering more " << iter->first << endl;
	//	SimExec::ScheduleEventAt(1, new PlaceOrderEA(this, iter->second), 0.0, "PlaceOrderEA");
	//}
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
	else
	{
		cout << "Service time is ";
		_inspecFailProb->PrintDistribution();
		cout << endl;
	}
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
		//cout << "in the loop \n"

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