//Scribe.cpp: Kevin Voyt

#include "Scribe.h"
#include "Distribution.h"

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
	availability = 0;
	lastchange = 0;
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
	availability = initialCount;
	lastchange = 0;
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
	availability = node2.availability;
	lastchange = node2.lastchange;
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

repairJobNode::repairJobNode(string aircraft,int id, string job, float time)
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

	failureHead = nullptr;
	failureRunner = nullptr;
	failureTail = nullptr;

	resourceWaitHead = nullptr;
	resourceWaitRunner = nullptr;
	resourceWaitTail = nullptr;

	serviceWaitHead = nullptr;
	serviceWaitRunner = nullptr;
	serviceWaitTail = nullptr;

	repairJobHead = nullptr;
	repairJobRunner = nullptr;
	repairJobTail = nullptr;

	reworkHead = nullptr;
	reworkRunner = nullptr;
	reworkTail = nullptr;

	requestsHead = nullptr;
	requestsRunner = nullptr;
	requestsTail = nullptr;

	restockHead = nullptr;
	restockRunner = nullptr;
	restockTail = nullptr;

	next = nullptr;
}

runNode::runNode(const runNode& node2)
{
	aircraftHead = node2.aircraftHead;
	missionHead = node2.missionHead;
	resourceHead = node2.resourceHead;
	failureHead = node2.failureHead;
	resourceWaitHead = node2.resourceWaitHead;
	serviceWaitHead = node2.serviceWaitHead;
	repairJobHead = node2.repairJobHead;
	reworkHead = node2.reworkHead;
	requestsHead = node2.requestsHead;
	restockHead = node2.restockHead;

	aircraftTail = node2.aircraftTail;
	missionTail = node2.missionTail;
	resourceTail = node2.resourceTail;
	failureTail = node2.failureTail;
	resourceWaitTail = node2.resourceWaitTail;
	serviceWaitTail = node2.serviceWaitTail;
	repairJobTail = node2.repairJobTail;
	reworkTail = node2.reworkTail;
	requestsTail = node2.requestsTail;
	restockTail = node2.restockTail;

	aircraftRunner = nullptr;
	missionRunner = nullptr;
	resourceRunner = nullptr;
	failureRunner = nullptr;
	resourceWaitRunner = nullptr;
	serviceWaitRunner = nullptr;
	repairJobRunner = nullptr;
	reworkRunner = nullptr;
	requestsRunner = nullptr;
	restockRunner = nullptr;

	next = node2.next;
}

runNode::~runNode()
{
	delete this;
}

Scribe::Scribe()
{
	runNumber = 1;

	runStart = new runNode();
	runEnd = runStart;
	runCurrent = runStart;

	runtime = 0;
	planned = 0;
	unplanned = 0;

	SetWarehousDims(0, 0);
	SetSeed(0);
	totalRuntime = 0;
	fileName = "";
}

Scribe::Scribe(int runs)
{
	runNumber = runs;
	runStart = nullptr;
	runCurrent = nullptr;
	runEnd = nullptr;
	for (int i = 0; i < runNumber; i++)
	{
		//No run nodes
		if (runStart == nullptr)
		{
			runStart = new runNode();
			runEnd = runStart;
			runCurrent = runStart;
		}
		else
		{
			runEnd->next = new runNode();
			runEnd = runEnd->next;
		}
	}

	runtime = 0;
	planned = 0;
	unplanned = 0;
	SetWarehousDims(0, 0);
	SetSeed(0);
	totalRuntime = 0;
	fileName = "";
}

Scribe::Scribe(const Scribe& node2)
{
	runNumber = node2.runNumber;

	runStart = node2.runStart;
	runEnd = node2.runEnd;
	runCurrent = node2.runCurrent;

	runtime = node2.runtime;
	planned = node2.planned;
	unplanned = node2.unplanned;

	warehouseL = node2.warehouseL;
	warehouseW = node2.warehouseW;
	seedVal = node2.seedVal;
	totalRuntime = node2.totalRuntime;
	fileName = node2.fileName;
}

Scribe::~Scribe()
{
	delete this;
}

//create node containing an aircraft type
void Scribe::RecordAircraft(string aircraft)
{
	//aircraft list empty
	if (runCurrent->aircraftHead == nullptr)
	{
		runCurrent->aircraftHead = new aircraftNode(aircraft);
		runCurrent->aircraftTail = runCurrent->aircraftHead;
	}
	else
	{
		//search for aircraft type
		bool exists = false;
		runCurrent->aircraftRunner = runCurrent->aircraftHead;
		do
		{
			//check type against aircraft
			if (runCurrent->aircraftRunner->type == aircraft)
			{
				exists = true;
			}
			else
			{
				//advance runner
				runCurrent->aircraftRunner = runCurrent->aircraftRunner->next;
			}
		} while (runCurrent->aircraftRunner != nullptr);
		
		//exists being false means no match found
		if (!exists)
		{
			runCurrent->aircraftTail = new aircraftNode(aircraft);
			runCurrent->aircraftTail = runCurrent->aircraftTail->next;
		}
	}

}

//increment aircraft throughput
void Scribe::TallyAircraft(string aircraft)
{
	runCurrent->aircraftRunner = runCurrent->aircraftHead;
	do
	{
		if (runCurrent->aircraftRunner->type == aircraft)
		{
			runCurrent->aircraftRunner->count++;
		}
		else
		{
			runCurrent->aircraftRunner = runCurrent->aircraftRunner->next;
		}
	} while (runCurrent->aircraftRunner != nullptr);
	
}

//Create node containing a mission type
void Scribe::RecordMission(string mission)
{
	//mission list is empty
	if (runCurrent->missionHead == nullptr)
	{
		runCurrent->missionHead = new missionNode(mission);
		runCurrent->missionTail = runCurrent->missionHead;
	}
	else
	{
		bool exists = false;
		runCurrent->missionRunner = runCurrent->missionHead;
		do
		{
			if (runCurrent->missionRunner->type == mission)
			{
				exists = true;
			}
			else
			{
				runCurrent->missionRunner = runCurrent->missionRunner->next;
			}
		} while (runCurrent->missionRunner != nullptr);
		

		if (!exists)
		{
			runCurrent->missionTail->next = new missionNode(mission);
			runCurrent->missionTail = runCurrent->missionTail->next;
		}
	}
}

//Create a node containing a resource and an initial availability
void Scribe::RecordResource(string resource, int count)
{
	if (runCurrent->resourceHead == nullptr)
	{
		runCurrent->resourceHead = new resourceNode(resource, count);
		runCurrent->resourceTail = runCurrent->resourceHead;
	}
	else
	{
		bool exists = false;
		runCurrent->resourceRunner = runCurrent->resourceHead;
		do
		{
			if (runCurrent->resourceRunner->type == resource)
			{
				exists = true;
			}
			else
			{
				runCurrent->resourceRunner = runCurrent->resourceRunner->next;
			}
		} while (runCurrent->resourceRunner != nullptr);

		if (!exists)
		{
			runCurrent->resourceTail->next = new resourceNode(resource, count);
			runCurrent->resourceTail = runCurrent->resourceTail->next;
		}
	}
}

//Update the utilization values for a specific resource
void Scribe::UpdateResourceUtilization(string resource, int usage, float currentTime)
{
	
	runCurrent->resourceRunner = runCurrent->resourceHead;
	do
	{
		if (runCurrent->resourceRunner->type == resource)
		{
			runCurrent->resourceRunner->utilizationHours += (runCurrent->resourceRunner->initialCount - runCurrent->resourceRunner->availability) * (currentTime - runCurrent->resourceRunner->lastchange);
			runCurrent->resourceRunner->availability -= usage;
		}

		runCurrent->resourceRunner = runCurrent->resourceRunner->next;

	} while (runCurrent->resourceRunner != nullptr);
	
	
}

//Update utilization values at end of run - called automatically in SetRunTime() 
void Scribe::UpdateResourceUtilization()
{
	runCurrent->resourceRunner = runCurrent->resourceHead;
	do
	{
		runCurrent->resourceRunner->utilizationHours += (runCurrent->resourceRunner->initialCount - runCurrent->resourceRunner->availability) * (runtime - runCurrent->resourceRunner->lastchange);
		runCurrent->resourceRunner->utilizationPercent = (runCurrent->resourceRunner->utilizationHours) / (runCurrent->resourceRunner->initialCount * runtime) * 100;
	} while (runCurrent->resourceRunner != nullptr);
	
}

//update the requests for a specific resource
void Scribe::UpdateResourceRequests(string resource, bool successful)
{
	runCurrent->resourceRunner = runCurrent->resourceHead;
	do
	{
		if (runCurrent->resourceRunner->type == resource)
		{
			if (successful)
			{
				runCurrent->resourceRunner->requestNumber++;
			}
			else
			{
				runCurrent->resourceRunner->unsuccessfulRequests++;
			}
		}
		else
		{
			runCurrent->resourceRunner = runCurrent->resourceRunner->next;
		}
	} while (runCurrent->resourceRunner != nullptr);
}

//Create a node containing an aircraft type, failure type, and a failure duration
void Scribe::RecordFailure(string aircraft, string failure, float time)
{
	if (runCurrent->failureHead == nullptr)
	{
		runCurrent->failureHead = new failureNode(aircraft, failure, time);
		runCurrent->failureTail = runCurrent->failureHead;
	}
	else
	{
		runCurrent->failureTail->next = new failureNode(aircraft, failure, time);
		runCurrent->failureTail = runCurrent->failureTail->next;
	}
}

//Create a node containing an aircraft type, aircraft ID, resource type and a start time
void Scribe::RecordResourceWait(string aircraft, int id, string resource, float start)
{
	if (runCurrent->resourceWaitHead == nullptr)
	{
		runCurrent->resourceWaitHead = new resourceWaitNode(aircraft, id, resource, start);
		runCurrent->resourceWaitTail = runCurrent->resourceWaitHead;
	}
	else
	{
		runCurrent->resourceWaitTail->next = new resourceWaitNode(aircraft, id, resource, start);
		runCurrent->resourceWaitTail = runCurrent->resourceWaitTail->next;
	}
}

/*Search for an existing resource wait node with a specified aircraft ID, resource type, and duration of -1 to record
   the end time of the node and calculate the duration
*/
void Scribe::RecordResourceWaitEnd(int id, string resource, float end)
{
	runCurrent->resourceWaitRunner = runCurrent->resourceWaitHead;
	do
	{
		//Check 1: Is this the right aircraft?
		if (runCurrent->resourceWaitRunner->aircraftID == id)
		{
			//Check 2: Is this the resource it was waiting for?
			if (runCurrent->resourceWaitRunner->resourceType == resource)
			{
				//Check 3: Has the duration for this resource wait been updated?
				if (runCurrent->resourceWaitRunner->duration == -1)
				{
					runCurrent->resourceWaitRunner->timeEnd = end;
					runCurrent->resourceWaitRunner->duration = (runCurrent->resourceWaitRunner->timeEnd - runCurrent->resourceWaitRunner->timeStart);
				}
			}
		}
		
		runCurrent->resourceWaitRunner = runCurrent->resourceWaitRunner->next;

	} while (runCurrent->resourceWaitRunner != nullptr);
}

//Create a node containing an aircraft type, aircraft ID, location and start time
void Scribe::RecordServiceWait(string aircraft, int id, string spot, float start)
{
	if (runCurrent->serviceWaitHead == nullptr)
	{
		runCurrent->serviceWaitHead = new serviceWaitNode(aircraft, id, spot, start);
		runCurrent->serviceWaitTail = runCurrent->serviceWaitHead;
	}
	else
	{
		runCurrent->serviceWaitTail->next = new serviceWaitNode(aircraft, id, spot, start);
		runCurrent->serviceWaitTail = runCurrent->serviceWaitTail->next;
	}
}

/*Search for an existing service wait node with a specified aircraft ID, location and duration of -1 to record
  the end time of the node and calculate duration
*/
void Scribe::RecordServiceWaitEnd(int id, string spot, float end)
{
	runCurrent->serviceWaitRunner = runCurrent->serviceWaitHead;
	do
	{
		if ((runCurrent->serviceWaitRunner->aircraftID == id) && (runCurrent->serviceWaitRunner->location == spot) && (runCurrent->serviceWaitRunner->duration == -1))
		{
			runCurrent->serviceWaitRunner->timeEnd = end;
			runCurrent->serviceWaitRunner->duration = (runCurrent->serviceWaitRunner->timeEnd - runCurrent->serviceWaitRunner->timeStart);
		}

		runCurrent->serviceWaitRunner = runCurrent->serviceWaitRunner->next;

	} while (runCurrent->serviceWaitRunner != nullptr);
}

//Create a node containing an aircraft type, a repair job, and a start time
void Scribe::RecordRepairJob(string aircraft, int id, string job, float start)
{
	if (runCurrent->repairJobHead == nullptr)
	{
		runCurrent->repairJobHead = new repairJobNode(aircraft, id, job, start);
		runCurrent->repairJobTail = runCurrent->repairJobHead;
	}
	else
	{
		runCurrent->repairJobTail->next = new repairJobNode(aircraft, id, job, start);
		runCurrent->repairJobTail = runCurrent->repairJobTail->next;
	}
}

/*Search for an existing repair node with a specified aircraft id, repair job and duration of -1 to record
	the end time of the node and calculate duration
*/
void Scribe::RecordRepairEnd(int id, string job, float end)
{
	runCurrent->repairJobRunner = runCurrent->repairJobHead;
	do
	{
		if (runCurrent->repairJobRunner->aircraftID == id)
		{
			if (runCurrent->repairJobRunner->jobType == job)
			{
				if (runCurrent->repairJobRunner->duration == -1)
				{
					runCurrent->repairJobRunner->timeEnd = end;
					runCurrent->repairJobRunner->duration = (runCurrent->repairJobRunner->timeEnd - runCurrent->repairJobRunner->timeStart);
				}
			}
		}

		runCurrent->repairJobRunner = runCurrent->repairJobRunner->next;
	} while (runCurrent->repairJobRunner != nullptr);
}

//Create a node containing (an aircraft or resource type), a rework event and a duration
void Scribe::RecordRework(string object, string rework, float time)
{
	if (runCurrent->reworkHead == nullptr)
	{
		runCurrent->reworkHead = new reworkNode(object, rework, time);
		runCurrent->reworkTail = runCurrent->reworkHead;
	}
	else
	{
		runCurrent->reworkTail->next = new reworkNode(object, rework, time);
		runCurrent->reworkTail = runCurrent->reworkTail->next;
	}
}

//Search request nodes for a specific resource and update both number used and requests
void Scribe::RecordPartRequest(string resource, int usage, bool success)
{
	if (runCurrent->requestsHead == nullptr)
	{
		runCurrent->requestsHead = new partRequestNode(resource);
		runCurrent->requestsTail = runCurrent->requestsHead;

		runCurrent->requestsHead->numberUsed += usage;
		if (success)
		{
			runCurrent->requestsHead->requestNumber++;
		}
		else
		{
			runCurrent->requestsHead->unsuccessfulRequests++;
		}
	}
	else
	{
		bool exists = false;
		runCurrent->requestsRunner = runCurrent->requestsHead;
		do
		{
			if (runCurrent->requestsRunner->partType == resource)
			{
				exists = true;
				runCurrent->requestsRunner->numberUsed += usage;
				if (success)
				{
					runCurrent->requestsRunner->requestNumber++;
				}
				else
				{
					runCurrent->requestsRunner->unsuccessfulRequests++;
				}
			}

			runCurrent->requestsRunner = runCurrent->requestsRunner->next;

		} while (runCurrent->requestsRunner != nullptr);

		if (!exists)
		{
			runCurrent->requestsTail->next = new partRequestNode(resource);
			runCurrent->requestsTail = runCurrent->requestsTail->next;

			runCurrent->requestsTail->numberUsed += usage;
			if (success)
			{
				runCurrent->requestsTail->requestNumber++;
			}
			else
			{
				runCurrent->requestsTail->requestNumber++;
			}
		}
	}
}

//Create a node containing a resource and a duration
void Scribe::RecordRestock(string resource, float time)
{
	if (runCurrent->restockHead == nullptr)
	{
		runCurrent->restockHead = new restockNode(resource, time);
		runCurrent->restockTail = runCurrent->restockHead;
	}
	else
	{
		runCurrent->restockTail->next = new restockNode(resource, time);
		runCurrent->restockTail = runCurrent->restockTail->next;
	}
}

//Call to begin next run
void Scribe::AdvanceRun()
{
	//check for additional runs
	if (runCurrent->next != nullptr)
	{
		runCurrent = runCurrent->next;
	}
	// No further runs
	else
	{
		Archive();
	}
}

//call to set warehouse dimensions
//void Scribe::SetWarehousDims(string length, string width)
void Scribe::SetWarehousDims(double length, double width)
{
	warehouseL = to_string(length);
	warehouseW = to_string(width);
}

//call to set runtime at end of run
void Scribe::SetRunTime(float runT)
{
	runtime = runT;
	UpdateResourceUtilization();
	totalRuntime += runT;
}

//call to set the number of planned repair jobs
void Scribe::SetPlanned(int known)
{
	planned = known;
}

void Scribe::TallyUnplanned(int unplannedSet)
{
	unplanned += unplannedSet;
}

//Call to set the Seed Value for the warehouse setup and save file key
void Scribe::SetSeed(double value)
{
	seedVal = value;
}

//Call to set the name of the save file
void Scribe::SetSaveFile(string file)
{
	fileName = file;
}

//Record information in external file
void Scribe::Archive()
{
	//out file stream variable
	ofstream fileOut;
	//temporary string for formatting
	string tempStr = "";
	//integer to count end of list pointers
	int endCount;

	//Begin Archive
	fileOut.open(fileName);
	//Simulation data
		//Include Seed value for later analysis
	seedVal = Distribution::GetSystemSeed();
	fileOut << (to_string(seedVal) + "\n");

	fileOut << "\n";
	fileOut << "Number of Runs," + to_string(runNumber) + "\n";
	fileOut << "Warehouse Length," + warehouseL + "\n";
	fileOut << "Warehouse Width," + warehouseW + "\n";
	fileOut << "Runtime Duration," + to_string(totalRuntime) + "\n";
	fileOut << "Number of Planned Repair Jobs," + to_string(planned) + "\n";
	fileOut << "Number of Unplanned Repair Jobs," + to_string(unplanned) + "\n";

	fileOut << "\n";

	//Aircraft data for each run
	fileOut << "Aircraft\n";
	
		//Create run Headings
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,");
	}
	tempStr += "\n";
	fileOut << tempStr;
		
		//Create field Headings
	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Type,Number,");
	}
	tempStr += "\n";
	fileOut << tempStr;

		//Initialize aircraft search pointers
	runCurrent = runStart;
	for (int i = 0; i < runNumber; i++)
	{
		runCurrent->aircraftRunner = runCurrent->aircraftHead;
		runCurrent = runCurrent->next;
	}
	
	do
	{
		//re-initailize variables
		endCount = 0;
		runCurrent = runStart;
		tempStr = "";
		for (int i = 0; i < runNumber; i++)
		{
			//Check that pointer has data
			if (runCurrent->aircraftRunner == nullptr)
			{
				//record blank
				tempStr += ",,";
				endCount++;
			}
			else
			{
				//record both type and count
				tempStr += ((runCurrent->aircraftRunner->type) + "," + to_string(runCurrent->aircraftRunner->count) + ",");
				runCurrent->aircraftRunner = runCurrent->aircraftRunner->next;
			}
			//advance run
			runCurrent = runCurrent->next;
		}
		//end line
		tempStr += "\n";
		//Record Line
		fileOut << tempStr;
		//check that all lists have ended
	} while (endCount < runNumber);
		//Above should result in a blank line separating above from missions below

	//Mission data for each run
	fileOut << "Missions\n";
	tempStr = "";

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",");
	}
	tempStr += "\n";

	fileOut << tempStr;

		//Initialize Mission Search pointers
	runCurrent = runStart;
	for (int i = 0; i < runNumber; i++)
	{
		runCurrent->missionRunner = runCurrent->missionHead;
		runCurrent = runCurrent->next;
	}

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->missionRunner == nullptr)
			{
				tempStr += ",";
				endCount++;
			}
			else
			{
				tempStr += (runCurrent->missionRunner->type + ",");
				runCurrent->missionRunner = runCurrent->missionRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;
	} while (endCount < runNumber);

	//Resource data for each run
	fileOut << "Resources\n";
	tempStr = "";
	runCurrent = runStart;

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,,,,,");
		runCurrent->resourceRunner = runCurrent->resourceHead;
		runCurrent = runCurrent->next;
	}
	tempStr += "\n";
	fileOut << tempStr;

	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Resource,Initail Count,Utilization Hours,Utilization Percent,Number of Requests,Unsuccessful Requests,");
	}
	tempStr += "\n";
	fileOut << tempStr;

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;

		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->resourceRunner == nullptr)
			{
				tempStr += ",,,,,,";
				endCount++;
			}
			else
			{
				tempStr += ((runCurrent->resourceRunner->type) + "," + to_string(runCurrent->resourceRunner->initialCount) + "," + to_string(runCurrent->resourceRunner->utilizationHours) + "," + to_string(runCurrent->resourceRunner->utilizationPercent) +
					"," + to_string(runCurrent->resourceRunner->requestNumber) + "," + to_string(runCurrent->resourceRunner->unsuccessfulRequests) + ",");
				runCurrent->resourceRunner = runCurrent->resourceRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

	} while (endCount < runNumber);

	//Failure data for each run
	fileOut << "Resource Failure\n";
	tempStr = "";
	runCurrent = runStart;

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,,");
		runCurrent->failureRunner = runCurrent->failureHead;
		runCurrent = runCurrent->next;
	}
	tempStr += "\n";
	fileOut << tempStr;

	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Resource,Failure,Downtime,");
	}
	tempStr += "\n";
	fileOut << tempStr;

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;

		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->failureRunner == nullptr)
			{
				tempStr += ",,,";
				endCount++;
			}
			else
			{
				tempStr += ((runCurrent->failureRunner->resourceType) + "," + (runCurrent->failureRunner->failureType) + "," + to_string(runCurrent->failureRunner->duration)+",");
				runCurrent->failureRunner = runCurrent->failureRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

	} while (endCount < runNumber);

	//Resource wait data for each run
	fileOut << "Resource Waits\n";
	tempStr = "";
	runCurrent = runStart;

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,,,,,");
		runCurrent->resourceWaitRunner = runCurrent->resourceWaitHead;
		runCurrent = runCurrent->next;
	}
	tempStr += "\n";
	fileOut << tempStr;

	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Aircraft Type,ID,Resource,Start,End,Time,");
	}
	tempStr += "\n";
	fileOut << tempStr;

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;

		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->resourceWaitRunner == nullptr)
			{
				tempStr += (",,,,,,");
				endCount++;
			}
			else
			{
				tempStr += ((runCurrent->resourceWaitRunner->aircraftType) + "," + to_string(runCurrent->resourceWaitRunner->aircraftID) + "," + (runCurrent->resourceWaitRunner->resourceType) + "," + to_string(runCurrent->resourceWaitRunner->timeStart) +
					"," + to_string(runCurrent->resourceWaitRunner->timeEnd) + "," + to_string(runCurrent->resourceWaitRunner->duration) + ",");
				runCurrent->resourceWaitRunner = runCurrent->resourceWaitRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

	} while (endCount < runNumber);

	//Service wait data for each run
	fileOut << "Aircraft Wait\n";
	tempStr = "";
	runCurrent = runStart;

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,,,,,");
		runCurrent->serviceWaitRunner = runCurrent->serviceWaitHead;
		runCurrent = runCurrent->next;
	}
	tempStr += "\n";
	fileOut << tempStr;

	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("ID,Type,Location,Time Begin,Time End,Wait Time,");
	}
	tempStr += "\n";
	fileOut << tempStr;

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;

		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->serviceWaitRunner == nullptr)
			{
				tempStr += (",,,,,,");
				endCount++;
			}
			else
			{
				tempStr += (to_string(runCurrent->serviceWaitRunner->aircraftID) + "," + (runCurrent->serviceWaitRunner->aircraftType) + "," + (runCurrent->serviceWaitRunner->location) + "," +
					to_string(runCurrent->serviceWaitRunner->timeStart) + "," + to_string(runCurrent->serviceWaitRunner->timeEnd) + "," + to_string(runCurrent->serviceWaitRunner->duration) + ",");
				runCurrent->serviceWaitRunner = runCurrent->serviceWaitRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

	} while (endCount < runNumber);

	//Repair job data for each run
	fileOut << "Repair Jobs\n";
	tempStr = "";
	runCurrent = runStart;

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,,,,,");
		runCurrent->repairJobRunner = runCurrent->repairJobHead;
		runCurrent = runCurrent->next;
	}
	tempStr += "\n";
	fileOut << tempStr;

	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("ID,Type,Job,Start,Finish,Duration,");
	}
	tempStr += "\n";
	fileOut << tempStr;

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;

		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->repairJobRunner == nullptr)
			{
				tempStr += (",,,,,,");
				endCount++;
			}
			else
			{
				tempStr += (to_string(runCurrent->repairJobRunner->aircraftID) + "," + (runCurrent->repairJobRunner->aircraftType) + "," + (runCurrent->repairJobRunner->jobType) + "," +
					to_string(runCurrent->repairJobRunner->timeStart) + "," + to_string(runCurrent->repairJobRunner->timeEnd) + "," + to_string(runCurrent->repairJobRunner->duration) + ",");
				runCurrent->repairJobRunner = runCurrent->repairJobRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

	} while (endCount < runNumber);

	//Rework data for each run
	fileOut << "Reworks\n";
	tempStr = "";
	runCurrent = runStart;

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,,");
		runCurrent->reworkRunner = runCurrent->reworkHead;
		runCurrent = runCurrent->next;
	}
	tempStr += "\n";
	fileOut << tempStr;

	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Aircraft or Resource Type,Rework Event,Duration,");
	}
	tempStr += "\n";
	fileOut << tempStr;

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;

		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->reworkRunner == nullptr)
			{
				tempStr += ",,,";
				endCount++;
			}
			else
			{
				tempStr += ((runCurrent->reworkRunner->objectType) + "," + (runCurrent->reworkRunner->reworkEvent) + "," + to_string(runCurrent->reworkRunner->duration) + ",");
				runCurrent->reworkRunner = runCurrent->reworkRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

	} while (endCount < runNumber);

	//Part Request data for each run
	fileOut << "Parts Requests\n";
	tempStr = "";
	runCurrent = runStart;

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,,,");
		runCurrent->requestsRunner = runCurrent->requestsHead;
		runCurrent = runCurrent->next;
	}
	tempStr += "\n";
	fileOut << tempStr;

	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Part,Number Used,Times Requested,Unsuccessful Requests,");
	}
	tempStr += "\n";
	fileOut << tempStr;

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;

		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->requestsRunner == nullptr)
			{
				tempStr += (",,,,");
				endCount++;
			}
			else
			{
				tempStr += ((runCurrent->requestsRunner->partType) + "," + to_string(runCurrent->requestsRunner->numberUsed) + "," + to_string(runCurrent->requestsRunner->requestNumber) + ","
					+ to_string(runCurrent->requestsRunner->unsuccessfulRequests) + ",");
				runCurrent->requestsRunner = runCurrent->requestsRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

	} while (endCount < runNumber);

	//Restock data for each run
	fileOut << "Restocking\n";
	tempStr = "";
	runCurrent = runStart;

	for (int i = 0; i < runNumber; i++)
	{
		tempStr += ("Run " + to_string(i + 1) + ",,");
		runCurrent->restockRunner = runCurrent->restockHead;
		runCurrent = runCurrent->next;
	}
	tempStr += "\n";
	fileOut << tempStr;

	tempStr = "";
	for (int i = 0; i < runNumber; i++)
	{
		tempStr += "Part,TUA,";
	}
	tempStr += "\n";
	fileOut << tempStr;

	do
	{
		endCount = 0;
		tempStr = "";
		runCurrent = runStart;

		for (int i = 0; i < runNumber; i++)
		{
			if (runCurrent->restockRunner == nullptr)
			{
				tempStr += (",,");
				endCount++;
			}
			else
			{
				tempStr += ((runCurrent->restockRunner->partType) + "," + to_string(runCurrent->restockRunner->restockTime) + ",");
				runCurrent->restockRunner = runCurrent->restockRunner->next;
			}
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

	} while (endCount < runNumber);

	cout << "Archived to: " + fileName;
	system("pause");
}

runNode* Scribe::GetStart()
{
	return runStart;
}

