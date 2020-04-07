//Scribe.cpp: Kevin Voyt

#include "Scribe.h"

aircraftNode::aircraftNode()
{
	type = "";
	count = 0;

	next = nullptr;
}

aircraftNode::aircraftNode(string aircraftType)
{
	type = aircraftType;
	count = 0;

	next = nullptr;
}

aircraftNode::aircraftNode(const aircraftNode& node2)
{
	type = node2.type;
	count = node2.count;
	next = node2.next;
}

aircraftNode::~aircraftNode()
{
	delete this;
}

missionNode::missionNode()
{
	type = "";

	next = nullptr;
}

missionNode::missionNode(string missionName)
{
	type = missionName;

	next = nullptr;
}

missionNode::missionNode(const missionNode& node2)
{
	type = node2.type;
	next = node2.next;

}

missionNode::~missionNode()
{
	delete this;
}

resourceNode::resourceNode()
{
	type = "";
	initialCount = 0;
	utilizationHours = 0;
	utilizationPercent = 0;
	requestNumber = 0;
	unsuccessfulRequests = 0;

	next = nullptr;
}

resourceNode::resourceNode(string resourceName, int initialization)
{
	type = resourceName;
	initialCount = initialization;
	utilizationHours = 0;
	utilizationPercent = 0;
	requestNumber = 0;
	unsuccessfulRequests = 0;

	next = nullptr;
}

resourceNode::resourceNode(const resourceNode& node2)
{
	type = node2.type;
	initialCount = node2.initialCount;
	utilizationHours = node2.utilizationHours;
	utilizationPercent = node2.utilizationPercent;
	requestNumber = node2.requestNumber;
	unsuccessfulRequests = node2.unsuccessfulRequests;
	next = node2.next;
}

resourceNode::~resourceNode()
{
	delete this;
}

failureNode::failureNode()
{
	resourceType = "";
	failureType = "";
	duration = 0;

	next = nullptr;
}

failureNode::failureNode(string resource, string failure, float time)
{
	resourceType = resource;
	failureType = failure;
	duration = time;

	next = nullptr;
}

failureNode::failureNode(const failureNode& node2)
{
	resourceType = node2.resourceType;
	failureType = node2.failureType;
	duration = node2.duration;
	next = node2.next;
}

failureNode::~failureNode()
{
	delete this;
}

resourceWaitNode::resourceWaitNode()
{
	aircraftType = "";
	aircraftID = 0;
	resourceType = "";
	timeStart = 0;
	timeEnd = 0;
	duration = -1;

	next = nullptr;
}

resourceWaitNode::resourceWaitNode(string aircraft, int ID, string resource, float time)
{
	aircraftType = aircraft;
	aircraftID = ID;
	resourceType = resource;
	timeStart = time;
	timeEnd = 0;
	duration = -1;

	next = nullptr;
}

resourceWaitNode::resourceWaitNode(const resourceWaitNode& node2)
{
	aircraftType = node2.aircraftType;
	aircraftID = node2.aircraftID;
	resourceType = node2.resourceType;
	timeStart = node2.timeStart;
	timeEnd = node2.timeEnd;
	duration = node2.duration;
	next = node2.next;
}

resourceWaitNode::~resourceWaitNode()
{
	delete this;
}

serviceWaitNode::serviceWaitNode()
{
	aircraftType = "";
	aircraftID = 0;
	location = "";
	timeStart = 0;
	timeEnd = 0;
	duration = -1;

	next = nullptr;
}

serviceWaitNode::serviceWaitNode(string aircraft, int ID, string spot, float time)
{
	aircraftType = aircraft;
	aircraftID = ID;
	location = spot;
	timeStart = time;
	timeEnd = 0;
	duration = -1;

	next = nullptr;
}

serviceWaitNode::serviceWaitNode(const serviceWaitNode& node2)
{
	aircraftType = node2.aircraftType;
	aircraftID = node2.aircraftID;
	location = node2.location;
	timeStart = node2.timeStart;
	timeEnd = node2.timeEnd;
	duration = node2.duration;
	next = node2.next;
}

serviceWaitNode::~serviceWaitNode()
{
	delete this;
}

repairJobNode::repairJobNode()
{
	aircraftType = "";
	jobType = "";
	timeStart = 0;
	timeEnd = 0;
	duration = -1;

	next = nullptr;
}

repairJobNode::repairJobNode(string aircraft, string job, float time)
{
	aircraftType = aircraft;
	jobType = job;
	timeStart = time;
	timeEnd = 0;
	duration = -1;

	next = nullptr;
}

repairJobNode::repairJobNode(const repairJobNode& node2)
{
	aircraftType = node2.aircraftType;
	jobType = node2.jobType;
	timeStart = node2.timeStart;
	timeEnd = node2.timeEnd;
	duration = node2.duration;
	next = node2.next;
}

repairJobNode::~repairJobNode()
{
	delete this;
}

reworkNode::reworkNode()
{
	objectType = "";
	reworkEvent = "";
	duration = 0;

	next = nullptr;
}

reworkNode::reworkNode(string object, string rework, float time)
{
	objectType = object;
	reworkEvent = rework;
	duration = time;

	next = nullptr;
}

reworkNode::reworkNode(const reworkNode& node2)
{
	objectType = node2.objectType;
	reworkEvent = node2.reworkEvent;
	duration = node2.duration;
	next = node2.next;
}

reworkNode::~reworkNode()
{
	delete this;
}

partRequestNode::partRequestNode()
{
	partType = "";
	numberUsed = 0;
	requestNumber = 0;
	unsuccessfulRequests = 0;

	next = nullptr;
}

partRequestNode::partRequestNode(string part)
{
	partType = part;
	numberUsed = 0;
	requestNumber = 0;
	unsuccessfulRequests = 0;

	next = nullptr;
}

partRequestNode::partRequestNode(const partRequestNode& node2)
{
	partType = node2.partType;
	numberUsed = node2.numberUsed;
	requestNumber = node2.requestNumber;
	unsuccessfulRequests = node2.unsuccessfulRequests;
	next = node2.next;
}

partRequestNode::~partRequestNode()
{
	delete this;
}

restockNode::restockNode()
{
	partType = "";
	restockTime = 0;

	next = nullptr;
}

restockNode::restockNode(string part, float time)
{
	partType = part;
	restockTime = time;

	next = nullptr;
}

restockNode::restockNode(const restockNode& node2)
{
	partType = node2.partType;
	restockTime = node2.restockTime;
	next = node2.next;
}

restockNode::~restockNode()
{
	delete this;
}

runNode::runNode()
{
	aircraftHead = nullptr;
	aircraftRunner = nullptr;
	aircraftTail = nullptr;

	missionHead = nullptr;
	missionRunner = nullptr;
	missionTail = nullptr;

	resourceHead = nullptr;
	resourceRunner = nullptr;
	resourceTail = nullptr;
}

runNode::runNode(const runNode& node2)
{
	aircraftHead = node2.aircraftHead;
	missionHead = node2.missionHead;
	resourceHead = node2.resourceHead;
	resourceWaitHead = node2.resourceWaitHead;
	serviceWaitHead = node2.serviceWaitHead;
	repairJobHead = node2.repairJobHead;
	reworkHead = node2.reworkHead;
	requestsHead = node2.requestsHead;
	restockHead = node2.restockHead;

	aircraftTail = node2.aircraftTail;
	missionTail = node2.missionTail;
	resourceTail = node2.resourceTail;
	resourceWaitTail = node2.resourceWaitTail;
	serviceWaitTail = node2.serviceWaitTail;
	repairJobTail = node2.repairJobTail;
	requestsTail = node2.requestsTail;
	restockTail = node2.restockTail;

	aircraftRunner = nullptr;
	missionRunner = nullptr;
	resourceRunner = nullptr;
	resourceWaitRunner = nullptr;
	serviceWaitRunner = nullptr;
	repairJobRunner = nullptr;
	partRequestRunner = nullptr;
	restockRunner = nullptr;

	next = node2.next;
}

runNode::~runNode()
{
	delete this;
}

Scribe::Scribe()
{
	runStart = nullptr;
	runEnd = nullptr;
	runCurrent = nullptr;
}

Scribe::Scribe(const Scribe& node2)
{
	runStart = node2.runStart;
	runEnd = node2.runEnd;
	runCurrent = node2.runCurrent;
}

Scribe::~Scribe()
{
	delete this;
}

//create node con
void Scribe::RecordAircraft(string aircraft)
{
	//aircraft list empty
	if (runCurrent->aircraftHead == nullptr)
	{
		runCurrent->aircraftHead = new aircraftNode(aircraft);
	}
	//search for aircraft type

}

//increment aircraft throughput
void Scribe::TallyAircraft(string aircraft)
{
}

//Create node containing a mission type
void Scribe::RecordMission(string mission)
{
}

//Create a node containing a resource and an initial availability
//Create a request node for the same resource
void Scribe::RecordResource(string resource, int count)
{
}

//Update the utilization values for a specific resource
void Scribe::UpdateResourceUtilization(string resource, int usage)
{
}

//update the requests for a specific resource
void Scribe::UpdateResourceRequests(string resource, bool successful)
{
}

//Create a node containing an aircraft type, failure type, and a failure duration
void Scribe::RecordFailure(string aircraft, string failure, float time)
{
}

//Create a node containing an aircraft type, aircraft ID, resource type and a start time
void Scribe::RecordResourceWait(string aircraft, int id, string resource, float start)
{
}

/*Search for an existing resource wait node with a specified aircraft ID, resource type, and duration of -1 to record
   the end time of the node and calculate the duration
*/
void Scribe::RecordResourceWaitEnd(int id, string resource, float end)
{
}

//Create a node containing an aircraft type, aircraft ID, location and start time
void Scribe::RecordServiceWait(string aircraft, int id, string spot, float start)
{
}

/*Search for an existing service wait node with a specified aircraft ID, location and duration of -1 to record
  the end time of the node and calculate duration
*/
void Scribe::RecordServiceWaitEnd(int id, string spot, float end)
{
}

//Create a node containing an aircraft type, a repair job, and a start time
void Scribe::RecordRepairJob(string aircraft, int id, string job, float start)
{
}

/*Search for an existing repair node with a specified aircraft id, repair job and duration of -1 to record
	the end time of the node and calculate duration
*/
void Scribe::RecordRepairEnd(int, string, float)
{
}

//Create a node containing (an aircraft or resource type), a rework event and a duration
void Scribe::RecordRework(string object, string rework, float time)
{
}

//Search request nodes for a specific resource and update both number used and requests
void Scribe::RecordPartRequest(string resource, int usage, bool success)
{
}

//Create a node containing a resource and a duration
void Scribe::RecordRestock(string resource, float time)
{
}

//Record information in external file
void Scribe::Archive()
{
	//Simulation data

	//Aircraft data for each run

	//Mission data for each run

	//Resource data for each run

	//Failure data for each run

	//Resource wait data for each run

	//Service wait data for each run

	//Repair job data for each run

	//Rework data for each run

	//Part Request data for each run

	//Restock data for each run
}

