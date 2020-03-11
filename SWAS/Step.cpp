#include "Step.h"
#include "Distribution.h"
#include "SimExec.h"
#include "Aircraft.h"
#include <iterator>
#include <map>


map<string, Resource*> Step::_resourcePool;
map<string, Parts*> Step::_partsPool;

Step::Step(string serviceTime, string name) : Task(name)
{
	_servTime = serviceTime;
	_name = name;
}

////////////////////////////////////////////
/////////EVENT ACTIONS AND METHODS//////////
////////////////////////////////////////////

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

void Step::PlaceOrderEM(Parts* parts)
{
	//is creating an instance of the event action how i pass the 
	//event action in the schedule at function?
	PlaceOrderEA* ea;

	//give priority of event, event action object, distribution for lead time
// 	SimExec::ScheduleEventAt(1, ea, parts->GetLeadTime());

}


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

void Step::OrderArrivalEM(Parts* parts)
{
	//Logic: new count = current parts count + (initial count - current parts count)
	int newCount;

	//replace parts->SetPartsCount and GetPartsCount with the same from the parts pool
	map<string, Parts*>::const_iterator iter = _reqPartsMap.find(parts->GetPartsName());
	newCount = iter->second->GetPartsCount();
	
	parts->SetPartsCount(newCount);
}


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

void Step::StartServiceEM(Aircraft* aircraft, vector<string> _acquiredResources)
{
	//check if acquired resource vector has bay or spot
	bool hasResource = false;

	for (int i = 0; i < _acquiredResources.size(); i++)
	{
		if (_indoorReq == 'y')
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

	// previously, you had a resource pool and gave that resource pool to the tasks (acquire, release, join)
	// so does that mean you have to pass the resources down to step? so does step have a static container
	// of resources or pools that is populated at initialization, (consequently shared and edited by all the steps?)
	// in that case you must ensure that when a new step is created that it doesn't "zero out" the container
	// container is a container of resource object pointers, one for each possible resource

	// if no
	if (hasResource == false)
	{
		// if inside step
		if (_indoorReq == 'y')
		{
			map<string, Resource*>::iterator it = _resourcePool.find("bay");
			if (it->second->GetResourceCount() > 0)
			{
				// call decrement function, push it to acquired vector 
				_acquiredResources.push_back(it->first);
			}
			else
				cout << "we gotta wait for a bay \n";
			// wait?? 
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
				cout << "we gotta wait for an outspot \n";
			// wait?? 
		}
	}

	// number of bays initialized with simulation initialization from GUI, where are they stored (aka how does Step
	// get access to this information), how do I check if a bay is available


		//if no
			//if inside step (DEON: indoor req is in repairjob)
				//check if bay is available
					//if yes
				//acquire bay
					//if no
						//wait 

			//if outside step
				//check if spot is available
					//if yes
						//acquire spot
					//if no
						//wait
		//if yes
			//continue

	if (_name == "process")
	{
		map<string, Resource*>::const_iterator iter = _reqResourceMap.begin();
		//for all resources listed in required map
		while (iter != _reqResourceMap.end())
		{
			//compare key to acquired resources vector
			for (int i = 0; i < _acquiredResources.size(); i++)
			{
				if (iter->first == _acquiredResources[i])
					break;
			}

			map<string, Resource*>::iterator it = _resourcePool.find(iter->first);

			if (it->second->GetResourceCount() <= it->second->GetNumResNeeded())
			{
				//decrement appropriately
				_acquiredResources.push_back(it->first);
			}
			else
				cout << " we gotta wait for a/an " << it->first << endl;
			//if not found
				//if num of required resources is < or = num of avail resources
					//acquire the object (num of available resources decrements)
				//else
					//wait til available (logic)

			iter++;
		}


		map<string, Parts*>::const_iterator iter2 = _reqPartsMap.begin();
		//for all parts listed in required map
		while (iter2 != _reqPartsMap.end())
		{
			map<string, Parts*>::iterator it = _partsPool.find(iter2->first);

			if (it->second->AreEnoughParts() == true)
				cout << "need to decrement correct parts" << endl;
			//decrement num of avail parts
			else if (it->second->AreEnoughParts() == false)

				//	it->second->PlaceOrder();




					//if num of required parts is < or = num of avail parts
					//acquire parts (num of avail parts decrement)
				//else
					//wait til available (logic)
				iter2++;
		}

		//schedule Done service EA

		//check if there are more steps
			//if yes
				//
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

void Step::ScheduleDoneServiceEM(Aircraft* aircraft)
{
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

}


class Step::AddQueueEA : public EventAction {
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
};

void Step::AddQueueEM(Aircraft* aircraft)
{
	//	_PriorityQueue.insert(pair<int, Aircraft*>(aircraft->GetAircraftPriority(), aircraft));

	//_numberInQueue++;
	//if (_bays > 0) {
	//	//SimExec::ScheduleEventIn(0.0, new AcquireServerEA(this, aircraft));
	//}
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


////////////////////////////////////////////
/////////////     OTHER      ///////////////
////////////////////////////////////////////

map<string, Resource*>::iterator Step::FindResource(string resource)
{
	return _reqResourceMap.find(resource); 
}

map<string, Parts*>::iterator Step::FindParts(string resource)
{
	return _reqPartsMap.find(resource);
}

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
	//cout << "After creating the iterator to the map " << std::endl; 
//	cout << "MAP SIZE: " << _reqResourceMap.size() << endl;
	while (it != _reqResourceMap.end())
	{
		//cout << "in the loop \n";

		it->second->PrintResProperties();
		it++;
	}
}