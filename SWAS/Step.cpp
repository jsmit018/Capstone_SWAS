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


	//////cout << ".....IN STEP NEXT INDOOR REQ IS " << _indoorReq << endl;

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

	//not needed to copy because theyre not used til its already been copied:
	//vector<string> _acquiredResources;	//vector of acquired resources to be checked at the end of service
	//PriorityQueue<Aircraft>* _priorityQueue;
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
		//cout << "EXECUTING RESOURCE REPAIR WAIT FOR RESOURCE" << endl;
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
	//WaitForResourceEA(Step* step, Resource* resource, Aircraft* aircraft, int amountNeeded, vector<string> acqResources) {
	WaitForResourceEA(Step* step, StepResource* resource, Aircraft* aircraft, int amountNeeded, map<string, int> acqResources) {
		_step = step;
		_resource = resource;
		_aircraft = aircraft;
		_amountNeeded = amountNeeded;
		_acqResources = acqResources;
		//_outputRecorder = outputRecorder;
		//cout << aircraft->GetAircraftType() << ", ID: " << aircraft->GetAircraftID() << " is moving to CEL" << " For resource: " << resource->GetResourceName() << ", amout needed " << amountNeeded << endl;
		/*if (aircraft->GetAircraftID() == 5 && _resource->GetResourceName() == "M Bay")
			//cout << "Oogly Boogly" << endl;*/
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

		//cout << "EXECUTING WAIT FOR RESOURCE" << endl;

			////cout << _aircraft->GetAircraftID() << " has resource "
			//	<< " in step " << _step->GetName()
			//	<< _acqResources[i].first << " in quantity "
			//	<< _acqResources[i].second << endl;

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
	//vector<string> _acqResources;
	map<string, int> _acqResources;
	//Scribe* _outputRecorder;

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

		//cout << "EXECUTING WAIT FOR PARTS" << endl;
		for (int i = 0; i < _acqResources.size(); i++)
		{
			////cout << _aircraft->GetAircraftID() << " has resource "
			//	<< " job " << _step->GetMyRJName()
			//	<< " in step " << _step->GetName()
			//	<< _acqResources[i].first << " in quantity "
			//	<< _acqResources[i].second << endl;
		}


		//cout << " WAIT FOR PARTS " << endl;
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
	//NeedPartsEA(Step* step, Parts* parts, Aircraft* aircraft, int amountNeeded, vector<string> acqResources) {
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

		//cout << "EXECUTING WAIT FOR PARTS" << endl;
		for (int i = 0; i < _acqResources.size(); i++)
		{
			////cout << _aircraft->GetAircraftID() << " has resource "
			//	<< " job " << _step->GetMyRJName()
			//	<< " in step " << _step->GetName()
			//	<< _acqResources[i].first << " in quantity "
			//	<< _acqResources[i].second << endl;
		}


		//cout << " WAIT FOR PARTS " << endl;
		_aircraft->SetCELflag(1);
		_step->StartServiceEM(_aircraft, _acqResources);
	}

private:
	Parts* _parts;
	Step* _step;
	Aircraft* _aircraft;
	int _amountNeeded;
	//vector<string> _acqResources;
	map<string, int> _acqResources;
};

class Step::NeedBaysEA : public CondEventAction {
public:
	//NeedBaysEA(Step* step, Aircraft* aircraft, vector<string> acqResources, Resource* bay, int numNeeded) {
	//NeedBaysEA(Step* step, Aircraft* aircraft, vector<pair<string, int>> acqResources, Resource* bay, int numNeeded) {
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
	//vector<string> _acqResources;
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
	//StartServiceEA(Step* step, Aircraft* aircraft, vector<string> acqResources) {
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
	//vector<string> _acqResources;
	map<string, int> _acqResources;
};

class Step::ShiftChangeEA : public EventAction {
public:
	ShiftChangeEA() {
	}

	void Execute() 
	{
		//cout << "in shiftchange ea before updating shift" << endl;
		UpdateShift();
	}
};

class Step::StartRepairServiceEA : public EventAction {
public:
	//StartRepairServiceEA(Step* step, Resource* resource, vector<string> acqResources) {
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
	//vector<string> _acqResources;
	map<string, int> _acqResources;
};

class Step::DoneServiceEA : public EventAction {
public:
	//DoneServiceEA(Step* step, Aircraft* aircraft, vector<string> acqResources)
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
	//vector<string> _acqResources;
	map<string, int> _acqResources;
};

class Step::DoneResourceServiceEA : public EventAction {
public:
	//DoneRepairServiceEA(Step* step, Resource* resource, vector<string> acqResources)
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
	//vector<string> _acqResources;
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
	//cout << "flag " << _firstShiftUpdateFlag << endl;

	double newCount;
	double lastShiftsInitCount;
	double thisShiftsInitCount;

	//we don't want to change the counts for the first 6am shift
	if (isFirstShiftChange() == true)
	{
		//cout << " Shifts Starting, month is " << SimExec::ConvertDate(SimExec::GetSimulationTime()._month)
			//<< " day is " << SimExec::GetSimulationTime()._day + 1
			//<< " time is " << SimExec::GetSimulationTime()._timeOfDay << "00" << endl;
		SetFirstShiftUpdateFlag(0);
		////schedule this same shift change tomorrow at the same time
		SimExec::ScheduleEventAt(-10, new ShiftChangeEA(), 1.0, "ShiftChangeEA", 1);
		return;
	}
	////if its a wartime simulation
	if (InputReader::IsWartime() == true)
	{
		//cout << " wartime, month is " << SimExec::ConvertDate(SimExec::GetSimulationTime()._month)
			//<< " day is " << SimExec::GetSimulationTime()._day + 1
			//<<" time is " << SimExec::GetSimulationTime()._timeOfDay << "00" << endl;

		////if i'm in shift one
		if (InputReader::GetShiftOneStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			//cout << "in war shift one " << endl;
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
					//cout << "shift two " << iter->first << " had init count of " << lastShiftsInitCount << endl;

					thisShiftsInitCount = iter->second->GetShiftOneCount();
					//cout << "shift one will have init count of " << thisShiftsInitCount << endl;

					//cout << "difference is " << thisShiftsInitCount - lastShiftsInitCount << endl;
					//cout << "adding to " << iter->second->GetResourceCount() << endl;

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);
					//cout << "new  " << iter->first << " count is " << newCount << endl << endl;
					iter->second->SetResourceCount(newCount);
				}

				iter++;
			}			
		}
		////else i'm in shift two
		else if(InputReader::GetShiftTwoStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			//cout << "in war shift two " << endl;
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
					//cout << "shift one " << iter->first << " had init count of " << lastShiftsInitCount << endl;

					thisShiftsInitCount = iter->second->GetShiftTwoCount();
					//cout << "shift two will have init count of " << thisShiftsInitCount << endl;

					//cout << "difference is " << thisShiftsInitCount - lastShiftsInitCount << endl;
					//cout << "adding to " << iter->second->GetResourceCount() << endl;

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);
					//cout << "new  " << iter->first << " count is " << newCount << endl << endl;
		
					iter->second->SetResourceCount(newCount);
					//cout << "after setting " << endl;
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
		//cout << " peacetime, time is " << SimExec::GetSimulationTime()._timeOfDay << endl;
		////if i'm in shift one
		//if (InputReader::GetShiftOneStartTime() <= SimExec::GetSimulationTime()._timeOfDay < InputReader::GetShiftTwoStartTime())
		if (InputReader::GetShiftOneStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			//cout << "in peace shift one " << endl;
			////for all resources in the resource pool, update the count
			map<string, StepResource*>::iterator iter = _resourcePool.begin();
			while (iter != _resourcePool.end())
			{
				////if there's a difference in the counts
				if (iter->second->GetShiftThreeCount() != iter->second->GetShiftOneCount())
				{
					////new count is =
					////current count + the difference between last shift's intitial count and this shift's initial count
					lastShiftsInitCount = iter->second->GetShiftThreeCount();
					//cout << "shift three  " << iter->first << " had init count of " << lastShiftsInitCount << endl;

					thisShiftsInitCount = iter->second->GetShiftOneCount();
					//cout << "shift one will have init count of " << thisShiftsInitCount << endl;

					//cout << "difference is " << thisShiftsInitCount - lastShiftsInitCount << endl;
					//cout << "adding to " << iter->second->GetResourceCount() << endl;

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);
					//cout << "new  " << iter->first << " count is " << newCount << endl << endl;

					iter->second->SetResourceCount(newCount);
				}

				iter++;
			}
		}

		////else if i'm in shift two
		else if (InputReader::GetShiftTwoStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{

			//cout << "in peace shift two " << endl;
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
					//cout << "shift one  " << iter->first << " had init count of " << lastShiftsInitCount << endl;

					thisShiftsInitCount = iter->second->GetShiftTwoCount();
					//cout << "shift two will have init count of " << thisShiftsInitCount << endl;

					//cout << "difference is " << thisShiftsInitCount - lastShiftsInitCount << endl;
					//cout << "adding to " << iter->second->GetResourceCount() << endl;

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);
					//cout << "new  " << iter->first << " count is " << newCount << endl << endl;

					iter->second->SetResourceCount(newCount);
				}
				iter++;
			}

		}

		////else i'm in shift three
		else if (InputReader::GetShiftThreeStartTime() == SimExec::GetSimulationTime()._timeOfDay)
		{
			//cout << "in shift three " << endl;
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
					//cout << "shift two  " << iter->first << " had init count of " << lastShiftsInitCount << endl;

					thisShiftsInitCount = iter->second->GetShiftThreeCount();
					//cout << "shift three will have init count of " << thisShiftsInitCount << endl;

					//cout << "difference is " << thisShiftsInitCount - lastShiftsInitCount << endl;
					//cout << "adding to " << iter->second->GetResourceCount() << endl;

					newCount = iter->second->GetResourceCount() + (thisShiftsInitCount - lastShiftsInitCount);
				//	cout << "new  " << iter->first << " count is " << newCount << endl << endl;

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
	//cout << "ugh" << endl;
}

void Step::PlaceOrderEM(Parts* parts)
{
	//if (parts->GetLeadTime() == nullptr)
		//	////cout << "poop " << endl;
		//Schedule Order Arrivals EA at now + lead time
		////cout << _myRJ << " " << _stepID << " " << parts->GetPartsName() << endl;
	//////cout << "Parts total has fallen below its identified threshold, placing an order" << endl;
	double restockTime = parts->GetLeadTime()->GetRV();
	Scribe::RecordRestock(parts->GetPartsName(), restockTime);
	SimExec::ScheduleEventAt(0, new OrderArrivalEA(this, parts), restockTime, "OrderArrivalEA");
}

void Step::OrderArrivalEM(Parts* parts)
{
	//Logic: new count = initial count -- same as new count = current count + (initial count - current count)
	int newCount;

	//map<string, Parts*>::const_iterator iter = _reqPartsMap.find(parts->GetPartsName());
	map<string, Parts*>::iterator iter = _partsPool.find(parts->GetPartsName());
	newCount = iter->second->GetInitPartsCount();

	//parts->SetPartsCount(newCount);
	SetPartPoolCount(iter->first, newCount);

	//Scribe::RecordRestock(parts->GetPartsName(), SimExec::GetSimulationTime()._timeOfDay);
	SimExec::CheckConditionalEvents(0, parts);

	if (iter->second->GetPartsCount() <= iter->second->GetThreshold())
		SimExec::ScheduleEventAt(0, new PlaceOrderEA(this, iter->second), 0.0, "PlaceOrderEA");
}

int Step::GetReturnStep()
{
	return _returnStep;
}

//void Step::StartServiceEM(Aircraft* aircraft, vector<string> acquiredResources)
//void Step::StartServiceEM(Aircraft* aircraft, vector<pair<string, int>> acquiredResources)
void Step::StartServiceEM(Aircraft* aircraft, map<string, int> acquiredResources)
{
	//if (isNextStepReturnStep == true)
	//{
	//	//cout << aircraft->GetAircraftID() << " starting return step " << _name << endl;
	//	////cout << aircraft->GetAircraftID() <<" is in return step " << this->GetID() << " "<<  this->GetName()<< endl;
	//	//this->SetStepID(aircraft->GetMyRepairJobObj(_myRJ)->GetMyReturnStep());
	//	////cout << aircraft->GetAircraftID() << " is in return step " << this->GetID() << " " << this->GetName() << endl;
	//}
	//cout << "Aircraft ID: " << aircraft->GetAircraftID() << " , Step ID: " << _stepID << ", RJ: " << _myRJ << endl;
	isNextStepReturnStep = false;

	if (aircraft->IsAfterCEL() == true)
	{
		//cout << " AFTER CEL " << endl;

		map<string, int>::iterator it = _acquiredResources.begin();
		while (it != _acquiredResources.end())
		{
			//cout << aircraft->GetAircraftID() << " in step " << _name << " has resource "
			//			<< it->first << " in quantity "
			//			<< it->second << endl;
			it++;
		}
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
		/*IF TARMAC HAS LIMITED SPOTS*/
		/*
		else {
			if (_acquiredResources[i] == "outspot") {
				hasResource = true;
				break;
			}
		}
		*/
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
				////cout << "we have to wait for an outspot \n";
			// WAITING
			////cout << "Outspot unavailable, Adding Plane Step to Conditional Event List until it is." << endl;
			SimExec::ScheduleConditionalEvent(aircraft->GetAircraftPriority(), new WaitForResourceEA(this, it->second, aircraft, it->second->GetNumberOfResourcesNeeded(), _acquiredResources));
		}
	*/
	}

	if (_type == "process" || _type == "Process")
	{
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
		//cout << endl << endl << endl;
		////cout << " IN START SERVICE _ NEED " << iter->second->GetResourceName() << " OF COUNT " << iter->second->GetNumberOfResourcesNeeded() << endl;
		////cout << " I currently have  " << _acquiredResources.size() << " types of resources " << endl;

		if (_acquiredResources.size() > 0)
		{
			map<string, int>::iterator it = _acquiredResources.begin();
			while (it != _acquiredResources.end())
			{
				//cout << it->first << " " << it->second << endl;
				it++;
			}
		}

		//for all resources listed in required map (if it has any)
		while (iter != _reqResourceMap.end())
		{
			bool alreadyAcquired = false;
			//compare key to acquired resources vector
			// if i don't have any resources (besides a bay if i'm an indoor step)
			if (_acquiredResources.size() == 0 /*(_indoorReq == 'N' && _acquiredResources.size() == 0) || (_indoorReq == 'Y' && _acquiredResources.size() == 1)*/)
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
					//cout << aircraft->GetAircraftID() << " " << _myRJ << " " << _name << " pushing back " << _acquiredResources.size() << endl;
					//acquire them
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
					////cout << "in start service after acquire in 'what i need is less than or = what's avail' " << endl;
					////cout << "these should match " << iter->first << " " << _resourcePool.find(iter->first)->second->GetResourceName() << endl;
					////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
					////cout << aircraft->GetAircraftID() << " " << _myRJ << " " << _name << " after push back " << _acquiredResources.size() << endl;
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
							////cout << "in start service after acquire in 'i need more of it, & there are enough avail' " << endl;
							////cout << "these should match " << iter->first << " " << it->first << endl;
							////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() - it->second << endl;
							it->second += (iter->second->GetNumberOfResourcesNeeded() - it->second);
							////cout << " new size of resource " << it->second << " new size of vector " << _acquiredResources.size() << endl;

							//}
						}
						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) == _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
							////cout << "in start service after acquire in ' theres an equal amount to what i need' " << endl;
							////cout << "these should match " << iter->first << " " << it->first << endl;
							////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
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
					////cout << "in start service after acquire in 'i don't have it yet' " << endl;
					////cout << "these should match " << iter->first << " " << it->first << endl;
					////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
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
					////cout << "decrementing parts" << endl;
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

		//cout << aircraft->GetAircraftID() << " " << _myRJ << " IN START SERVICE " << _stepID << "- ACQUIRED VEC SIZE IS " << _acquiredResources.size() << endl;

		DoneServiceEA* doneEA = new DoneServiceEA(this, aircraft, _acquiredResources);
		SimExec::ScheduleEventAt(1, doneEA, this->_servTime->GetRV(), "DoneServiceEA");
		//what is this? does it mean waiting for a bay is ended? or service is ended? if either, needs to have an if statement for those cases
		Scribe::RecordServiceWaitEnd(aircraft->GetAircraftID(), "Bay", SimExec::GetSimulationTime()._timeOfDay);

	}

	else if (_type == "inspection" || _type == "Inspection")
	{
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
		//for all resources listed in required map
		while (iter != _reqResourceMap.end())
		{
			bool alreadyAcquired = false;

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
					////cout << "in start service after acquire in 'i have nothing yet, ' " << endl;
					////cout << "these should match " << iter->first << " " << it->first << endl;
					////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
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
						////cout << "after release resource 'what i have is greater'" << endl;
						////cout << "these should match " << iter->first << " " << it->first << endl;
						////cout << "num to release " << it->second - iter->second->GetNumberOfResourcesNeeded() << endl;

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
							////cout << "in start service after acquire in 'there are more than enough, acquire the diff' " << endl;
							////cout << "these should match " << iter->first << " " << it->first << endl;
							////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() - it->second << endl;

							//pushback the difference so we know the number acquired
							if (it->first == iter->first)
							{
								it->second += (iter->second->GetNumberOfResourcesNeeded() - it->second);
							}
						}
						else if (iter->second->GetNumberOfResourcesNeeded() - it->second == _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
							////cout << "in start service after acquire in 'what i need is less than or = what's avail' " << endl;
							////cout << "these should match " << iter->first << " " << it->first << endl;
							////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
						}
					}
				}
				it++;
			}
			iter++;
		}
		
		//cout << aircraft->GetAircraftID() << " " << _myRJ << " IN START SERVICE " << _stepID << "- ACQUIRED VEC SIZE IS " << _acquiredResources.size() << endl;

		/*bool */isFail = IsInpectionFail(_inspecFailProb);

		//if (IsInpectionFail(_inspecFailProb) == true)
		if (isFail == true)
		{
			////cout << " heeere " << endl;
			isNextStepReturnStep = true;
			////////////////////
			SimExec::ScheduleEventAt(1, new DoneServiceEA(this, aircraft, _acquiredResources), _servTime->GetRV(), "DoneServiceEA");

			////cout << " heeere 2" << endl;

			///////////
			Scribe::RecordInspectionFailure(aircraft->GetAircraftID(), aircraft->GetAircraftType(), _myRJ, _stepID);
			Scribe::RecordRework(aircraft->GetAircraftType(), _myRJ, SimExec::GetSimulationTime()._timeOfDay);
			//cout << aircraft->GetAircraftID() <<" INSPECTION FAILED, RETURN STEP IS " << _returnStep << endl;
			////cout << aircraft->GetMyRepairJobObj(_myRJ)->GetStep(_returnStep)->GetName();
			//SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(aircraft->GetMyRepairJobObj(_myRJ)->GetStep(_returnStep), aircraft, _acquiredResources), 0, "StartServiceEA");
			//reschedule step of id = to return step id and all following steps
			//isFail = false;

			return;
		}
		//else if (IsInpectionFail(_inspecFailProb) == false)
		else if (isFail == false)
		{
			//cout << "Aircraft maintenance passed inspection, scheduling DoneService." << endl;
			//Scribe::RecordRepairEnd(aircraft->GetAircraftID(), _myRJ, _stepID, SimExec::GetSimulationTime()._timeOfDay);
			SimExec::ScheduleEventAt(1, new DoneServiceEA(this, aircraft, _acquiredResources), _servTime->GetRV(), "DoneServiceEA");
		}
	}
}

void Step::StartRepairServiceEM(StepResource* resource, map<string, int> acquiredResources)
{ 
	if (isNextStepReturnStep == true)
	{
		//cout << " is return step " << _name << " for " << resource->GetResourceName() << " return step is " << RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetMyReturnStep();
		SetStepID(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetMyReturnStep());
	}
	else
	{
		Scribe::RecordFailure(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetTotalSimulationTime());
	}


	if (resource->IsAfterCEL() == true)
	{
		//cout << " AFTER CEL " << endl;

		map<string, int>::iterator it = _acquiredResources.begin();
		while (it != _acquiredResources.end())
		{
			//cout << resource->GetResourceName() << " in step " << _name << " has resource "
			/*	<< it->first << " in quantity "
				<< it->second << endl;*/
			it++;
		}
		resource->SetCELflag(0);
	}

	_acquiredResources = acquiredResources;
	//cout << "Step " << _type << " " << _name << " " << _stepID << " of " << resource->GetFailureName() << " started on "
		//<< resource->GetResourceName();

	bool hasResource = false;

	if (_type == "process" || _type == "Process")
	{
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
		if (_acquiredResources.size() > 0)
		{
			map<string, int>::iterator it = _acquiredResources.begin();
			while (it != _acquiredResources.end())
			{
				//cout << it->first << " " << it->second << endl;
				it++;
			}
		}

		//for all resources listed in required map (if it has any)
		while (iter != _reqResourceMap.end())
		{
			bool alreadyAcquired = false;
			//compare key to acquired resources vector
			// if i don't have any resources (besides a bay if i'm an indoor step)
			if (_acquiredResources.size() == 0 /*(_indoorReq == 'N' && _acquiredResources.size() == 0) || (_indoorReq == 'Y' && _acquiredResources.size() == 1)*/)
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
					////cout << resource->GetResourceID() << " " << _myRJ << " " << _name << " pushing back " << _acquiredResources.size() << endl;
					//acquire them
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
					////cout << "in start service after acquire in 'what i need is less than or = what's avail' " << endl;
					////cout << "these should match " << iter->first << " " << _resourcePool.find(iter->first)->second->GetResourceName() << endl;
					////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
					////cout << resource->GetResourceID() << " " << _myRJ << " " << _name << " after push back " << _acquiredResources.size() << endl;
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
							////cout << "in start service after acquire in 'i need more of it, & there are enough avail' " << endl;
							////cout << "these should match " << iter->first << " " << it->first << endl;
							////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() - it->second << endl;
							it->second += (iter->second->GetNumberOfResourcesNeeded() - it->second);
							////cout << " new size of resource " << it->second << " new size of vector " << _acquiredResources.size() << endl;
						}
						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) == _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
							////cout << "in start service after acquire in ' theres an equal amount to what i need' " << endl;
							////cout << "these should match " << iter->first << " " << it->first << endl;
							////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
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
					//cout << "in start service after acquire in 'i don't have it yet' " << endl;
					////cout << "these should match " << iter->first << " " << it->first << endl;
					//cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
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

				//if (it->second == NULL) {
				if (it->second->AreEnoughParts() == true)
				{
					////cout << "decrementing parts" << endl;
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

		//cout << " IN START SERVICE - ACQUIRED VEC SIZE IS " << _acquiredResources.size() << endl;

		DoneResourceServiceEA* doneEA = new DoneResourceServiceEA(this, resource, _acquiredResources);
		SimExec::ScheduleEventAt(RepairJob::GetMyResRepairJobObj(_myRJ)->GetPriority(), doneEA, this->_servTime->GetRV(), "DoneServiceEA");
		//what is this? does it mean waiting for a bay is ended? or service is ended? if either, needs to have an if statement for those cases

	}

	else if (_type == "inspection" || _type == "Inspection")
	{
		map<string, StepResource*>::iterator iter = _reqResourceMap.begin();
		if (_acquiredResources.size() > 0)
		{
			map<string, int>::iterator it = _acquiredResources.begin();
			while (it != _acquiredResources.end())
			{
				//cout << it->first << " " << it->second << endl;
				it++;
			}
		}

		//for all resources listed in required map (if it has any)
		while (iter != _reqResourceMap.end())
		{
			bool alreadyAcquired = false;
			//compare key to acquired resources vector
			// if i don't have any resources (besides a bay if i'm an indoor step)
			if (_acquiredResources.size() == 0 /*(_indoorReq == 'N' && _acquiredResources.size() == 0) || (_indoorReq == 'Y' && _acquiredResources.size() == 1)*/)
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
					//cout << 0 << " " << _myRJ << " " << _name << " pushing back " << _acquiredResources.size() << endl;
					//acquire them
					AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
					////cout << "in start service after acquire in 'what i need is less than or = what's avail' " << endl;
					////cout << "these should match " << iter->first << " " << _resourcePool.find(iter->first)->second->GetResourceName() << endl;
					////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
					_acquiredResources.insert(make_pair(iter->first, iter->second->GetNumberOfResourcesNeeded()));
					////cout << resource->GetResourceID() << " " << _myRJ << " " << _name << " after push back " << _acquiredResources.size() << endl;
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
							////cout << "in start service after acquire in 'i need more of it, & there are enough avail' " << endl;
							////cout << "these should match " << iter->first << " " << it->first << endl;
							////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() - it->second << endl;
							it->second += (iter->second->GetNumberOfResourcesNeeded() - it->second);
							////cout << " new size of resource " << it->second << " new size of vector " << _acquiredResources.size() << endl;
						}
						else if ((iter->second->GetNumberOfResourcesNeeded() - it->second) == _resourcePool.find(iter->first)->second->GetResourceCount())
						{
							AcquireResourceEM(iter->second, iter->second->GetNumberOfResourcesNeeded());
							////cout << "in start service after acquire in ' theres an equal amount to what i need' " << endl;
							////cout << "these should match " << iter->first << " " << it->first << endl;
							////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
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
					////cout << "in start service after acquire in 'i don't have it yet' " << endl;
					////cout << "these should match " << iter->first << " " << it->first << endl;
					////cout << "num to acquire " << iter->second->GetNumberOfResourcesNeeded() << endl;
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
			//NEED TO SCHEDULE DONE SERVICE AND USE FLAG TO KNOW IF ITS FAILED INSPECTION
			SimExec::ScheduleEventAt(_RJpriority, new StartRepairServiceEA(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(_returnStep), 
				resource, _acquiredResources), 0, "StartServiceEA");
			//reschedule step of id = to return step id and all following steps
			return;
		}
		else if (IsInpectionFail(_inspecFailProb) == false)
		{
			////cout << "Aircraft maintenance passed inspection, scheduling DoneService." << endl;
			//Scribe::RecordRepairEnd(0, _myRJ, _stepID, SimExec::GetSimulationTime()._timeOfDay);
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
		//cout << "Current Counter, " << iter->second->GetResourceCount() << ", New Count" << newCount << ", Bay Size: " << iter->first << endl;
	//	system("PAUSE");
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


//void Step::DoneServiceEM(Aircraft* aircraft, vector<string> acquiredResources)
//void Step::DoneServiceEM(Aircraft* aircraft, vector<pair<string, int>> acquiredResources)
void Step::DoneServiceEM(Aircraft* aircraft, map<string, int> acquiredResources)
{
	_acquiredResources = acquiredResources;

	/*if (aircraft->GetAircraftID() == 8 && _stepID == 5 && _indoorReq == 'N' && _myRJ == "Battery Charger/Converter Unit - Replace")
		cout << "Bleh" << endl;*/
	//if this is not the last step in the job)
	if (_stepID < aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize())
	{
		//int nextID = _stepID + 1;
		int nextID;

		//if (isNextStepReturnStep == true)
		if (_type == "Inspection" && isFail == true)
		{
			//cout << " YO THIS STEP IS " << this->GetName() << endl;
			//cout << aircraft->GetAircraftID() << " IN DONE OF RETURN, job is "<< _myRJ << "next step is " << this->GetReturnStep() << endl;
			nextID = this->GetReturnStep();
		}
		else
		{
			////cout << " NOT IN DONE OF RETURN" << endl;
			nextID = _stepID + 1;

		}

		//check the next step's required resources against my acquired list
		//for (int i = 0; i < _acquiredResources.size(); i++)
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
					////cout << "in done service, after release resource 'i won't need this in next step'" << endl;
					////cout << "these should match " << iter->first << " " << it->first << endl;
					////cout << "num to release " << it->second << endl;

					//empty appropriate acquired vector index
					auto trash = std::next(it, 1);
					trash--;
					it++;
					_acquiredResources.erase(trash->first);
					incremented = true; 
					////cout << " after release in 'not last step in job' " << endl;
				}
			}

			//if resource name is found in next step's required vector, keep it in the vector
			else if (aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID)->ResourceInReqResource(it->first) == true)
			{
				//cout << aircraft->GetAircraftID() << " " << aircraft->GetAircraftType() << " Retaining " << it->first << " for next step " << aircraft->GetMyRepairJobObj(_myRJ)->GetStep(nextID)->GetName() << endl;

				//Check next step's resources against this step's resources and release if I have more than i need			
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
							////cout << "in done service after release in 'release the difference' " << endl;
							////cout << "these should match " << futureMap->first << " " << it->first << endl;
							////cout << "num to release " << it->second - futureMap->second->GetNumberOfResourcesNeeded() << endl;

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

		//cout << aircraft->GetAircraftID() << "In Done service after releasing (there are more steps)" << endl;
	}
	//else if the current step is the last step
	else if (_stepID == aircraft->GetMyRepairJobObj(_myRJ)->GetStepVecSize())
	{
		int nextID;

		if (isNextStepReturnStep == true)
		{
			//cout << aircraft->GetAircraftID() << " IN DONE OF RETURN, " << " RJ is " << _myRJ << " next step is " << this->GetReturnStep() << endl;
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
					//for (int i = 0; i < _acquiredResources.size(); ++i) {

					map<string, int>::iterator it = _acquiredResources.begin();
					while (it != _acquiredResources.end())
					{
						if (it->first == iter->first) {
							ReleaseBay(iter->second, aircraft->GetBaySizeReq(), it->first, it->second);
							//cout << " in done service after release in 'no more jobs after this step - bay' " << endl;
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
							////cout << " in done service after release in 'no more jobs after this step - resource' " << endl;
							////cout << "these should match: " << iter->first << " " << it->first << endl;
							////cout << "num to release " << it->second << endl;
						}
						it++;
					}
				}
				iter++;
			}
			_acquiredResources.clear();

			if (_acquiredResources.size() != 0)
				//cout << "ERROR \n";

			//depart
			Scribe::RecordRepairEnd(aircraft->GetAircraftID(), _myRJ, _stepID, SimExec::GetSimulationTime()._timeOfDay);
			this->SetNextTask(SimExec::GetSystemSink());
			//cout << "________________THIS AIRCRAFT IS DEPARTING: " << aircraft->GetAircraftID() << " " << aircraft->GetAircraftType() << endl;
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
							////cout << " in done service after release in 'i have more jobs, next job needs nothing but bay' " << endl;
							////cout << "these should match: " << iter->first << " " << it->first << endl;
							////cout << "num to release " << it->second << endl;

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
								ReleaseBay(iter->second, aircraft->GetBaySizeReq(), it->first, it->second);
								////cout << "in done service after release in 'next job doesn't need bay - bay' " << endl;
								////cout << "these should match: " << iter->first << " " << it->first << endl;
								////cout << "num to release " << iter->second->GetNumberOfResourcesNeeded() << endl;

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
								////cout << "in done service after release in 'next job doesn't need bay - resource' " << endl;
								////cout << "these should match " << iter->first << " " << it->first << endl;
								////cout << "num to release " << it->second << endl;

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
			//cout << "  new job " << _myRJ << endl;
			aircraft->DeleteJob(oldJob);
			////cout << aircraft->GetAircraftID() << "---------------------- NUMBER AFTER " << aircraft->GetMyRJMapSize() << endl;
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
			////cout << " YO THIS STEP IS " << this->GetName() << endl;
			////cout << resource->GetResourceName() << " IN DONE OF RESOURCE FAILED INSPEC, job is " << _myRJ << "next step is " << this->GetReturnStep() << endl;
			nextID = this->GetReturnStep();
		}
		else
		{
			////cout << " NOT IN DONE OF RETURN" << endl;
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
					////cout << "in done service, after release resource 'i won't need this in next step'" << endl;
					////cout << "these should match " << iter->first << " " << it->first << endl;
					////cout << "num to release " << it->second << endl;

					//empty appropriate acquired vector index
					auto trash = std::next(it, 1);
					trash--;
					it++;
					_acquiredResources.erase(trash->first);
					incremented = true;
					////cout << " after release in 'not last step in job' " << endl;
				}
			}
			//else if resource name is found in next step's required vector, keep it in the vector
			else if (RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetStep(nextID)->ResourceInReqResource(it->first) == true)
			{
				//cout << 0 << " " << resource->GetResourceName() << " Retaining " << it->first << " for " << this->GetName() << endl;

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
							////cout << "in done resource service after release in 'release the difference' " << endl;
							////cout << "these should match " << futureMap->first << " " << it->first << endl;
							////cout << "num to release " << it->second - futureMap->second->GetNumberOfResourcesNeeded() << endl;

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

		//cout << "In Done resource repair service after releasing (there are more steps)" << endl;
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
					////cout << " in done resource service after release in 'no more jobs after this step - resource' " << endl;
					////cout << "these should match: " << iter->first << " " << it->first << endl;
					////cout << "num to release " << it->second << endl;
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



void Step::AddQueueEM(Aircraft* aircraft)
{
	//_priorityQueue->AddEntity(aircraft, aircraft->GetAircraftPriority());
	//_numInQueue++;
	//InitialArrivalBayCheck();
}

void Step::AcquireResourceEM(StepResource* resource, int numNeeded)
{
	double newCount;
	bool acquired = false;

	/*if (resource->GetResourceName() == "M Bay" || resource->GetResourceName() == "L Bay")
		cout << "bleh" << endl;*/

	map<string, StepResource*>::iterator iter = _resourcePool.find(resource->GetResourceName());
	//map<string, Resource*>::const_iterator numIt = _reqResourceMap.find(resource->GetResourceName());
	if (numNeeded <= iter->second->GetResourceCount())
	{
		acquired = true;
		Scribe::UpdateResourceUtilization(resource->GetResourceName(), numNeeded, SimExec::GetSimulationTime()._timeOfDay);
		newCount = iter->second->GetResourceCount() - numNeeded;
		//cout << "--------------\n" <<
		//	"Resource: " << iter->first << ", Initial Count: " << resource->GetResourceCount() << ", current count: " << iter->second->GetResourceCount() <<
		//	", new count: " << newCount; 
		//resource->SetResourceCount(newCount);
		//resource->SetResourceCount(newCount);
		//SetResPoolCount(resource->GetResourceName(),newCount);
		SetResPoolCount(iter->second->GetResourceName(), newCount);
		//cout << ", after updating through ResPoolFunc: ";
		//cout << iter->second->GetResourceCount() << "\n----------" << endl;
		//system("PAUSE");
	}

	//iter->second->SetResourceCount(newCount);
	//numIt->second->SetResourceCount(newCount);

	Scribe::UpdateResourceRequests(resource->GetResourceName(), acquired);
}

void Step::ReleaseResourceEM(StepResource* resource, int numRelease)
{
	int newCount;

	//if (resource->GetResourceName() == "M Bay" || resource->GetResourceName() == "L Bay")
	//	//cout << "bleh" << endl;
	map<string, StepResource*>::iterator iter = _resourcePool.find(resource->GetResourceName());
	//map<string, Resource*>::const_iterator numIt = _reqResourceMap.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() + numRelease;


	/*cout << "+++++++\n" <<
		_myRJ <<
		" Resource: " << iter->first <<
		", Count before release: " << iter->second->GetResourceCount() <<
		", count expected after release: " << newCount << "\n+++++++" << endl;*/
	//system("PAUSE");


	//resource->SetResourceCount(newCount);
	SetResPoolCount(iter->first, newCount);
	
	//cout << " pool count after release is: ";
	//cout << iter->second->GetResourceCount() << " "; 

	////cout << IsResourceReleased(iter, newCount) << "\n+++++++" << endl;


	int negativeCount = numRelease * (-1);//Used to increment utilization for scribe

	Scribe::UpdateResourceUtilization(resource->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
	/////////******For Andrea is this where we want to put this? I feel it may be best!
	//SimExec::CheckConditionalEvents(resource, 0);
	SimExec::CheckConditionalEvents(iter->second, 0);
}


void Step::FailResourceEM(StepResource* resource)
{
	//cout << "resource " << resource->GetResourceName() << " just failed" << endl;
	int newCount;
	//Mark added this i'm not sure we need to create a new instance, but i'm just going to put a priority of 1 - Jordan
	//RepairJob* newJob;

	map<string, StepResource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());

	newCount = iter->second->GetResourceCount() - 1;
	SetResPoolCount(iter->first, newCount);
	//resource->SetResourceCount(newCount);

	//schedule first step of the repair job for this resource (at the end of which a restore resource will be scheduled)
	SimExec::ScheduleEventAt(_RJpriority, 
		new StartRepairServiceEA(RepairJob::GetMyResRepairJobObj(resource->GetResourceName())->GetFirstStep(), resource, _acquiredResources), 
		0.0, 
		"StartServiceEA");

	//schedule the next random failure of this resource -- happening in swas
	//SimExec::ScheduleEventAt(1, new FailResourceEA(this, resource), iter->second->GetFailureDistr()->GetRV(), "Resource Failure");

	Scribe::RecordFailure(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetSimulationTime()._timeOfDay);
}

void Step::RestoreResourceEM(StepResource* resource)
{
	//cout << "Resource has been restored, updating amount and checking conditional events" << endl;
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
	////cout << " AHHH " << inspecFailProb->GetRV() << " " << prob.GetRV() << endl;
	if (inspecFailProb->GetRV() >= prob.GetRV())
	{
		//cout << "failure" << endl;
		return true;
	}
	else return false;
	//return true;
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
	/*map<string, Resource*>::const_iterator sIt = _resourcePool.find("S Bay");
	map<string, Resource*>::const_iterator mIt = _resourcePool.find("M Bay");
	map<string, Resource*>::const_iterator lIt = _resourcePool.find("L Bay");*/
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
	//cout << "is in first shift change" << endl;
	if (_firstShiftUpdateFlag == 1)
	{
		//cout << "is first flag" << endl;
		return true;
	}
	else
	{
		//cout << "is not first flag" << endl;
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
	//return _numberInQueue;
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

//map<string, StepResource*> Step::GetResourcePool()
//{
//	return _resourcePool;
//}

map<string, StepResource*>::iterator Step::GetResourcePoolBegin()
{
	return _resourcePool.begin();
}

map<string, StepResource*>::iterator Step::GetResourcePoolEnd()
{
	return _resourcePool.end();
}

void Step::ScheduleFirstRecurringStep(Step* step, Aircraft* aircraft)
{
	// Testing //
	//vector<string> test;
	map<string, int> test;
	SimExec::ScheduleEventAtRecurring(_RJpriority, new StartServiceEA(step, aircraft, test), 0.0, "StartServiceEA");
	//SimExec::ScheduleEventAtRecurring(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "StartServiceEA");
	////cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
}

void Step::ScheduleCalendarStep(Step* step, Aircraft* aircraft, CalendarObj* calobj, int i)
{
	//vector<string> test;
	map<string, int> test;
	//for (int i = 0; i < 1; ++i) {
	//for (int i = 0; i < calobj->GetNumEvents()/2; ++i) {
		//Test//

		//________
		SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _RJpriority, new StartServiceEA(step, aircraft, test), "StartServiceEA");
		//SimExec::ScheduleEventAtCalendar(calobj->_months[i], calobj->_days[i], calobj->_timeOfDays[i], calobj->_year[i], _RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), "StartServiceEA");
//	}
	////cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
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
	//cout << "scheduling first peacetime shifts " << endl;

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
	//cout << "in sched first wartime - scheduling first wartime shifts " << endl;
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
	//	////cout << "JOB IS " << this->GetMyRJName() << endl;
	//	////cout << "STEP IS " << this->GetName() << endl;
	//		////cout << "step id " << stepID << " " << _stepID << endl;
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
	//////cout << "&&&&&    *** JOB PRIORITY" << _RJpriority << endl;

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

	//////cout << "whole thing " << failureProb << endl;
	//////cout << "first: " << firstHalf << endl;
	//////cout << "second: " << secHalf << endl;

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
	//	////cout << "first: " << firstHalf << endl;
	//	////cout << "sec: " << secHalf << endl;

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
		//	////cout << "LINE: " << line << endl;
		StepResource* newResource = new StepResource();
		istringstream ss(line);
		string resourceName;
		string numString;
		int num;

		getline(ss, resourceName, '(');
		getline(ss, numString, ')');

		//	////cout << "	R: " << resource << "	N: " << numString << endl;

		istringstream ssNum(numString);
		ssNum >> num;

		//	////cout << "	R: " << resourceName << "	N: " << num << endl;

		//newResource->SetNumResNeeded(num);
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
	//vector<string> test;
	map<string, int> test;
	SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, test), 0.0, "StartServiceEA");
	//SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "StartServiceEA");
	////cout << "(ID: " << aircraft->GetAircraftID() << ") " << aircraft->GetAircraftType() << "'s " << _stepID << "st Step of " << _myRJ << " has been scheduled " << endl;
	////	SimExec::ScheduleEventAt(_RJpriority, new StartServiceEA(step, aircraft, _acquiredResources), 0.0, "AddToQueueEA");
}

void Step::ReleaseBay(StepResource* bay, string myOriginalBaySize, string baySizeIHave, int numRelease)
{
	int newCount;
	map<string, StepResource*>::iterator iter = _resourcePool.find(bay->GetResourceName());

	if (myOriginalBaySize == baySizeIHave) {
		newCount = iter->second->GetResourceCount() + numRelease;
		//cout << "Current Counter, " << iter->second->GetResourceCount() << ", New Count" << newCount << ", Bay Size: " << iter->first << endl;
		//system("PAUSE");
		//resource->SetResourceCount(newCount);
		SetResPoolCount(iter->first, newCount);
		//iter->second->SetResourceCount(newCount);
		//numIt->second->SetResourceCount(newCount);
		IsResourceReleased(iter, newCount);

		int negativeCount = numRelease * (-1);//Used to increment utilization for scribe
		Scribe::UpdateResourceUtilization(bay->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);

		/////////******For Andrea is this where we want to put this? I feel it may be best!
		//SimExec::CheckConditionalEvents(bay, 0);
		SimExec::CheckConditionalEvents(iter->second, 0);
	}
	else {
		if (myOriginalBaySize == "S Bay") {
			if (baySizeIHave == "M Bay") {
				map<string, StepResource*>::iterator it = _resourcePool.find("M Bay");
				newCount = it->second->GetResourceCount() + ((double)numRelease / 2.0);
				SetResPoolCount(it->first, newCount);
				//numIt->second->SetResourceCount(newCount);
				IsResourceReleased(it, newCount);

				int negativeCount = numRelease * (-1);//Used to increment utilization for scribe
				Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
			}
			else if (baySizeIHave == "L Bay") {
				map<string, StepResource*>::iterator it = _resourcePool.find("L Bay");
				newCount = it->second->GetResourceCount() + ((double)numRelease / 4.0);
				SetResPoolCount(it->first, newCount);
				//numIt->second->SetResourceCount(newCount);
				IsResourceReleased(it, newCount);

				int negativeCount = numRelease * (-1);//Used to increment utilization for scribe
				Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
			}
		}
		else if (myOriginalBaySize == "M Bay") {
			//if (baySizeIHave == "S Bay") {
			//	map<string, Resource*>::const_iterator it = _resourcePool.find("S Bay");
			//	newCount = it->second->GetResourceCount() + (numRelease * 2);
			//	SetResPoolCount(it->first, newCount);
			//	//numIt->second->SetResourceCount(newCount);
			//	IsResourceReleased(it, newCount);

			//	int negativeCount = numRelease * (-1);//Used to increment utilization for scribe
			//	Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
			//}
			if (baySizeIHave == "L Bay") {
				map<string, StepResource*>::const_iterator it = _resourcePool.find("L Bay");
				newCount = it->second->GetResourceCount() + ((double)numRelease / 2.0);
				SetResPoolCount(it->first, newCount);
				//numIt->second->SetResourceCount(newCount);
				IsResourceReleased(it, newCount);

				int negativeCount = numRelease * (-1);//Used to increment utilization for scribe
				Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
			}
		}
		//else if (myOriginalBaySize == "L Bay") {
		//	if (baySizeIHave == "S Bay") {
		//		map<string, Resource*>::const_iterator it = _resourcePool.find("S Bay");
		//		newCount = it->second->GetResourceCount() + (numRelease * 4);
		//		SetResPoolCount(it->first, newCount);
		//		//numIt->second->SetResourceCount(newCount);
		//		IsResourceReleased(it, newCount);

		//		int negativeCount = numRelease * (-1);//Used to increment utilization for scribe
		//		Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
		//	}
		//	else if (baySizeIHave == "M Bay") {
		//		map<string, Resource*>::const_iterator it = _resourcePool.find("M Bay");
		//		newCount = it->second->GetResourceCount() + (numRelease * 2);
		//		SetResPoolCount(it->first, newCount);
		//		//numIt->second->SetResourceCount(newCount);
		//		IsResourceReleased(it, newCount);

		//		int negativeCount = numRelease * (-1);//Used to increment utilization for scribe
		//		Scribe::UpdateResourceUtilization(it->second->GetResourceName(), negativeCount, SimExec::GetSimulationTime()._timeOfDay);
		//	}
		//}
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
	//////cout << "adding " << partsName << endl; 
//	parts->PrintPartsProperties();
	//////cout << "wow \n";
	_partsPool[partsName] = parts;
}

//void Step::SetResPoolCount(string resource, int newCount)
void Step::SetResPoolCount(string resource, double newCount)
{
	_resourcePool.find(resource)->second->SetResourceCount(newCount);
}

void Step::SetPartPoolCount(string part, int newCount)
{
	_partsPool.find(part)->second->SetPartsCount(newCount);
	//////cout << part << "'s new count is " << newCount << endl;
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
	////****Discontinued Logic
	//SimExec::ScheduleEventAt(aircraft->GetAircraftPriority(), new AddQueueEA(this, aircraft), 0.0, "AddQueueEA");
}

void Step::AddResource(StepResource* resource, string resourceName, int numNeeded)
{
	//_reqResourceMap.insert(pair<string, Resource*>(resourceName, resource));
	//////cout << " ADDING RESOURCE " << resourceName << " # " << numNeeded << endl;
	resource->SetNumResNeeded(numNeeded);
	_reqResourceMap[resourceName] = resource;


}


//void SchedResourceFailure()
//{
//	//schedule resource failure logic
//	map<string, Resource*>::const_iterator iter = InputReader::GetMasterResMapBegin();
//	while (iter != InputReader::GetMasterResMapEnd())
//	{
//		////cout << "" << endl;
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
	//////cout << " IN ADD PARTS " << partsName << endl;
	//////cout << " PARTS POINTER " << parts->GetPartsName() << endl;
	_reqPartsMap[partsName] = parts;
	_reqPartsMap[partsName]->SetNumPartsNeeded(numNeeded);
	//////cout << "INSERTED PARTS" << _reqPartsMap[partsName]->GetPartsName() << endl;

}

void Step::AcquireParts(Parts* parts, int numNeeded)
{
	int newCount;

	//////cout << " ______ PART " << parts->GetPartsName()<<" THRESHOLD " << parts->GetThreshold() << endl;

	map<string, Parts*>::iterator iter = _partsPool.find(parts->GetPartsName());
	//map<string, Resource*>::const_iterator numIt = _reqResourceMap.find(resource->GetResourceName());
	if (numNeeded <= iter->second->GetPartsCount())
	{
		//	////cout << "now acquiring " << iter->first << endl;
		//	////cout << "current count is " << iter->second->GetPartsCount() << endl;
		//	////cout << "num needed " << numNeeded << endl;
		newCount = iter->second->GetPartsCount() - numNeeded;
		SetPartPoolCount(iter->first, newCount);
		//	////cout << " after set parts pool count " << endl;

		if (newCount <= iter->second->GetThreshold())
		{
			//		////cout << "Parts Count reached threshold, ordering more " << iter->first << endl;
			SimExec::ScheduleEventAt(0, new PlaceOrderEA(this, iter->second), 0.0, "PlaceOrderEA");
		}
	}
	/*else if(numNeeded <= iter->second->GetThreshold())
		SimExec::ScheduleEventAt(0, new PlaceOrderEA(this, iter->second), 0.0, "PlaceOrderEA");*/

		//if (iter->second->GetPartsCount() <= iter->second->GetThreshold())
		//{
		//	////cout << "Parts Count reached threshold, ordering more " << iter->first << endl;
		//	SimExec::ScheduleEventAt(1, new PlaceOrderEA(this, iter->second), 0.0, "PlaceOrderEA");
		//}
		//new count of resource pool is = to current count - num needed from req resource map
}

////////////////////////////////////////////
//////////////   PRINTERS   ////////////////
////////////////////////////////////////////

void Step::Print()
{
	////cout << "		Step Name: " << _name << endl;
	////cout << "		Step ID: " << _stepID << endl;
	////cout << "		Step Type: " << _type << endl;
	////cout << "		Indoor Req: " << _indoorReq << endl;
	////cout << "		RJ Priority: " << _RJpriority << endl;
	////cout << "		Inspection Failure Probability: ";
	if (_inspecFailProb == nullptr) {}
	////cout << "None \n";
	else
	{
		////cout << "Service time is ";
		_inspecFailProb->PrintDistribution();
		////cout << endl;
	}
	////cout << "		Service Time Distribution: ";
	if (_servTime == nullptr)
		////cout << "None \n";
	//else
	//{
	//	////cout << "Service time is ";
	//	_servTime->PrintDistribution();
	//	////cout << endl;
	//}
	//////cout << "		Required Resources: " << _reqRes << endl;
	//////cout << "		Required Parts: " << _reqParts << endl;
	////cout << "		Return Step If Inspection Fails: " << _returnStep << endl;

	////cout << "			Parts - " << endl;
		PrintParts();
	////cout << endl;

	////cout << "			Resource - " << endl;
	map<string, StepResource*>::iterator it = _reqResourceMap.begin();
	for (int i = 0; i < _reqResourceMap.size(); i++)
	{
		//	////cout << "			" << it->first << endl;
		it++;
	}
	PrintResources();
	////cout << endl;
}

void Step::PrintParts()
{
	map<string, Parts*>::iterator it = _reqPartsMap.begin();
	//////cout << "After creating the iterator to the map " << std::endl; 
	while (it != _reqPartsMap.end())
	{
		//////cout << "in the loop \n"

		it->second->PrintPartsProperties();
		it++;
	}
}

void Step::PrintResources()
{
	map<string, StepResource*>::iterator it = _reqResourceMap.begin();
	//	////cout << "MAP SIZE: " << _reqResourceMap.size() << endl;
	while (it != _reqResourceMap.end())
	{
		it->second->PrintResProperties();
		it++;
	}
}

void Step::PrintPools()
{
	////cout << "RESOURCE POOL" << endl;
	map<string, StepResource*>::iterator it = _resourcePool.begin();
	////cout << "MAP SIZE: " << _resourcePool.size() << endl;
	while (it != _resourcePool.end())
	{
		it->second->PrintResProperties();
		it++;
	}

	////cout << endl;

	////cout << "PARTS POOL:" << endl;
	map<string, Parts*>::iterator it2 = _partsPool.begin();
	////cout << "MAP SIZE: " << _partsPool.size() << endl;
	while (it2 != _partsPool.end())
	{
		it2->second->PrintPartsProperties();
		it2++;
	}

}
