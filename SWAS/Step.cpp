#include "Step.h"
#include "Aircraft.h" //need to remove and replace priority values with 
#include <iterator>
#include "InputInterface.h"
#include "SinkTask.h"
#include <map>

map<string, StepResource*> Step::_resourcePool;
map<string, Parts*> Step::_partsPool;
map<string, RepairJob*> RepairJob::_resourceRepairMap;
bool isNextStepReturnStep = false;
bool isFail = false;
int Step::_firstShiftUpdateFlag = 1;

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
		//////cout << "COPYING INSPEC FAIL ";
		//_inspecFailProb->PrintDistribution();
		//////cout << endl;
	}
	_servTime = mapStep._servTime; // does this need to have copythis()
	_reqRes = mapStep._reqRes;
	_reqParts = mapStep._reqParts;
	if (mapStep._returnStep == NULL)
		_returnStep = NULL;
	else
		_returnStep = mapStep._returnStep;

	//iterate through vectors/nonstatic maps to use resource and parts and aircraft object copy constructors

	//iterating through old required resource map, inserting its first and second into the new required resource map
	map<string, StepResource*>::const_iterator reqResIter = mapStep._reqResourceMap.begin();
	while (reqResIter != mapStep._reqResourceMap.end())
	{
		//////cout << " iN COPY STEP " << reqResIter->first << endl;
		StepResource* newRes = new StepResource();
		newRes->CopyMapResource(*reqResIter->second);
		_reqResourceMap.insert(pair<string, StepResource*>(reqResIter->first, newRes));

		reqResIter++;
	}

	map<string, Parts*>::const_iterator reqPartsIter = mapStep._reqPartsMap.begin();
	while (reqPartsIter != mapStep._reqPartsMap.end())
	{
		Parts* newParts = new Parts();
		//		////cout << " iN COPY STEP, part is " << reqPartsIter->first << endl;

				//////cout << " parts test in copy step ";
				//reqPartsIter->second->GetLeadTime()->PrintDistribution();
		newParts->CopyMapParts(*reqPartsIter->second);
		_reqPartsMap.insert(pair<string, Parts*>(reqPartsIter->first, newParts));

		reqPartsIter++;
	}
}


////////////////////////////////////////////
/////////EVENT ACTIONS AND METHODS//////////
////////////////////////////////////////////

class Step::ResWaitForResEA : public CondEventAction {
public:
	ResWaitForResEA(Step* step, StepResource* resBeingRepaired, StepResource* resNeeded, int amountNeeded, map<string, int> acqResources) {
		_step = step;
		_resBeingRepaired = resBeingRepaired;
		_resNeeded = resNeeded;
		_amountNeeded = amountNeeded;
		_acqResources = acqResources;
	}

	bool Condition(StepResource* resource, Parts* parts) {
		if (resource == _resNeeded) {
			if (_resNeeded->GetResourceCount() >= _amountNeeded)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	void Execute() {
		_resBeingRepaired->SetCELflag(1);
		_step->StartRepairServiceEM(_resBeingRepaired, _acqResources);
	}

private:
	StepResource* _resBeingRepaired;
	StepResource* _resNeeded;
	Step* _step;
	int _amountNeeded;
	string _funcName;
	map<string, int> _acqResources;
};

class Step::WaitForResourceEA : public CondEventAction {
public:
	WaitForResourceEA(Step* step, StepResource* resource, Aircraft* aircraft, int amountNeeded, map<string, int> acqResources) {
		_step = step;
		_resource = resource;
		_aircraft = aircraft;
		_amountNeeded = amountNeeded;
		_acqResources = acqResources;
	}

	bool Condition(StepResource* resource, Parts* parts) {
		if (resource == 0)
			return false;
		else if (resource->GetResourceName() == _resource->GetResourceName()) {
			map<string, StepResource*>::const_iterator it = _resourcePool.find(_resource->GetResourceName());
			if (it->second->GetResourceCount() >= _amountNeeded)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	void Execute() {

		_aircraft->SetCELflag(1);
		Scribe::RecordResourceWaitEnd(_aircraft->GetAircraftID(), _resource->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
		_step->StartServiceEM(_aircraft, _acqResources);
	}

private:
	StepResource* _resource;
	Step* _step;
	Aircraft* _aircraft;
	int _amountNeeded;
	string _funcName;
	map<string, int> _acqResources;
};

class Step::ResNeedPartsEA : public CondEventAction {
public:
	ResNeedPartsEA(Step* step, Parts* parts, StepResource* resBeingRepaired, int amountNeeded, map<string, int> acqResources) {
		_step = step;
		_parts = parts;
		_resBeingRepaired = resBeingRepaired;
		_amountNeeded = amountNeeded;
		_acqResources = acqResources;
	}

	bool Condition(StepResource* resource, Parts* parts) {
		if (parts == _parts) {
			if (_parts->GetPartsCount() >= _amountNeeded)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	void Execute() {

		_resBeingRepaired->SetCELflag(1);
		_step->StartRepairServiceEM(_resBeingRepaired, _acqResources);
	}

private:
	Parts* _parts;
	Step* _step;
	StepResource* _resBeingRepaired;
	int _amountNeeded;
	map<string, int> _acqResources;
};

class Step::NeedPartsEA : public CondEventAction {
public:
	NeedPartsEA(Step* step, Parts* parts, Aircraft* aircraft, int amountNeeded, map<string, int> acqResources) {
		_step = step;
		_parts = parts;
		_aircraft = aircraft;
		_amountNeeded = amountNeeded;
		_acqResources = acqResources;
	}

	bool Condition(StepResource* resource, Parts* parts) {
		if (parts == _parts) {
			if (_parts->GetPartsCount() >= _amountNeeded)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	void Execute() {

		_aircraft->SetCELflag(1);
		_step->StartServiceEM(_aircraft, _acqResources);
	}

private:
	Parts* _parts;
	Step* _step;
	Aircraft* _aircraft;
	int _amountNeeded;
	map<string, int> _acqResources;
};

class Step::NeedBaysEA : public CondEventAction {
public:
	NeedBaysEA(Step* step, Aircraft* aircraft, map<string, int> acqResources, StepResource* bay, int numNeeded) {
		_step = step;
		_aircraft = aircraft;
		_acqResources = acqResources;
		_bay = bay;
		_numNeeded = numNeeded;
	}

	bool Condition(StepResource* resource, Parts* parts) {
		if (resource == _bay) {
			if (_bay->GetResourceCount() >= _numNeeded)
				return true;
			else {
				if (_step->AreThereBaysAvailable(_bay->GetResourceName()))
					return true;
				else
					return false;
			}
		}
		else
			return false;
	}

	void Execute() {
		_step->StartServiceEM(_aircraft, _acqResources);
	}

private:
	Step* _step;
	Aircraft* _aircraft;
	map<string, int> _acqResources;
	StepResource* _bay;
	int _numNeeded;
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
	StartServiceEA(Step* step, Aircraft* aircraft, map<string, int> acqResources) {
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
	map<string, int> _acqResources;
};

class Step::ShiftChangeEA : public EventAction {
public:
	ShiftChangeEA() {
	}

	void Execute() 
	{
		UpdateShift();
	}
};

class Step::StartRepairServiceEA : public EventAction {
public:
	StartRepairServiceEA(Step* step, StepResource* resource, map<string, int> acqResources) {
		_step = step;
		_resource = resource;
		_acqResources = acqResources;
	}

	void Execute() {
		_step->StartRepairServiceEM(_resource, _acqResources);
	}
private:
	Step* _step;
	StepResource* _resource;
	map<string, int> _acqResources;
};

class Step::DoneServiceEA : public EventAction {
public:
	DoneServiceEA(Step* step, Aircraft* aircraft, map<string, int> acqResources)
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
	map<string, int> _acqResources;
};

class Step::DoneResourceServiceEA : public EventAction {
public:
	DoneResourceServiceEA(Step* step, StepResource* resource, map<string, int> acqResources)
	{
		_step = step;
		_resource = resource;
		_acqResources = acqResources;
	}

	void Execute()
	{
		_step->DoneResourceServiceEM(_resource, _acqResources);
	}
private:
	Step* _step;
	StepResource* _resource;
	map<string, int> _acqResources;
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
	AcquireResourceEA(Step* step, StepResource* resource)
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
	StepResource* _resource;
	int _numNeeded;
};

class Step::ReleaseResourceEA : public EventAction {
public:
	ReleaseResourceEA(Step* step, StepResource* resource) {
		_step = step;
		_resource = resource;
		_numRelease = 0;
	}

	void Execute() {
		_step->ReleaseResourceEM(_resource, _numRelease);
	}
private:
	Step* _step;
	StepResource* _resource;
	int _numRelease;
};

class Step::FailResourceEA : public EventAction {
public:
	FailResourceEA(Step* step, StepResource* resource) {
		_step = step;
		_resource = resource;
	}

	void Execute() {
		_step->FailResourceEM(_resource);
	}
private:
	Step* _step;
	StepResource* _resource;
};

class Step::RestoreResourceEA : public EventAction {
public:
	RestoreResourceEA(Step* step, StepResource* resource) {
		_step = step;
		_resource = resource;
	}

	void Execute() {
		_step->RestoreResourceEM(_resource);
	}

private:
	Step* _step;
	StepResource* _resource;
};

// PLACE ORDER/ REPLENISH order process
	// schedule an event that will act as "place order"   -> placeorderEA does placeOrderEM
	// "place order" execution will schedule a "order arrived/ parts replenish event"	 placeOrderEM schedules partsArrivalEA
	// "order arrived/ parts replenish event" execution will simply do _partsCount + Number ordered -> partsArrivalEA does partsArrivalEM

void Step::UpdateShift()
{ 
	double newCount;
	double lastShiftsInitCount;
	double thisShiftsInitCount;

	//we don't want to change the counts for the first 6am shift
	if (isFirstShiftChange() == true)
	{
		SetFirstShiftUpdateFlag(0);
		////schedule this same shift change tomorrow at the same time
		SimExec::ScheduleEventAt(-10, new ShiftChangeEA(), 1.0, "ShiftChangeEA", 1);
		return;
	}
	////if its a wartime simulation
	if (InputReader::IsWartime() == true)
	{

		////if i'm in shift one
		if (InputReader::GetShiftOneStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			////for all resources in the resource pool, update the count
			map<string, StepResource*>::iterator iter = _resourcePool.begin();
			while (iter != _resourcePool.end())
			{
				////if there's a difference in the counts
				if (iter->second->GetShiftTwoCount() != iter->second->GetShiftOneCount())
				{
					////new count is =
					////current count + the difference between last shift's intitial count and this shift's initial count
					lastShiftsInitCount = iter->second->GetShiftTwoCount();

					thisShiftsInitCount = iter->second->GetShiftOneCount();

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);
					iter->second->SetResourceCount(newCount);
				}

				iter++;
			}			
		}
		////else i'm in shift two
		else if(InputReader::GetShiftTwoStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			////for all resources in the resource pool, update the count
			map<string, StepResource*>::iterator iter = _resourcePool.begin();
			while (iter != _resourcePool.end())
			{
				////if there's a difference in the counts
				if (iter->second->GetShiftTwoCount() != iter->second->GetShiftOneCount())
				{
					////new count is 
					////current count + the difference between last shift's intitial count and this shift's initial count
					lastShiftsInitCount = iter->second->GetShiftOneCount();

					thisShiftsInitCount = iter->second->GetShiftTwoCount();

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);
		
					iter->second->SetResourceCount(newCount);
				}
				iter++;
			}
			//cout << "after loop" << endl;

		}
		else
			cout << "ERROR: trying to schedule in a shift that doesnt exist" << endl;
	}

	////else its a peacetime simulation
	else //if (InputReader::IsWartime() == false)
	{
		////if i'm in shift one
		if (InputReader::GetShiftOneStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			////for all resources in the resource pool, update the count
			map<string, StepResource*>::iterator iter = _resourcePool.begin();
			while (iter != _resourcePool.end())
			{
				////if there's a difference in the counts
				if (iter->second->GetShiftThreeCount() != iter->second->GetShiftOneCount())
				{
					////new count is =
					//current count + the difference between last shift's intitial count and this shift's initial count
					lastShiftsInitCount = iter->second->GetShiftThreeCount();

					thisShiftsInitCount = iter->second->GetShiftOneCount();

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);

					iter->second->SetResourceCount(newCount);
				}

				iter++;
			}
		}

		////else if i'm in shift two
		else if (InputReader::GetShiftTwoStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			////for all resources in the resource pool, update the count
			map<string, StepResource*>::iterator iter = _resourcePool.begin();
			while (iter != _resourcePool.end())
			{
				////if there's a difference in the counts
				if (iter->second->GetShiftThreeCount() != iter->second->GetShiftOneCount())
				{
					////new count is 
					////current count + the difference between last shift's intitial count and this shift's initial count
					lastShiftsInitCount = iter->second->GetShiftOneCount();

					thisShiftsInitCount = iter->second->GetShiftTwoCount();

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);

					iter->second->SetResourceCount(newCount);
				}
				iter++;
			}

		}

		////else i'm in shift three
		else if (InputReader::GetShiftThreeStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			////for all resources in the resource pool, update the count
			map<string, StepResource*>::iterator iter = _resourcePool.begin();
			while (iter != _resourcePool.end())
			{
				////if there's a difference in the counts
				if (iter->second->GetShiftThreeCount() != iter->second->GetShiftOneCount())
				{
					////new count is 
					////current count + the difference between last shift's intitial count and this shift's initial count
					lastShiftsInitCount = iter->second->GetShiftTwoCount();

					thisShiftsInitCount = iter->second->GetShiftThreeCount();

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);

					iter->second->SetResourceCount(newCount);
				}
				iter++;
			}
		}
		else
			cout << "ERROR: trying to schedule in a shift that doesnt exist" << endl;
	}

	////schedule this same shift change tomorrow at the same time
	SimExec::ScheduleEventAt(-10, new ShiftChangeEA(), 1.0, "ShiftChangeEA", 1);
}

void Step::PlaceOrderEM(Parts* parts)
{
	double restockTime = parts->GetLeadTime()->GetRV();
	Scribe::RecordRestock(parts->GetPartsName(), restockTime);
	SimExec::ScheduleEventAt(0, new OrderArrivalEA(this, parts), restockTime, "OrderArrivalEA");
}

void Step::OrderArrivalEM(Parts* parts)
{
	int newCount;

	map<string, Parts*>::iterator iter = _partsPool.find(parts->GetPartsName());
	newCount = iter->second->GetInitPartsCount();

	SetPartPoolCount(iter->first, newCount);

	SimExec::CheckConditionalEvents(0, parts);

	if (iter->second->GetPartsCount() <= iter->second->GetThreshold())
		SimExec::ScheduleEventAt(0, new PlaceOrderEA(this, iter->second), 0.0, "PlaceOrderEA");
}

int Step::GetReturnStep()
{
	return _returnStep;
}

void Step::StartServiceEM(Aircraft* aircraft, map<string, int> acquiredResources)
{
	isNextStepReturnStep = false;

	if (aircraft->IsAfterCEL() == true)
	{
		aircraft->SetCELflag(0);
	}

	_acquiredResources = acquiredResources;
	Scribe::RecordRepairJob(aircraft->GetAircraftType(), aircraft->GetAircraftID(), _myRJ, _stepID, aircraft->GetMyRepairJobObj(_myRJ)->GetStep(_stepID)->GetName(), SimExec::GetSimulationTime()._timeOfDay);
	Scribe::RecordServiceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), "Bay", SimExec::GetSimulationTime()._timeOfDay);

	bool hasResource = false;

	map<string, int>::iterator it = _acquiredResources.begin();
	while (it != _acquiredResources.end())
	{

		if (_indoorReq == 'Y' || _indoorReq == 'y') {
			if (it->first == "S Bay" || it->first == "M Bay" || it->first == "L Bay") {
				hasResource = true;
				break;
			}
		}	

		it++;
	}

	// if i don't have a bay 
	if (hasResource == false)
	{
		if (_indoorReq == 'Y' || _indoorReq == 'y')
		{
			//TODO need to check for specific bay size
			string bayReq = aircraft->GetBaySizeReq();
			map<string, StepResource*>::iterator it = _resourcePool.find(bayReq);
			if (it != _resourcePool.end())
			{
				if (IsMyBaySizeAvailable(it->first)) {
					_acquiredResources.insert(make_pair(it->first,1));
					AcquireBay(it->second, 1);
				}
				else {
					if (AreThereBaysAvailable(it->first)) {
						string bay = AcquireBay(it->second, 1);
						if (WasBayAcquired(bay)) {
							_acquiredResources.insert(make_pair(bay,1));
						}
						else {
							SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedBaysEA(this, aircraft, _acquiredResources, it->second, 1),
								"WaitForResourceEA", aircraft->GetAircraftType(), it->first, aircraft->GetAircraftID(), _myRJ);
							
							Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), it->second->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
							Scribe::UpdateResourceRequests(it->second->GetResourceName(), false);
							return;
						}

					}
					else {
						SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedBaysEA(this, aircraft, _acquiredResources, it->second, 1), "WaitForResourceEA",
							aircraft->GetAircraftType(), it->first, aircraft->GetAircraftID(), _myRJ);
						Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), it->second->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);
						Scribe::UpdateResourceRequests(it->second->GetResourceName(), false);
						return;
					}
				}
			}
		}
	}

	if (_type == "process" || _type == "Process")
	{
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
		cout << " YO I AM A JOB STEP " << _myRJ << " " << _name << " INDOOR REQ IS " << _indoorReq << " TYPE " << _type << endl;
		//for all resources listed in required map (if it has any)
		while (iter != _reqResourceMap.end())
		{
			cout << "MY RESOURCES ARE: ";
				cout << iter->first << endl;

			bool alreadyAcquired = false;
			//compare key to acquired resources vector
			// if i don't have any resources (besides a bay if i'm an indoor step)
			if (_acquiredResources.size() == 0)
			{
				//if what i need is more than what's avail, wait
				if (iter->second->GetNumberOfResourcesNeeded() > _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, iter->second, aircraft,
						iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "WaitForResourceEA", aircraft->GetAircraftType(), iter->first,
						aircraft->GetAircraftID(), _myRJ);

					//record waits
					Scribe::UpdateResourceRequests(iter->second->GetResourceName(), false);
					Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), iter->second->GetResourceName(),
						SimExec::GetSimulationTime()._timeOfDay);
					return;
				}
				//else if what i need is less than or = what's avail
				else if (iter->second->GetNumberOfResourcesNeeded() <= _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());

					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
				}
				continue;
			}

			bool foundIt = false;

			map<string, int>::iterator it = _acquiredResources.begin();
			while (it != _acquiredResources.end())
			{
				if (iter->first == it->first) {
					foundIt = true;
					if (iter->first == "S Bay" || iter->first == "M Bay" || iter->first == "L Bay") {
						alreadyAcquired = true;
						break;
					}

					alreadyAcquired = true;

					//i've already acquired this resource - if i need more of it
					if (it->second < iter->second->GetNumberOfResourcesNeeded())
					{
						//i've already acquired this resource - if i need more than are avail, wait
						if ((iter->second->GetNumberOfResourcesNeeded() - it->second) > _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							//if i have any resource and i need to wait, my priority increases
							if (_indoorReq == 'N' || (_indoorReq == 'Y' && _acquiredResources.size() > 1))
							{
								SimExec::ScheduleConditionalEvent((-1 * _acquiredResources.size()), new WaitForResourceEA(this, iter->second,
									aircraft, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "WaitForResourceEA", aircraft->GetAircraftType(),
									iter->first, aircraft->GetAircraftID(), _myRJ);
							}
							else
							{
								SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, iter->second,
									aircraft, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "WaitForResourceEA", aircraft->GetAircraftType(),
									iter->first, aircraft->GetAircraftID(), _myRJ);
							}
							Scribe::UpdateResourceRequests(iter->second->GetResourceName(), false);
							Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), iter->second->GetResourceName(),
								SimExec::GetSimulationTime()._timeOfDay);
							return;
						}

						//i've already acquired this resource - if i need more of it, & there are enough avail
						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) < _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							//acquire the difference
							AcquireResourceEM(iter->second, (iter->second->GetNumberOfResourcesNeeded() - it->second));
							it->second += (iter->second->GetNumberOfResourcesNeeded() - it->second);
						}

						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) == _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
						}
					}
				}
				it++;
			}
			//didn't find this resource
			if (foundIt == false)
			{
				if (iter->second->GetNumberOfResourcesNeeded() <= _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
				}
				else {
					SimExec::ScheduleConditionalEvent((-1 * _acquiredResources.size()), new WaitForResourceEA(this, iter->second,
						aircraft, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "WaitForResourceEA", aircraft->GetAircraftType(),
						iter->first, aircraft->GetAircraftID(), _myRJ);
					Scribe::UpdateResourceRequests(iter->second->GetResourceName(), false);
					Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), iter->second->GetResourceName(),
						SimExec::GetSimulationTime()._timeOfDay);
					return;
				}
			}
			iter++;
		}

		//for all parts listed in required map
		if (_reqPartsMap.size() > 0)
		{
			map<string, Parts*>::iterator iterParts = _reqPartsMap.begin();
			while (iterParts != _reqPartsMap.end())
			{
				map<string, Parts*>::iterator it = _partsPool.find(iterParts->first);

				//if (it->second == NULL) {
				if (it->second->AreEnoughParts() == true)
				{
					AcquireParts(iterParts->second, iterParts->second->GetNumPartsNeeded());
					Scribe::RecordPartRequest(iterParts->first, iterParts->second->GetNumPartsNeeded(), true);
				}

				else {
					Scribe::RecordPartRequest(iterParts->first, iterParts->second->GetNumPartsNeeded(), false);
					SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new NeedPartsEA(this, it->second, aircraft, it->second->GetNumPartsNeeded(), _acquiredResources), "NeedPartsEA", aircraft->GetAircraftType(), it->first, aircraft->GetAircraftID(), _myRJ);
					return;
				}
				iterParts++;
			}	
		}

		DoneServiceEA* doneEA = new DoneServiceEA(this, aircraft, _acquiredResources);
		SimExec::ScheduleEventAt(1, doneEA, this->_servTime->GetRV(), "DoneServiceEA");

		Scribe::RecordServiceWaitEnd(aircraft->GetAircraftID(), "Bay", SimExec::GetSimulationTime()._timeOfDay);

	}

	else if (_type == "inspection" || _type == "Inspection")
	{
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();

		//for all resources listed in required map
		while (iter != _reqResourceMap.end())
		{
			bool alreadyAcquired = false;

			//if i have no resources
			if(_acquiredResources.size() == 0)
			{
				//if what i need is more than what's avail, wait
				if (iter->second->GetNumberOfResourcesNeeded() > _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, iter->second, aircraft,
						iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "WaitForResourceEA", aircraft->GetAircraftType(), iter->first,
						aircraft->GetAircraftID(), _myRJ);

					//record waits
					Scribe::UpdateResourceRequests(iter->second->GetResourceName(), false);
					Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), iter->second->GetResourceName(),
						SimExec::GetSimulationTime()._timeOfDay);
					return;
				}
				//else if what i need is less than or equal what's avail
				else
				{
					//acquire them
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
				}
				continue;
			}
			//compare key to acquired resources map
			while (it != _acquiredResources.end())
			{
				if(iter->first == "S Bay" || iter->first == "M Bay" || iter->first == "L Bay") {
					alreadyAcquired = true;
					break;
				}
				if (iter->first == it->first) {
					alreadyAcquired = true;

					//Compare what i have (acquired) with what i need (reqresourceneed)
					//if what i have is greater, release the difference
					if (it->second > iter->second->GetNumberOfResourcesNeeded())
					{
						ReleaseResourceEM(iter->second, it->second - iter->second->GetNumberOfResourcesNeeded());

						it->second -= it->second - iter->second->GetNumberOfResourcesNeeded();
					}
					//if what i have is less, acquire the difference
					else if (it->second < iter->second->GetNumberOfResourcesNeeded())
					{
						//if i need more than are avail
						if (iter->second->GetNumberOfResourcesNeeded() - it->second > _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							//if i have some, my priority increases
							if (_indoorReq == 'N' || (_indoorReq == 'Y' && _acquiredResources.size() > 1))
							{
								SimExec::ScheduleConditionalEvent((-1 * _acquiredResources.size()), new WaitForResourceEA(this, iter->second, 
									aircraft, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "WaitForResourceEA", aircraft->GetAircraftType(),
									iter->first, aircraft->GetAircraftID(), _myRJ);
							}
							else
							{
								SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, iter->second,
									aircraft, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "WaitForResourceEA", aircraft->GetAircraftType(),
									iter->first, aircraft->GetAircraftID(), _myRJ);
							}
							Scribe::UpdateResourceRequests(iter->second->GetResourceName(), false);
							Scribe::RecordResourceWait(aircraft->GetAircraftType(), aircraft->GetAircraftID(), iter->second->GetResourceName(),
								SimExec::GetSimulationTime()._timeOfDay);
							return;
						}
						//else if there are enough avail
						else if(iter->second->GetNumberOfResourcesNeeded() - it->second < _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							//acquire the difference
							AcquireResourceEM(iter->second, (iter->second->GetNumberOfResourcesNeeded() - it->second));

							//pushback the difference so we know the number acquired
							if (it->first == iter->first)
							{
								it->second += (iter->second->GetNumberOfResourcesNeeded() - it->second);
							}
						}
						else if (iter->second->GetNumberOfResourcesNeeded() - it->second == _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
						}
					}
				}
				it++;
			}
			iter++;
		}
		
		isFail = IsInpectionFail(_inspecFailProb);

		if (isFail == true)
		{

			isNextStepReturnStep = true;
			
			SimExec::ScheduleEventAt(1, new DoneServiceEA(this, aircraft, _acquiredResources), _servTime->GetRV(), "DoneServiceEA");

			Scribe::RecordInspectionFailure(aircraft->GetAircraftID(), aircraft->GetAircraftType(), _myRJ, _stepID);
			Scribe::RecordRework(aircraft->GetAircraftType(), _myRJ, SimExec::GetSimulationTime()._timeOfDay);

			return;
		}
		else if (isFail == false)
		{
			SimExec::ScheduleEventAt(1, new DoneServiceEA(this, aircraft, _acquiredResources), _servTime->GetRV(), "DoneServiceEA");
		}
	}
}

void Step::StartRepairServiceEM(StepResource* resource, map<string, int> acquiredResources)
{ 
	if (isNextStepReturnStep == true)
	{
		SetStepID(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetMyReturnStep());
	}
	else
	{
		Scribe::RecordFailure(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetTotalSimulationTime());
	}


	if (resource->IsAfterCEL() == true)
	{
		resource->SetCELflag(0);
	}

	_acquiredResources = acquiredResources;

	bool hasResource = false;

	if (_type == "process" || _type == "Process")
	{
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
		if (_acquiredResources.size() > 0)
		{
			map<string, int>::iterator it = _acquiredResources.begin();
			while (it != _acquiredResources.end())
			{
				cout << it->first << " " << it->second << endl;
				it++;
			}
		}

		//for all resources listed in required map (if it has any)
		while (iter != _reqResourceMap.end())
		{
			bool alreadyAcquired = false;
			//compare key to acquired resources vector
			// if i don't have any resources (besides a bay if i'm an indoor step)
			if (_acquiredResources.size() == 0)
			{
				//if what i need is more than what's avail, wait
				if (iter->second->GetNumberOfResourcesNeeded() > _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					SimExec::ScheduleConditionalEvent(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetPriority(), 
						new ResWaitForResEA(this, resource, iter->second, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), 
						"ResWaitForResEA", iter->first, resource->GetResourceName(), 77.7, _myRJ);

					//record waits
					Scribe::UpdateResourceRequests(iter->second->GetResourceName(), false);
					Scribe::RecordResourceWait(resource->GetResourceName(), 0, iter->second->GetResourceName(),
						SimExec::GetSimulationTime()._timeOfDay);
					return;
				}
				//else if what i need is less than or = what's avail
				else if (iter->second->GetNumberOfResourcesNeeded() <= _resourcePool.find(iter->first)->second->GetResourceCount())
				{
						//acquire them
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
				}
				continue;
			}

			bool foundIt = false;

			map<string, int>::iterator it = _acquiredResources.begin();
			while (it != _acquiredResources.end())
			{
				if (iter->first == it->first) {
					foundIt = true;
					
					alreadyAcquired = true;

					//i've already acquired this resource - if i need more of it
					if (it->second < iter->second->GetNumberOfResourcesNeeded())
					{
						//i've already acquired this resource - if i need more than are avail, wait
						if ((iter->second->GetNumberOfResourcesNeeded() - it->second) > _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							SimExec::ScheduleConditionalEvent((RepairJob::GetMyResRepairJobObj(_myRJ)->GetPriority()), new ResWaitForResEA(this, iter->second,
								resource, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "ResWaitForResEA", resource->GetResourceName(),
								iter->first, 0, _myRJ);

							return;
						}
						//i've already acquired this resource - if i need more of it, & there are enough avail
						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) < _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							//acquire the difference
							AcquireResourceEM(iter->second, (iter->second->GetNumberOfResourcesNeeded() - it->second));

							it->second += (iter->second->GetNumberOfResourcesNeeded() - it->second);
						}
						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) == _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
						}
					}
				}
				it++;
			}
			//didn't find this resource
			if (foundIt == false)
			{
				if (iter->second->GetNumberOfResourcesNeeded() <= _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
				}
				else {
					SimExec::ScheduleConditionalEvent((RepairJob::GetMyResRepairJobObj(_myRJ)->GetPriority()),
						new ResWaitForResEA(this, iter->second, resource, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources),
						"ResWaitForResEA", resource->GetResourceName(), iter->first, 0, _myRJ);
					return;
				}
			}
			iter++;
		}

		//for all parts listed in required map
		if (_reqPartsMap.size() > 0)
		{
			map<string, Parts*>::iterator iterParts = _reqPartsMap.begin();
			while (iterParts != _reqPartsMap.end())
			{
				map<string, Parts*>::iterator it = _partsPool.find(iterParts->first);

				if (it->second->AreEnoughParts() == true)
				{
					AcquireParts(iterParts->second, iterParts->second->GetNumPartsNeeded());
					Scribe::RecordPartRequest(iterParts->first, iterParts->second->GetNumPartsNeeded(), true);
				}

				else {
					Scribe::RecordPartRequest(iterParts->first, iterParts->second->GetNumPartsNeeded(), false);
					SimExec::ScheduleConditionalEvent(RepairJob::GetMyResRepairJobObj(_myRJ)->GetPriority(), 
						new ResNeedPartsEA(this, it->second, resource, it->second->GetNumPartsNeeded(), _acquiredResources), 
						"NeedPartsEA", resource->GetResourceName(), it->first,0, _myRJ);
					return;
				}
				iterParts++;
			}
		}

		DoneResourceServiceEA* doneEA = new DoneResourceServiceEA(this, resource, _acquiredResources);
		SimExec::ScheduleEventAt(RepairJob::GetMyResRepairJobObj(_myRJ)->GetPriority(), doneEA, this->_servTime->GetRV(), "DoneServiceEA");

	}

	else if (_type == "inspection" || _type == "Inspection")
	{
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();

		//for all resources listed in required map (if it has any)
		while (iter != _reqResourceMap.end())
		{
			bool alreadyAcquired = false;
			//compare key to acquired resources vector
			// if i don't have any resources (besides a bay if i'm an indoor step)
			if (_acquiredResources.size() == 0)
			{
				//if what i need is more than what's avail, wait
				if (iter->second->GetNumberOfResourcesNeeded() > _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					SimExec::ScheduleConditionalEvent(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetPriority(),
						new ResWaitForResEA(this, resource, iter->second, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources),
						"ResWaitForResEA", iter->first, resource->GetResourceName(), 77.7, _myRJ);

					//record waits
					Scribe::UpdateResourceRequests(iter->second->GetResourceName(), false);
					Scribe::RecordResourceWait(resource->GetResourceName(), 0, iter->second->GetResourceName(),
						SimExec::GetSimulationTime()._timeOfDay);
					return;
				}
				//else if what i need is less than or = what's avail
				else if (iter->second->GetNumberOfResourcesNeeded() <= _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					//acquire them
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());

					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
				}
				continue;
			}

			bool foundIt = false;

			map<string, int>::iterator it = _acquiredResources.begin();
			while (it != _acquiredResources.end())
			{
				if (iter->first == it->first) {
					foundIt = true;

					alreadyAcquired = true;

					//i've already acquired this resource - if i need more of it
					if (it->second < iter->second->GetNumberOfResourcesNeeded())
					{
						//i've already acquired this resource - if i need more than are avail, wait
						if ((iter->second->GetNumberOfResourcesNeeded() - it->second) > _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							SimExec::ScheduleConditionalEvent((RepairJob::GetMyResRepairJobObj(_myRJ)->GetPriority()), new ResWaitForResEA(this, iter->second,
								resource, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources), "ResWaitForResEA", resource->GetResourceName(),
								iter->first, 0, _myRJ);

							return;
						}
						//i've already acquired this resource - if i need more of it, & there are enough avail
						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) < _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							//acquire the difference
							AcquireResourceEM(iter->second, (iter->second->GetNumberOfResourcesNeeded() - it->second));

							it->second += (iter->second->GetNumberOfResourcesNeeded() - it->second);
						}
						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) == _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());

						}
					}
				}
				it++;
			}
			//didn't find this resource
			if (foundIt == false)
			{
				if (iter->second->GetNumberOfResourcesNeeded() <= _resourcePool.find(iter->first)->second->GetResourceCount())
				{
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());

					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
				}
				else {
					SimExec::ScheduleConditionalEvent((RepairJob::GetMyResRepairJobObj(_myRJ)->GetPriority()),
						new ResWaitForResEA(this, iter->second, resource, iter->second->GetNumberOfResourcesNeeded(), _acquiredResources),
						"ResWaitForResEA", resource->GetResourceName(), iter->first, 0, _myRJ);
					return;
				}
			}
			iter++;
		}

		if (IsInpectionFail(_inspecFailProb) == true)
		{
			isNextStepReturnStep = true;
			Scribe::RecordInspectionFailure(0,resource->GetResourceName(), RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(_stepID)->GetName(), _stepID);

			SimExec::ScheduleEventAt(_RJpriority, new StartRepairServiceEA(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(_returnStep), 
				resource, _acquiredResources), 0, "StartServiceEA");
			//reschedule step of id = to return step id and all following steps
			return;
		}
		else if (IsInpectionFail(_inspecFailProb) == false)
		{
			SimExec::ScheduleEventAt(1, new DoneResourceServiceEA(this, resource, _acquiredResources), _servTime->GetRV(), "DoneServiceEA");
		}
	}
}

void Step::SetMyRJName(string myRJ)
{
	_myRJ = myRJ;
}

string Step::GetMyRJName()
{
	return _myRJ;
}

string Step::AcquireBay(StepResource* bay, int numNeeded)
{
	double newCount;
	bool acquired = false;
	map<string, StepResource*>::const_iterator iter = _resourcePool.find(bay->GetResourceName());

	if (numNeeded <= iter->second->GetResourceCount())
	{
		//cout << " in acquire bay, there are enough " << endl;
		acquired = true;
		Scribe::UpdateResourceUtilization(bay->GetResourceName(), numNeeded, SimExec::GetSimulationTime()._timeOfDay);
		newCount = iter->second->GetResourceCount() - numNeeded;
		//resource->SetResourceCount(newCount);
		//SetResPoolCount(resource->GetResourceName(),newCount);
		SetResPoolCount(iter->second->GetResourceName(), newCount);
		Scribe::UpdateResourceRequests(bay->GetResourceName(), acquired);
		return bay->GetResourceName();
	}
	else {
		if (iter->first == "S Bay") {
			//Check up to Med
			map<string, StepResource*>::const_iterator miter = _resourcePool.find("M Bay");
			if (miter->second->GetResourceCount() > 0 && (miter->second->GetResourceCount() - ((double)numNeeded / 2.0)) >= 0.0) {
				acquired = true;
				Scribe::UpdateResourceRequests(miter->second->GetResourceName(), acquired);
				newCount = miter->second->GetResourceCount() - ((double)numNeeded / 2.0);
				SetResPoolCount(miter->second->GetResourceName(), newCount);
				return miter->second->GetResourceName();
			}
			else {
				//Check up to Large
				map<string, StepResource*>::const_iterator liter = _resourcePool.find("L Bay");
				if (liter->second->GetResourceCount() > 0) {
					if ((liter->second->GetResourceCount() - ((double)numNeeded / 4.0)) < 0.0) {
						Scribe::UpdateResourceRequests(bay->GetResourceName(), acquired);
						return "";
					}
					else {
						acquired = true;
						Scribe::UpdateResourceRequests(liter->second->GetResourceName(), acquired);
						newCount = liter->second->GetResourceCount() - ((double)numNeeded / 4.0);
						SetResPoolCount(liter->second->GetResourceName(), newCount);
						return liter->second->GetResourceName();
					}
				}
				else {
					Scribe::UpdateResourceRequests(bay->GetResourceName(), acquired);
					return "";
				}
			}
		}
		else if (iter->first == "M Bay") {
			//cout << " in acquire bay, there aren't enough med, get large " << endl;

			//Check up
			map<string, StepResource*>::const_iterator liter = _resourcePool.find("L Bay");
			if (liter->second->GetResourceCount() > 0){
				if ((liter->second->GetResourceCount() - ((double)numNeeded / 2.0)) >= 0.0) {
					acquired = true;
					Scribe::UpdateResourceRequests(liter->second->GetResourceName(), acquired);
					newCount = liter->second->GetResourceCount() - ((double)numNeeded) / 2.0;
					SetResPoolCount(liter->second->GetResourceName(), newCount);
					return liter->second->GetResourceName();
				}
				else {
					Scribe::UpdateResourceRequests(bay->GetResourceName(), acquired);
					return "";
				}
			}
			else {

				Scribe::UpdateResourceRequests(bay->GetResourceName(), acquired);
				return "";
			}
		}
		else if (iter->first == "L Bay") {

			Scribe::UpdateResourceRequests(bay->GetResourceName(), acquired);
			return "";
		}
	}
}

bool isNextJob = false;

void Step::DoneServiceEM(Aircraft* aircraft, map<string, int> acquiredResources)
{
	_acquiredResources = acquiredResources;

	//if this is not the last step in the job)
	if (_stepID < aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize())
	{
		//int nextID = _stepID + 1;
		int nextID;

		//if (isNextStepReturnStep == true)
		if (_type == "Inspection" && isFail == true)
		{
			nextID = this->GetReturnStep();
		}
		else
		{
			nextID = _stepID + 1;
		}

		//check the next step's required resources against my acquired list
		map<string, int>::iterator it = _acquiredResources.begin();
		bool incremented = false; 
		while (it != _acquiredResources.end())
		{
			//if resource name is not found in next step's required vector, release
			if (aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID)->ResourceInReqResource(it->first) == false)
			{
				//release all of this name 
				map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
				iter = _reqResourceMap.find(it->first);
				if (iter != _reqResourceMap.end())
				{
					ReleaseResourceEM(iter->second, it->second);

					//empty appropriate acquired vector index
					auto trash = std::next(it, 1);
					trash--;
					it++;
					_acquiredResources.erase(trash->first);
					incremented = true; 

				}
			}

			//if resource name is found in next step's required vector, keep it in the vector
			else if (aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID)->ResourceInReqResource(it->first) == true)
			{
				map<string, StepResource*>::iterator futureMap = aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID)->GetResourceMapBegin();
				while (futureMap != aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID)->GetResourceMapEnd())
				{
					//if i have more of this resource than i need, release
					if (it->first == futureMap->first)
					{
						if (it->second > futureMap->second->GetNumberOfResourcesNeeded())
						{
							//release the difference
							ReleaseResourceEM(futureMap->second, it->second - futureMap->second->GetNumberOfResourcesNeeded());

							it->second -= (it->second - futureMap->second->GetNumberOfResourcesNeeded());
						}
					}
					futureMap++;
				}
			}
			if (incremented == false)
				it++;
			else
				incremented = false; 
		}
		Scribe::RecordRepairEnd(aircraft->GetAircraftID(), _myRJ, _stepID, SimExec::GetSimulationTime()._timeOfDay);
		SimExec::ScheduleEventAt(GetRJPriority(), new StartServiceEA(aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID), aircraft,
			_acquiredResources), 0.0, "StartServiceEA");

	}
	//else if the current step is the last step
	else if (_stepID == aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize())
	{
		int nextID;

		if (isNextStepReturnStep == true)
		{
			nextID = this->GetReturnStep();
			isNextStepReturnStep = false;
		}

		//if i have no more jobs after this one
		if (aircraft->GetMyRJMapSize() == 1)
		{
			//empty appropriate acquired vector index
			map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
			while (iter != _reqResourceMap.end())
			{
				if (iter->first == "S Bay" || iter->first == "M Bay" || iter->first == "L Bay") {

					map<string, int>::iterator it = _acquiredResources.begin();
					while (it != _acquiredResources.end())
					{
						if (it->first == iter->first) {
							ReleaseBay(iter->second, aircraft->GetBaySizeReq(), it->first, iter->second->GetNumberOfResourcesNeeded());
						}
						it++;
					}
				}
				else {
					map<string, int>::iterator it = _acquiredResources.begin();
					while (it != _acquiredResources.end())
					{
						if (it->first == iter->first) {
							ReleaseResourceEM(iter->second, it->second);
						}
						it++;
					}
				}
				iter++;
			}
			_acquiredResources.clear();

			if (_acquiredResources.size() != 0)

			//depart
			Scribe::RecordRepairEnd(aircraft->GetAircraftID(), _myRJ, _stepID, SimExec::GetSimulationTime()._timeOfDay);
			this->SetNextTask(SimExec::GetSystemSink());
			Depart(aircraft);
		}

		//else if i have more jobs
		else
		{
			//stepid is 1, because its the first step of the new job
			int nextID = 1;

			//if next job requires a bay and i have one already, keep it and clear all else 
			if (aircraft->GetNextRepairJob(_myRJ)->GetIndoorReq() == 'Y' && _indoorReq == 'Y')
			{
				map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
				while (iter != _reqResourceMap.end())
				{
					//////cout << "here 3" << endl;
					if (iter->first != "S Bay" && iter->first != "M Bay" && iter->first != "L Bay")
					{
						map<string, int>::iterator it = _acquiredResources.begin();
						it = _acquiredResources.find(iter->first);
						if (it != _acquiredResources.end())
						{
							ReleaseResourceEM(iter->second, it->second);

							//empty appropriate acquired vector index
							_acquiredResources.erase(it->first);
						}
					}
					iter++;
				}
			}
			//else, next repair job doesn't need a bay, so clear everything.
			else
			{
				int count = 0;
				map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
				while (iter != _reqResourceMap.end())
				{
					if (iter->first == "S Bay" || iter->first == "M Bay" || iter->first == "L Bay") {

						map<string, int>::iterator it = _acquiredResources.begin();
						while (it != _acquiredResources.end())
						{
							if (it->first == iter->first) {
								ReleaseBay(iter->second, aircraft->GetBaySizeReq(), it->first, iter->second->GetNumberOfResourcesNeeded());
							}
							it++;
						}
					}
					else {
						map<string, int>::iterator it = _acquiredResources.begin();
						while (it != _acquiredResources.end())
						{
							if (it->first == iter->first) {

								ReleaseResourceEM(iter->second, it->second);
							}
							it++;
						}
					}
					iter++;
				}
				_acquiredResources.clear();

				if (_acquiredResources.size() > 1)
					cout << "ERROR \n";
			}

			Scribe::RecordRepairEnd(aircraft->GetAircraftID(), _myRJ, _stepID, SimExec::GetSimulationTime()._timeOfDay);
			SimExec::ScheduleEventAt(aircraft->GetNextRepairJob(_myRJ)->GetPriority(), new StartServiceEA(aircraft->GetNextRepairJob(_myRJ)->GetStep(nextID), aircraft, _acquiredResources), 0.0, "StartServiceEA");

			string oldJob = _myRJ;
			SetMyRJName(aircraft->GetNextRepairJob(_myRJ)->GetName());
			aircraft->DeleteJob(oldJob);
		}
	}
}

void Step::DoneResourceServiceEM(StepResource* resource, map<string, int> acquiredResources)
{
	_acquiredResources = acquiredResources;

	//if there are more steps after this
	if (_stepID < RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStepVecSize())
	{
		int nextID;
		if (_type == "Inspection" && isFail == true)
		{
			nextID = this->GetReturnStep();
		}
		else
		{
			nextID = _stepID + 1;
		}

		map<string, int>::iterator it = _acquiredResources.begin();
		bool incremented = false;
		while (it != _acquiredResources.end())
		{
			//if resource name is not found in next step's required vector, release
			if (RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(nextID)->ResourceInReqResource(it->first) == false)
			{
				//release all of this name 
				map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
				iter = _reqResourceMap.find(it->first);
				if (iter != _reqResourceMap.end())
				{
					ReleaseResourceEM(iter->second, it->second);

					//empty appropriate acquired vector index
					auto trash = std::next(it, 1);
					trash--;
					it++;
					_acquiredResources.erase(trash->first);
					incremented = true;
				}
			}
			//else if resource name is found in next step's required vector, keep it in the vector
			else if (RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(nextID)->ResourceInReqResource(it->first) == true)
			{
				//Check next step's resources against this step's resources and release if I have more than i need			
				map<string, StepResource*>::iterator futureMap = RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(nextID)->GetResourceMapBegin();
				while (futureMap != RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(nextID)->GetResourceMapEnd())
				{
					//if i have more of this resource than i need, release
					if (it->first == futureMap->first)
					{
						if (it->second > futureMap->second->GetNumberOfResourcesNeeded())
						{
							//release the difference
							ReleaseResourceEM(futureMap->second, it->second - futureMap->second->GetNumberOfResourcesNeeded());

							it->second -= (it->second - futureMap->second->GetNumberOfResourcesNeeded());
						}
					}
					futureMap++;
				}
			}
			if (incremented == false)
				it++;
			else
				incremented = false;
		}
		Scribe::RecordRepairEnd(0, _myRJ, _stepID, SimExec::GetSimulationTime()._timeOfDay);
		SimExec::ScheduleEventAt(GetRJPriority(), new StartRepairServiceEA(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(nextID), resource,
			_acquiredResources), 0.0, "StartRepairServiceEA");

	}

	//else if this is the last step
	else if (_stepID == RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStepVecSize())
	{
		//empty appropriate acquired vector index
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
		while (iter != _reqResourceMap.end())
		{
			map<string, int>::iterator it = _acquiredResources.begin();
			while (it != _acquiredResources.end())
			{
				if (it->first == iter->first) {
					ReleaseResourceEM(iter->second, it->second);
				}
				it++;
			}
			iter++;
		}
		_acquiredResources.clear();
		Scribe::RecordRepairEnd(0, _myRJ, _stepID, SimExec::GetSimulationTime()._timeOfDay);
		if (_acquiredResources.size() != 0)
			cout << "ERROR \n";

	}

	//if last step, schedule restoreresourceEM
	SimExec::ScheduleEventAt(-1, new RestoreResourceEA(this, resource), this->_servTime->GetRV(), "RestoreResourceEA");
}

void Step::AcquireResourceEM(StepResource* resource, int numNeeded)
{
	double newCount;
	bool acquired = false;

	map<string, StepResource*>::iterator iter = _resourcePool.find(resource->GetResourceName());
	if (numNeeded <= iter->second->GetResourceCount())
	{
		acquired = true;
		Scribe::UpdateResourceUtilization(resource->GetResourceName(), numNeeded, SimExec::GetSimulationTime()._timeOfDay);
		newCount = iter->second->GetResourceCount() - numNeeded;

		SetResPoolCount(iter->second->GetResourceName(), newCount);
	}

	Scribe::UpdateResourceRequests(resource->GetResourceName(), acquired);
}

void Step::ReleaseResourceEM(StepResource* resource, int numRelease)
{
	int newCount;

	map<string, StepResource*>::iterator iter = _resourcePool.find(resource->GetResourceName());
	//map<string, Resource*>::const_iterator numIt = _reqResourceMap.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() + numRelease;

	SetResPoolCount(iter->first, newCount);
	
	int negativeCount = numRelease * (-1);//Used to increment utilization for scribe

	Scribe::UpdateResourceUtilization(resource->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);

	SimExec::CheckConditionalEvents(iter->second, 0);
}


void Step::FailResourceEM(StepResource* resource)
{
	int newCount;

	map<string, StepResource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() - 1;
	SetResPoolCount(iter->first, newCount);

	//schedule first step of the repair job for this resource (at the end of which a restore resource will be scheduled)
	SimExec::ScheduleEventAt(_RJpriority, 
		new StartRepairServiceEA(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetFirstStep(), resource, _acquiredResources), 
		0.0, 
		"StartServiceEA");

	Scribe::RecordFailure(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetSimulationTime()._timeOfDay);
}

void Step::RestoreResourceEM(StepResource* resource)
{
	resource->RestoreResource();
	Scribe::RecordRestore(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetSimulationTime()._timeOfDay);
	SimExec::CheckConditionalEvents(resource, 0);
}


////////////////////////////////////////////
//////////////   BOOLEANS   ////////////////
////////////////////////////////////////////

// Checking if iterator points to the end of the map // 
// Utilized to check if an element exists in the map // 
bool Step::IsResourceMapEnd(map<string, StepResource*>::iterator it)
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
	Uniform prob(0, 1);
	if (inspecFailProb->GetRV() >= prob.GetRV())
	{
		return true;
	}
	else return false;
}

bool Step::IsResourceReleased(map<string, StepResource*>::const_iterator iter, int newCount)
{
	if (iter->second->GetResourceCount() == newCount)
		return true;

	return false;
}

bool Step::IsMyBaySizeAvailable(string baySize)
{
	map<string, StepResource*>::const_iterator it = _resourcePool.find(baySize);
	if (it->second->GetResourceCount() > 0)
		return true;
	else
		return false;
}

bool Step::AreThereBaysAvailable(string baySize)
{
	map<string, StepResource*>::const_iterator it = _resourcePool.find(baySize);
	if (it->first == "S Bay") {
		map<string, StepResource*>::const_iterator mIt = _resourcePool.find("M Bay");
		map<string, StepResource*>::const_iterator lIt = _resourcePool.find("L Bay");
		if (it->second->GetResourceCount() > 0 || mIt->second->GetResourceCount() > 0 || lIt->second->GetResourceCount() > 0)
			return true;
		else
			return false;
	}
	else if (it->first == "M Bay") {
		map<string, StepResource*>::const_iterator lIt = _resourcePool.find("L Bay");
		if (it->second->GetResourceCount() > 0 || lIt->second->GetResourceCount() > 0)
			return true;
		else
			return false;
	}
	else if (it->first == "L Bay") {
		if (it->second->GetResourceCount() > 0)
			return true;
		else
			return false;
	}
	
}

bool Step::WasBayAcquired(string bayName)
{
	if (bayName != "")
		return true;
	else
		return false;
}

bool Step::isFirstShiftChange()
{
	if (_firstShiftUpdateFlag == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
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
	return 0;
}

StepResource* Step::GetResourceObj(string name)
{
	map<string, StepResource*>::iterator it = _reqResourceMap.find(name);
	if (it == _reqResourceMap.end())
		return nullptr;
	return it->second;
}
Parts* Step::GetPartsObj(string name)
{
	map<string, Parts*>::iterator it = _reqPartsMap.find(name);
	if (it == _reqPartsMap.end())
		return nullptr;
	return it->second;
}


map<string, StepResource*>::iterator Step::GetResourceMapBegin()
{
	return _reqResourceMap.begin();
}

map<string, StepResource*>::iterator Step::GetResourceMapEnd()
{
	return _reqResourceMap.end();
}

map<string, StepResource*>::iterator Step::FindResourceinReqResMap(string resource)
{
	return _reqResourceMap.find(resource);
}

void Step::ScheduleFirstRecurringStep(Step* step, Aircraft* aircraft)
{
	map<string, int> test;
	SimExec::ScheduleEventAtRecurring(_RJpriority, new StartServiceEA(step, aircraft, test), 0.0, "StartServiceEA");
}

void Step::ScheduleCalendarStep(Step* step, Aircraft* aircraft, CalendarObj* calobj, int i)
{

	map<string, int> test;

		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _RJpriority, new StartServiceEA(step, aircraft, test), "StartServiceEA");
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

void Step::ScheduleFirstPeacetimeShifts()
{
	//schedule 1st shift change
	SimExec::ScheduleEventAtCalendar(0.0, 0.0, InputReader::GetShiftOneStartTime(), 
		SimExec::GetSimulationTime()._year, -10, new ShiftChangeEA(), "Shift1Change");

	//schedule 2nd shift change
	SimExec::ScheduleEventAtCalendar(0.0, 0.0, InputReader::GetShiftTwoStartTime(), 
		SimExec::GetSimulationTime()._year, -10, new ShiftChangeEA(), "Shift2Change");

	//schedule 3rd shift change
	SimExec::ScheduleEventAtCalendar(0.0, 0.0, InputReader::GetShiftThreeStartTime(),
		SimExec::GetSimulationTime()._year, -10, new ShiftChangeEA(), "Shift3Change");
}

void Step::SetFirstShiftUpdateFlag(int flag)
{
	_firstShiftUpdateFlag = flag;
}

void Step::ScheduleFirstWartimeShifts()
{
	//schedule 1st shift change
	SimExec::ScheduleEventAtCalendar(0.0, 0.0, InputReader::GetShiftOneStartTime(),
		SimExec::GetSimulationTime()._year, -10, new ShiftChangeEA(), "Shift1Change");

	//schedule 2nd shift change
	SimExec::ScheduleEventAtCalendar(0.0, 0.0, InputReader::GetShiftTwoStartTime(),
		SimExec::GetSimulationTime()._year, -10, new ShiftChangeEA(), "Shift2Change");
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

}

void Step::SetReqResource(string reqResource)
{
	//change this to check for while finding &
	istringstream res(reqResource);
	string first;
	string sec;
	string third;

	string line;


	while (getline(res, line, '&'))
	{
		StepResource* newResource = new StepResource();
		istringstream ss(line);
		string resourceName;
		string numString;
		int num;

		getline(ss, resourceName, '(');
		getline(ss, numString, ')');


		istringstream ssNum(numString);
		ssNum >> num;

		AddResource(newResource, resourceName, num);
	}
}

void Step::SetReqParts(string reqParts, int numNeeded)
{
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


void Step::SetReturnStep( int returnStep)
{

	_returnStep = returnStep;
}


////////////////////////////////////////////
/////////////     OTHER      ///////////////
////////////////////////////////////////////

//now exclusive to unplanned
void Step::ScheduleFirstStep(Step* step, Aircraft* aircraft)
{
	map<string, int> test;
	SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, test), 0.0, "StartServiceEA");
}

void Step::ReleaseBay(StepResource* bay, string myOriginalBaySize, string baySizeIHave, int numRelease)
{
	int newCount;
	map<string, StepResource*>::iterator iter = _resourcePool.find(bay->GetResourceName());

	if (myOriginalBaySize == baySizeIHave) {

		newCount = iter->second->GetResourceCount() + numRelease;

		SetResPoolCount(iter->first, newCount);

		IsResourceReleased(iter, newCount);

		int negativeCount = numRelease * (-1);		//Used to increment utilization for scribe
		Scribe::UpdateResourceUtilization(bay->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);

		SimExec::CheckConditionalEvents(iter->second, 0);
	}
	else {
		if (myOriginalBaySize == "S Bay") {
			if (baySizeIHave == "M Bay") {
				map<string, StepResource*>::iterator it = _resourcePool.find("M Bay");
				newCount = it->second->GetResourceCount() + ((double)numRelease / 2.0);
				SetResPoolCount(it->first, newCount);
				IsResourceReleased(it, newCount);

				int negativeCount = numRelease * (-1); //Used to increment utilization for scribe
				Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
			}
			else if (baySizeIHave == "L Bay") {
				map<string, StepResource*>::iterator it = _resourcePool.find("L Bay");
				newCount = it->second->GetResourceCount() + ((double)numRelease / 4.0);
				SetResPoolCount(it->first, newCount);
				IsResourceReleased(it, newCount);

				int negativeCount = numRelease * (-1); //Used to increment utilization for scribe
				Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
			}
		}
		else if (myOriginalBaySize == "M Bay") {
			if (baySizeIHave == "L Bay") {
				map<string, StepResource*>::const_iterator it = _resourcePool.find("L Bay");
				newCount = it->second->GetResourceCount() + ((double)numRelease / 2.0);
				SetResPoolCount(it->first, newCount);
				IsResourceReleased(it, newCount);

				int negativeCount = numRelease * (-1);//Used to increment utilization for scribe
				Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
			}
		}
	}
}


map<string, Parts*> Step::GetPartsPool()
{
	return _partsPool;
}

map<string, StepResource*> Step::GetResPool()
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

void Step::AddToResPool(StepResource* resource, string resourceName)
{
	_resourcePool[resourceName] = resource;
}

void Step::AddToPartsPool(Parts* parts, string partsName)
{
	_partsPool[partsName] = parts;
}

void Step::SetResPoolCount(string resource, double newCount)
{
	_resourcePool.find(resource)->second->SetResourceCount(newCount);
}

void Step::SetPartPoolCount(string part, int newCount)
{
	_partsPool.find(part)->second->SetPartsCount(newCount);
}

map<string, StepResource*>::iterator Step::FindResource(string resource)
{
	return _reqResourceMap.find(resource);
}

map<string, Parts*>::iterator Step::FindParts(string resource)
{
	return _reqPartsMap.find(resource);
}

void Step::Execute(Aircraft* aircraft) {

}

void Step::AddResource(StepResource* resource, string resourceName, int numNeeded)
{
	resource->SetNumResNeeded(numNeeded);
	_reqResourceMap[resourceName] = resource;
}


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
	_reqPartsMap[partsName] = parts;
	_reqPartsMap[partsName]->SetNumPartsNeeded(numNeeded);
}

void Step::AcquireParts(Parts* parts, int numNeeded)
{
	int newCount;


	map<string, Parts*>::iterator iter = _partsPool.find(parts->GetPartsName());
	if (numNeeded <= iter->second->GetPartsCount())
	{
		newCount = iter->second->GetPartsCount() - numNeeded;
		SetPartPoolCount(iter->first, newCount);

		if (newCount <= iter->second->GetThreshold())
		{
			SimExec::ScheduleEventAt(0, new PlaceOrderEA(this, iter->second), 0.0, "PlaceOrderEA");
		}
	}
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
	{
		cout << "None \n";
	}
	else
	{
		cout << "Service time is ";
		_inspecFailProb->PrintDistribution();
		cout << endl;
	}
	cout << "		Service Time Distribution: ";
	if (_servTime == nullptr)
		cout << "None \n";
	else
	{
		cout << "Service time is ";
		_servTime->PrintDistribution();
		cout << endl;
	}
	cout << "		Required Resources: " << _reqRes << endl;
	cout << "		Required Parts: " << _reqParts << endl;
	cout << "		Return Step If Inspection Fails: " << _returnStep << endl;

	cout << "			Parts - " << endl;
	PrintParts();
	cout << endl;

	cout << "			Resource - " << endl;
	map<string, StepResource*>::iterator it = _reqResourceMap.begin();
	for (int i = 0; i < _reqResourceMap.size(); i++)
	{
			cout << "			" << it->first << endl;
		it++;
	}
	PrintResources();
	cout << endl;
}

void Step::PrintParts()
{
	map<string, Parts*>::iterator it = _reqPartsMap.begin();
	while (it != _reqPartsMap.end())
	{
		it->second->PrintPartsProperties();
		it++;
	}
}

void Step::PrintResources()
{
	map<string, StepResource*>::iterator it = _reqResourceMap.begin();

	while (it != _reqResourceMap.end())
	{
		it->second->PrintResProperties();
		it++;
	}
}

void Step::PrintPools()
{
	map<string, StepResource*>::iterator it = _resourcePool.begin();
	while (it != _resourcePool.end())
	{
		it->second->PrintResProperties();
		it++;
	}

	map<string, Parts*>::iterator it2 = _partsPool.begin();

	while (it2 != _partsPool.end())
	{
		it2->second->PrintPartsProperties();
		it2++;
	}
}
