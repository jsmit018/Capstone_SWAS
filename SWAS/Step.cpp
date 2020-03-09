#include "Step.h"
#include "Distribution.h"
#include "SimExec.h"
#include "Aircraft.h"
#include <iterator>
#include <map>

Step::Step(string serviceTime, string name) : Task(name)
{
	_servTime = serviceTime;
	_name = name;
}

/* For @ARobey 
	Waiting logic for an aircraft that doesn't have the right resources -> We won't want to just yank it out of the event list but we also can't leave
	it alone. I suggest we just move the event make it have the highest priority first thing the next day to check if it is available. 
*/

class Step::StartServiceEA : public EventAction {
public:
	StartServiceEA(Step* step, Aircraft* aircraft, RepairJob* repairJob) {
		_step = step;
		_aircraft = aircraft;
	}

	void Execute() {
	//	_step->StartServiceEM(_aircraft, );
	}
private:
	Step* _step;
	Aircraft* _aircraft;
};

//class Step::ScheduleDoneServiceEA : public EventAction {
//public:
//
//private:
//};

//class Step::AddQueueEA : public EventAction {
//public:
//	AddQueueEA(Step* step, Aircraft* aircraft) {
//		_step = step;
//		_aircraft = aircraft;
//	}
//
//	void Execute() {
//		_step->AddQueueEM(_aircraft);
//	}
//
//private:
//	Step* _step;
//	Aircraft* _aircraft;
//};


string Step::GetName()
{
	return _name;
}
int Step::GetNumberInQueue()
{
	//return _numberInQueue;
	return 0;
}




//Acquiring a bay and other resources
void Step::StartServiceEM(Aircraft* aircraft, vector<string> _acquiredResources)
{
	if (_name == "process")
	{
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		//for all resources listed in required map
		while (iter != _reqResourceMap.end())
		{
			//compare key to acquired resources vector
			//if not found
				//if num of required resources is < or = num of avail resources
					//acquire the object (num of available resources decrements)
				//else
					//wait til available (logic)
					//@ARobey from what we agreed on the last time we spoke is that we'd reschedule 12 hours later to check.
					//You'll just need to plug in the priority value and the repair job.
					//Wherever you need the wait logic please just copy the below schedule statement.
					//SimExec::ScheduleEventAt(/ priority / +65, new StartServiceEA(this, aircraft, / repairjob / ), new Constant(12), "StartServiceEA");

			iter++;
		}
		
		map<string, Parts*>::const_iterator iter2 = _reqPartsMap.begin();
		//for all parts listed in required map
		while (iter2 != _reqPartsMap.end())
		{
			//if num of required parts is < or = num of avail parts
				//acquire parts (num of avail parts decrement)
			//else
				//wait til available (logic)
			iter2++;
		}

		//schedule Done service EA
	}

	else if (_name == "inspection")
	{
		//for all resources listed in required map
			//compare key to acquired resources vector
			//if not found
				//if resource is available
					//acquire the object
				//else
					//wait til available (logic)

		//if inspection results in failure = true
			//schedule service EA
		//else
			//schedule Done service EA
	}	



	//SimExec::ScheduleEventIn(0.0, new WhateverEAIsCalled(this.nextStep, aircraft));
}

void Step::AddQueueEM(Aircraft* aircraft)
{
	//	_PriorityQueue.insert(pair<int, Aircraft*>(aircraft->GetAircraftPriority(), aircraft));

	//_numberInQueue++;
	//if (_bays > 0) {
	//	//SimExec::ScheduleEventIn(0.0, new AcquireServerEA(this, aircraft));
	//}
}

void Step::ScheduleDoneServiceEM(Aircraft* aircraft)
{
	//if resources I have are needed for next step
		//keep
	//else
		//schedule release resource ea
}

//@TODO figure out what these are supposed to be... EM?
//void Step::ReleaseServer()
//{
//}
//
//void Step::DoneService()
//{
//}

void Step::Execute(Aircraft* aircraft) {
	//SimExec::ScheduleEventIn(0.0, new AddQueueEA(this, aircraft));
}

void Step::AddResource(Resource* resource, string resourceName)
{
	_reqResourceMap[resourceName] = resource;
	//_reqResourceVec.push_back(resource);

}

void Step::AddParts(Parts* parts, string partsName)
{
	_reqPartsMap[partsName] = parts;
	//_reqPartsVec.push_back(parts);

}

////////////////// Setters /////

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
	_inspecFailProb = failureProb;
}

void Step::SetServiceTime(string serviceTime)
{
	_servTime = serviceTime;
}

void Step::SetReqResource(string reqResource)
{
	_reqRes = reqResource;
}


void Step::SetReqParts(string reqParts)
{
	_reqParts = reqParts;
}


void Step::SetReturnStep(/*int stepId*/ int returnStep)
{
	//_returnStep = stepId;
	_returnStep = returnStep;
}


//////GETTERS
Resource* Step::GetResourceObj(string name)
{
	map<string, Resource*>::iterator it = _reqResourceMap.find(name);
	if (it == _reqResourceMap.end())
		return nullptr;
	return it->second;
}


void Step::Print()
{
	cout << "		Step Name: " << _name << endl;
	cout << "		Step Type: " << _type << endl;
	cout << "		Inspection Failure Probability: " << _inspecFailProb << endl;
	cout << "		Service Time Distribution: " << _servTime << endl;
	cout << "		Required Resources: " << _reqRes << endl;
	cout << "		Required Parts: " << _reqParts << endl;
	cout << "		Return Step If Inspection Fails: " << _returnStep << endl;
	
}