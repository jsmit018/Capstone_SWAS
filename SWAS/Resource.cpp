#include "Resource.h"
#include "SimExec.h"
#include "Scribe.h"


StepResource::StepResource()
{
	//_failureName = "none specified yet";  
	//_failureType = "none specified yet";  
	//_repairProc = "none specified yet";  
}

class StepResource::WaitForResourceEA : public CondEventAction {
public:
	WaitForResourceEA(StepResource* resource, int amountNeeded) {
		_resource = resource;  
		_amountNeeded = amountNeeded;  
	}

	bool Condition(StepResource* resource, Parts* parts) {
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
		//Scribe::RecordResourceWaitEnd(_aircraft->GetAircraftID(), _resource->GetResourceName(), SimExec::GetSimulationTime()._timeOfDay);  
		_resource->FailResourceEM(_resource);  
	}

private:
	StepResource* _resource;  
	int _amountNeeded;  
};  

class StepResource::FailResourceEA : public EventAction {
public:
	FailResourceEA(StepResource* resource) {
		//_step = step;  
		_resource = resource;  
	}

	void Execute() {
		_resource->FailResourceEM(_resource);  
	}
private:
	//Step* _step;  
	StepResource* _resource;  
};  

class StepResource::RestoreResourceEA : public EventAction {
public:
	RestoreResourceEA(StepResource* resource) {
		//_step = step;  
		_resource = resource;  
	}

	void Execute() {
		_resource->RestoreResourceEM(_resource);  
	}

private:
	//Step* _step;  
	StepResource* _resource;  
};  

void StepResource::CopyMapResource(const StepResource& mapResource)
{
	_resourceCount = mapResource._resourceCount;  
	_numNeeded = mapResource._numNeeded;  
	_resourceName = mapResource._resourceName;  
	_length = mapResource._length;  
	_width = mapResource._width;  
	_failureName = mapResource._failureName;  
	_failureType = mapResource._failureType;  
	_repairProc = mapResource._repairProc;  
	_shiftOneCount = mapResource._shiftOneCount;  
	_shiftTwoCount = mapResource._shiftTwoCount;  
	_shiftThreeCount = mapResource._shiftThreeCount;  

	if (mapResource._failureDist == nullptr)
		_failureDist = nullptr;   
	else
		_failureDist = mapResource._failureDist->CopyThis();  
}

//@TODO will need to figure out logic for what happens if amount needed is greater
void StepResource::Acquire(int amountNeeded)
{
	//if there aren't enough resources for this step
	if (amountNeeded > _resourceCount) {
		//continuously check when available and schedule now when its avail
	}
	else {
		//otherwise, subtract amount needed from resource count
		_resourceCount -= amountNeeded;  
	}

	_resourceCount -= amountNeeded;  
}

void StepResource::Release(int amountToRelease)
{
	_resourceCount += amountToRelease;  
}

bool StepResource::IsAvailable(int amountNeeded)
{
	if (amountNeeded > _resourceCount)
		return false;  
	else {
		return true;  
	}
}

void StepResource::FailResource()
{
	//@TODO write the algorithm for a resource failure essentially is just scheduling an event
	//so needed EA and EM
	_resourceCount--;  
}

void StepResource::RestoreResource()
{
	//May probably need an associated Event to Execute function call. 
	_resourceCount++;  
}

///////////////////////////////
////  GETTERS AND SETTERS  ////
///////////////////////////////

void StepResource::SetShiftOneCount(int shiftcount)
{
	_shiftOneCount = shiftcount;  
};  

double StepResource::GetShiftOneCount()
{
	return _shiftOneCount;  
};  

void StepResource::SetShiftTwoCount(int shiftcount)
{
	_shiftTwoCount = shiftcount;  
};  

double StepResource::GetShiftTwoCount()
{
	return _shiftTwoCount;  
};  

void StepResource::SetShiftThreeCount(int shiftcount)
{
	_shiftThreeCount = shiftcount;  
};  

double StepResource::GetShiftThreeCount()
{
	return _shiftThreeCount;  
}


void StepResource::SetResourceCount(double resourceCount)
{
	_resourceCount = resourceCount;  
}

double StepResource::GetResourceCount()
{
	return _resourceCount;  
}

void StepResource::SetResourceName(string resourceName)
{
	_resourceName = resourceName;  
}

string StepResource::GetResourceName()
{
	return _resourceName;  
}

void StepResource::SetNumResNeeded(int numNeeded)
{
	//get from step table values 
	_numNeeded = numNeeded;  
}

int StepResource::GetNumberOfResroucesNeeded()
{
	return _numNeeded;  
}

void StepResource::SetResourceFootprint(double length, double width)
{
	_length = length;  
	_width = width;  
}

double StepResource::GetResourceFootprint()
{
	return _length, _width;  
}

void StepResource::SetFailureName(string failureName)
{
	_failureName = failureName;  
}

string StepResource::GetFailureName()
{
	return _failureName;  
}

void StepResource::SetFailureType(string failureType)
{
	_failureType = failureType;  
}

string StepResource::GetFailureType()
{
	return _failureType;  
}

void StepResource::SetFailureDistr(string failureDistr)
{
	//turn failure distr from string into distributions
	istringstream failDist(failureDistr);  
	string firstHalf;  
	string secHalf;  

	getline(failDist, firstHalf, '(');  
	getline(failDist, secHalf, ')');  
	//	cout << "first: " << firstHalf << endl;  
	//	cout << "sec: " << secHalf << endl;  

	istringstream nums(secHalf);  
	if (firstHalf == "Triangular" || firstHalf == "Tri")
	{
		double min, expected, max;  
		nums >> min;  
		nums >> expected;  
		nums >> max;  
		_failureDist = new Triangular(min, expected, max);  
	}

	else if (firstHalf == "Exponential")
	{
		double mean;  
		nums >> mean;  

		_failureDist = new Exponential(mean);  
	}

	else if (firstHalf == "Uniform")
	{
		double min, max;  
		nums >> min >> max;  

		_failureDist = new Uniform(min, max);  
	}

	else if (firstHalf == "Normal")
	{
		double mean, stdev;  
		nums >> mean >> stdev;  

		_failureDist = new Normal(mean, stdev);  
	}

	else if (firstHalf == "Poisson")
	{
		double mean;  
		nums >> mean;  

		_failureDist = new Poisson(mean);  
	}

	else if (firstHalf == "Constant" || firstHalf == "Fixed")
	{
		double mean;  
		nums >> mean;  

		_failureDist = new Constant(mean);  
	}

	else if (firstHalf == "Weibull")
	{
		double scale, shape;  
		nums >> scale >> shape;  

		_failureDist = new Weibull(scale, shape);  
	}

	//Determines correct distribution and prints
	//_failureDist->PrintDistribution();  

}

Distribution* StepResource::GetFailureDistr()
{
	return _failureDist;   //check if this works
}

void StepResource::SetRepairProcess(string repairProc)
{
	_repairProc = repairProc;  
}

void StepResource::ScheduleFirstFailures(StepResource* resource)
{
	//SimExec::ScheduleEventAtRecurring(0, new FailResourceEA(resource), 2, "FailResourceEA");  
	SimExec::ScheduleEventAtRecurring(0, new FailResourceEA(resource), 1, "FailResourceEA");  
}

string StepResource::GetRepairProcess()
{
	return _repairProc;  
}

bool StepResource::IsAfterCEL()
{
	if (_CELflag == 1)
		return true;  
	if (_CELflag == 0)
		return false;  
}

void StepResource::SetCELflag(int CELflag)
{
	_CELflag = CELflag;  
}


void StepResource::PrintResProperties()
{
	cout << "			Resource name: " << _resourceName << endl;  
	cout << "			Initial count: " << _resourceCount << endl;  
	cout << "			Resource footprint X: " << _length << endl;  
	cout << "			Resource footprint Y: " << _width << endl;  
	cout << "			Failure name: " << _failureName << endl;  
	cout << "			Failure type: " << _failureType << endl;  
	cout << "			Failure distr: ";  
	if (_failureDist == nullptr)
	{
		cout << " no failure distr" << endl;  
	}
	else
		_failureDist->PrintDistribution();  
	cout << endl;  
	cout << "			Repair process: " << _repairProc << endl;  
	cout << endl;  
}

//void StepResource::SetResourceID(int resourceID)
//{
//	_resourceID = resourceID;  
//}
//
//int StepResource::GetResourceID()
//{
//	return _resourceID;  
//}

void StepResource::FailResourceEM(StepResource* resource)
{
	//int newCount;  
	//Mark added this i'm not sure we need to create a new instance, but i'm just going to put a priority of 1 - Jordan
	//RepairJob* newJob;  

	//map<string, Resource*>::const_iterator iter = _resourcePool.find(resource->GetResourceName());  

	//newCount = iter->second->GetResourceCount() - 1;  
	//SetResPoolCount(iter->first, newCount);  
	//resource->SetResourceCount(newCount);  
	//if (resource->GetResourceCount() > 0) {
		//Scribe::RecordFailure(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetSimulationTime()._timeOfDay);  
		resource->FailResource();  

		//SimExec::ScheduleEventAt(newJob->GetPriority(), new FailResourceEA(this, resource), iter->second->GetFailureDistr()->GetRV(), "New Repair Job");  
		//This Event action should actually be scheduling a restore resource instead of a fail one.
		//cout << "Resource has failed, scheduling a restore resource" << endl;  
		//SimExec::ScheduleEventAt(1, new RestoreResourceEA(this, resource), this->_servTime->GetRV(), "RestoreResourceEA");  

		//Jordan: schedule next failure in iter->second->GetFailureDistr
		//SimExec::ScheduleEventAtRecurring(0, new RestoreResourceEA(resource), resource->GetFailureDistr()->GetRV(), "RestoreResourceEA");  
		SimExec::ScheduleEventAtRecurring(0, new RestoreResourceEA(resource),	2, "RestoreResourceEA");  
		Scribe::RecordFailure(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetSimulationTime()._timeOfDay);  
	//}	
	//else {
	//	//cout << "Waiting for next available resource to become available, and it will fail" << endl;  
	//	SimExec::ScheduleConditionalEvent(0, new WaitForResourceEA(resource, 1));  
	//}
}

void StepResource::RestoreResourceEM(StepResource* resource)
{
	//cout << "Resource has been restored, updating amount and checking conditional events" << endl;  
	resource->RestoreResource();  
	//SimExec::ScheduleEventAtRecurring(0, new FailResourceEA(resource), resource->GetFailureDistr()->GetRV(), "FailResourceEA");  
	Scribe::RecordRestore(resource->GetResourceName(), resource->GetFailureName(), SimExec::GetTotalSimulationTime());  
	SimExec::ScheduleEventAtRecurring(0, new FailResourceEA(resource), 2, "FailResourceEA");  
	SimExec::CheckConditionalEvents(resource, 0);  
}