//Scribe.cpp: Kevin Voyt

#include "Scribe.h"
#include "Distribution.h"
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>



//Scribe static initializers
runNode* Scribe::runStart = nullptr;

runNode* Scribe::runCurrent = nullptr;

runNode* Scribe::runEnd = nullptr;

string Scribe::warehouseL = "0";

string Scribe::warehouseW = "0";

string Scribe::fileName = "nullfile";

float Scribe::runtime = 0;

float Scribe::totalRuntime = 0;

int Scribe::planned = 0;

int Scribe::unplanned = 0;

int Scribe::runNumber = 0;

double Scribe::seedVal = 0;

int Scribe::_numRuns;

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
	ellapse = 0;
	date = "";

	next = nullptr;
}

failureNode::failureNode(string resource, string failure, float time)
{
	resourceType = resource;
	failureType = failure;
	tStart = SimExec::GetTotalSimulationTime();
	ellapse = -1;
	date = (to_string(SimExec::GetSimulationTime()._month + 1) + "/" + to_string(SimExec::GetSimulationTime()._day + 1) + "/" + to_string(SimExec::GetSimulationTime()._year));

	next = nullptr;
}

failureNode::failureNode(const failureNode& node2)
{
	resourceType = node2.resourceType;
	failureType = node2.failureType;
	ellapse = node2.ellapse;
	date = node2.date;
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
	dayStart = 0;
	monthStart = 0;
	yearStart = 0;
	timeEnd = 0;
	dayEnd = 0;
	monthEnd = 0;
	yearEnd = 0;
	ellapse = -1;

	next = nullptr;
}

resourceWaitNode::resourceWaitNode(string aircraft, int ID, string resource, float time)
{
	aircraftType = aircraft;
	aircraftID = ID;
	resourceType = resource;
	timeStart = time;
	dayStart = SimExec::GetSimulationTime()._day + 1;
	monthStart = SimExec::GetSimulationTime()._month + 1;
	yearStart = SimExec::GetSimulationTime()._year;
	timeEnd = 0;
	dayEnd = 0;
	monthEnd = 0;
	yearEnd = 0;
	ellapse = -1;

	next = nullptr;
}

resourceWaitNode::resourceWaitNode(const resourceWaitNode& node2)
{
	aircraftType = node2.aircraftType;
	aircraftID = node2.aircraftID;
	resourceType = node2.resourceType;
	timeStart = node2.timeStart;
	dayStart = node2.dayStart;
	monthStart = node2.monthStart;
	yearStart = node2.yearStart;
	timeEnd = node2.timeEnd;
	dayEnd = node2.dayEnd;
	monthEnd = node2.monthEnd;
	yearEnd = node2.yearEnd;
	ellapse = node2.ellapse;
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
	dayStart = 0;
	monthStart = 0;
	yearStart = 0;
	timeEnd = 0;
	dayEnd = 0;
	monthEnd = 0;
	yearEnd = 0;
	ellapse = -1;

	next = nullptr;
}

serviceWaitNode::serviceWaitNode(string aircraft, int ID, string spot, float time)
{
	aircraftType = aircraft;
	aircraftID = ID;
	location = spot;
	timeStart = time;
	dayStart = SimExec::GetSimulationTime()._day + 1;
	monthStart = SimExec::GetSimulationTime()._month + 1;
	yearStart = SimExec::GetSimulationTime()._year;

	timeEnd = 0;
	dayEnd = 0;
	monthEnd = 0;
	yearEnd = 0;
	ellapse = -1;

	next = nullptr;
}

serviceWaitNode::serviceWaitNode(const serviceWaitNode& node2)
{
	aircraftType = node2.aircraftType;
	aircraftID = node2.aircraftID;
	location = node2.location;
	timeStart = node2.timeStart;
	dayStart = node2.dayStart;
	monthStart = node2.monthStart;
	yearStart = node2.yearStart;

	timeEnd = node2.timeEnd;
	dayEnd = node2.dayEnd;
	monthEnd = node2.monthEnd;
	yearEnd = node2.yearEnd;

	ellapse = node2.ellapse;
	next = node2.next;
}

serviceWaitNode::~serviceWaitNode()
{
	delete this;
}

repairJobNode::repairJobNode()
{
	aircraftType = "";
	aircraftID = 0;
	jobType = "";
	stepNumber = 0;
	stepName = "";
	timeStart = 0;
	dayStart = 0;
	monthStart = 0;
	yearStart = 0;
	timeEnd = 0;
	dayEnd = 0;
	monthEnd = 0;
	yearEnd = 0;
	ellapse = -1;

	next = nullptr;
}

repairJobNode::repairJobNode(string aircraft, int id, string job, int stepNum, string step, float time)
{
	aircraftType = aircraft;
	aircraftID = id;
	jobType = job;
	stepNumber = stepNum;
	stepName = step;
	timeStart = time;
	dayStart = SimExec::GetSimulationTime()._day + 1;
	monthStart = SimExec::GetSimulationTime()._month + 1;
	yearStart = SimExec::GetSimulationTime()._year;
	timeEnd = 0;
	dayEnd = 0;
	monthEnd = 0;
	yearEnd = 0;
	ellapse = -1;

	next = nullptr;
}

repairJobNode::repairJobNode(const repairJobNode& node2)
{
	aircraftType = node2.aircraftType;
	aircraftID = node2.aircraftID;
	jobType = node2.jobType;
	stepNumber = node2.stepNumber;
	stepName = node2.stepName;
	timeStart = node2.timeStart;
	dayStart = node2.dayStart;
	monthStart = node2.monthStart;
	yearStart = node2.yearStart;
	timeEnd = node2.timeEnd;
	dayEnd = node2.dayEnd;
	monthEnd = node2.monthEnd;
	yearEnd = node2.yearEnd;
	ellapse = node2.ellapse;
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
	ellapse = 0;

	next = nullptr;
}

reworkNode::reworkNode(string object, string rework, float time)
{
	objectType = object;
	reworkEvent = rework;
	ellapse = time;
	date = (to_string(SimExec::GetSimulationTime()._month + 1) + "/" + to_string(SimExec::GetSimulationTime()._day + 1) + "/" + to_string(SimExec::GetSimulationTime()._year));

	next = nullptr;
}

reworkNode::reworkNode(const reworkNode& node2)
{
	objectType = node2.objectType;
	reworkEvent = node2.reworkEvent;
	ellapse = node2.ellapse;
	date = node2.date;
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
	date = "";

	next = nullptr;
}

restockNode::restockNode(string part, float time)
{
	partType = part;
	restockTime = time;
	date = (to_string(SimExec::GetSimulationTime()._month + 1) + "/" + to_string(SimExec::GetSimulationTime()._day + 1) + "/" + to_string(SimExec::GetSimulationTime()._year));

	next = nullptr;
}

restockNode::restockNode(const restockNode& node2)
{
	partType = node2.partType;
	restockTime = node2.restockTime;
	date = node2.date;
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

	inspectionHead = nullptr;
	inspectionRunner = nullptr;
	inspectionTail = nullptr;

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
	inspectionHead = node2.inspectionHead;

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
	inspectionTail = node2.inspectionTail;

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
	inspectionRunner = nullptr;

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

	SetWarehousDims(0.0, 0.0);
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
	SetWarehousDims(0.0, 0.0);
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

void Scribe::SetNumRuns(int numRuns)
{
	_numRuns = numRuns;
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
		} while (runCurrent->aircraftRunner != nullptr && exists != true);

		//exists being false means no match found
		if (!exists)
		{
			runCurrent->aircraftTail->next = new aircraftNode(aircraft);
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
		/*else
		{*/
		runCurrent->aircraftRunner = runCurrent->aircraftRunner->next;
		//}
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
		} while (runCurrent->missionRunner != nullptr && (exists != true));


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
//void Scribe::UpdateResourceUtilization(string resource, int usage, float currentTime)
void Scribe::UpdateResourceUtilization(string resource, double usage, float currentTime)
{

	runCurrent->resourceRunner = runCurrent->resourceHead;
	do
	{
		if (runCurrent->resourceRunner->type == resource)
		{
			runCurrent->resourceRunner->utilizationHours += (runCurrent->resourceRunner->initialCount - runCurrent->resourceRunner->availability) * (SimExec::GetTotalSimulationTime() - runCurrent->resourceRunner->lastchange);
			runCurrent->resourceRunner->availability -= usage;
			runCurrent->resourceRunner->lastchange = SimExec::GetTotalSimulationTime();
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
		runCurrent->resourceRunner = runCurrent->resourceRunner->next;
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

		runCurrent->resourceRunner = runCurrent->resourceRunner->next;

	} while (runCurrent->resourceRunner != nullptr);
}

//Create a failure node containing an resource type, failure type, and a failure duration
void Scribe::RecordFailure(string resource, string failure, float time)
{
	if (runCurrent->failureHead == nullptr)
	{
		runCurrent->failureHead = new failureNode(resource, failure, time);
		runCurrent->failureTail = runCurrent->failureHead;
	}
	else
	{
		runCurrent->failureTail->next = new failureNode(resource, failure, time);
		runCurrent->failureTail = runCurrent->failureTail->next;
	}
}

void Scribe::RecordRestore(string resource, string failure, float time)
{
	runCurrent->failureRunner = runCurrent->failureHead;
	do
	{
		if (runCurrent->failureRunner->resourceType == resource)
		{
			if (runCurrent->failureRunner->failureType == failure)
			{
				if (runCurrent->failureRunner->ellapse == -1)
				{
					runCurrent->failureRunner->tEnd = SimExec::GetTotalSimulationTime();
					runCurrent->failureRunner->ellapse = runCurrent->failureRunner->tEnd - runCurrent->failureRunner->tStart;
				}
			}
		}

		runCurrent->failureRunner = runCurrent->failureRunner->next;
	} while (runCurrent->failureRunner != nullptr);
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
				if (runCurrent->resourceWaitRunner->ellapse == -1)
				{
					runCurrent->resourceWaitRunner->timeEnd = end;
					runCurrent->resourceWaitRunner->dayEnd = SimExec::GetSimulationTime()._day + 1;
					runCurrent->resourceWaitRunner->monthEnd = SimExec::GetSimulationTime()._month + 1;
					runCurrent->resourceWaitRunner->yearEnd = SimExec::GetSimulationTime()._year;

					//Convert Start Date into days
						//Year indicates full years with every fourth year being a leap year with one extra day
					long double startDate = int((runCurrent->resourceWaitRunner->yearStart) * 365.25);

					//Add full months
					for (int i = 1; i < (runCurrent->resourceWaitRunner->monthStart); i++)
					{
						if (i == 4 || i == 6 || i == 9 || i == 11)
						{
							startDate += 30;
						}
						else if (i == 2)
						{
							if ((int(runCurrent->resourceWaitRunner->yearStart) % 4) == 0)
							{
								startDate += 29;
							}
							else
							{
								startDate += 28;
							}
						}
						else
						{
							startDate += 30;
						}
					}
					//Add days in current month
					startDate += (runCurrent->resourceWaitRunner->dayStart);

					//Repeat process for End Date
					long double endDate = int((runCurrent->resourceWaitRunner->yearEnd) * 365.25);
					for (int i = 1; i < (runCurrent->resourceWaitRunner->monthEnd); i++)
					{
						if (i == 4 || i == 6 || i == 9 || i == 11)
						{
							endDate += 30;
						}
						else if (i == 2)
						{
							if ((int(runCurrent->resourceWaitRunner->yearEnd) % 4) == 0)
							{
								endDate += 29;
							}
							else
							{
								endDate += 28;
							}
						}
						else
						{
							endDate += 30;
						}
					}
					endDate += (runCurrent->resourceWaitRunner->dayEnd);

					//Convert start and end to hours - current day is not complete
					long double startTime = (startDate * 24) + runCurrent->resourceWaitRunner->timeStart;
					long double endTime = (endDate * 24) + runCurrent->resourceWaitRunner->timeEnd;

					runCurrent->resourceWaitRunner->ellapse = endTime - startTime;
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
		if ((runCurrent->serviceWaitRunner->aircraftID == id) && (runCurrent->serviceWaitRunner->location == spot) && (runCurrent->serviceWaitRunner->ellapse == -1))
		{
			runCurrent->serviceWaitRunner->timeEnd = end;
			runCurrent->serviceWaitRunner->dayEnd = SimExec::GetSimulationTime()._day + 1;
			runCurrent->serviceWaitRunner->monthEnd = SimExec::GetSimulationTime()._month + 1;
			runCurrent->serviceWaitRunner->yearEnd = SimExec::GetSimulationTime()._year;
			long double startDate = int((runCurrent->serviceWaitRunner->yearStart) * 365.25);
			for (int i = 1; i < (runCurrent->serviceWaitRunner->monthStart); i++)
			{
				if (i == 4 || i == 6 || i == 9 || i == 11)
				{
					startDate += 30;
				}
				else if (i == 2)
				{
					if ((int(runCurrent->serviceWaitRunner->yearStart) % 4) == 0)
					{
						startDate += 29;
					}
					else
					{
						startDate += 28;
					}
				}
				else
				{
					startDate += 30;
				}
			}

			startDate += (runCurrent->serviceWaitRunner->dayStart);

			long double endDate = int((runCurrent->serviceWaitRunner->yearEnd) * 365.25);
			for (int i = 1; i < (runCurrent->serviceWaitRunner->monthEnd); i++)
			{
				if (i == 4 || i == 6 || i == 9 || i == 11)
				{
					endDate += 30;
				}
				else if (i == 2)
				{
					if ((int(runCurrent->serviceWaitRunner->yearEnd) % 4) == 0)
					{
						endDate += 29;
					}
					else
					{
						endDate += 28;
					}
				}
				else
				{
					endDate += 30;
				}
			}
			endDate += (runCurrent->serviceWaitRunner->dayEnd);

			long double startTime = (startDate * 24) + runCurrent->serviceWaitRunner->timeStart;
			long double endTime = (endDate * 24) + runCurrent->serviceWaitRunner->timeEnd;
			runCurrent->serviceWaitRunner->ellapse = (endTime - startTime);
		}

		runCurrent->serviceWaitRunner = runCurrent->serviceWaitRunner->next;

	} while (runCurrent->serviceWaitRunner != nullptr);
}

//Create a node containing an aircraft type, a repair job, and a start time
void Scribe::RecordRepairJob(string aircraft, int id, string job, int stepNum, string stepNme, float start)
{
	if (runCurrent->repairJobHead == nullptr)
	{
		runCurrent->repairJobHead = new repairJobNode(aircraft, id, job, stepNum, stepNme, start);
		runCurrent->repairJobTail = runCurrent->repairJobHead;
	}
	else
	{
		runCurrent->repairJobTail->next = new repairJobNode(aircraft, id, job, stepNum, stepNme, start);
		runCurrent->repairJobTail = runCurrent->repairJobTail->next;
	}
}

/*Search for an existing repair node with a specified aircraft id, repair job and duration of -1 to record
	the end time of the node and calculate duration
*/
void Scribe::RecordRepairEnd(int id, string job, int step, float end)
{
	runCurrent->repairJobRunner = runCurrent->repairJobHead;
	do
	{
		if (runCurrent->repairJobRunner->aircraftID == id)
		{
			if (runCurrent->repairJobRunner->jobType == job)
			{
				if (runCurrent->repairJobRunner->stepNumber == step)
				{
					if (runCurrent->repairJobRunner->ellapse == -1)
					{
						runCurrent->repairJobRunner->timeEnd = end;
						runCurrent->repairJobRunner->dayEnd = SimExec::GetSimulationTime()._day + 1;
						runCurrent->repairJobRunner->monthEnd = SimExec::GetSimulationTime()._month + 1;
						runCurrent->repairJobRunner->yearEnd = SimExec::GetSimulationTime()._year;
						long double startDate = int((runCurrent->repairJobRunner->yearStart) * 365.25);
						for (int i = 1; i < (runCurrent->repairJobRunner->monthStart); i++)
						{
							if (i == 4 || i == 6 || i == 9 || i == 11)
							{
								startDate += 30;
							}
							else if (i == 2)
							{
								if ((int(runCurrent->repairJobRunner->yearStart) % 4) == 0)
								{
									startDate += 29;
								}
								else
								{
									startDate += 28;
								}
							}
							else
							{
								startDate += 30;
							}
						}

						startDate += (runCurrent->repairJobRunner->dayStart);

						long double endDate = int((runCurrent->repairJobRunner->yearEnd) * 365.25);
						for (int i = 1; i < (runCurrent->repairJobRunner->monthEnd); i++)
						{
							if (i == 4 || i == 6 || i == 9 || i == 11)
							{
								endDate += 30;
							}
							else if (i == 2)
							{
								if ((int(runCurrent->repairJobRunner->yearEnd) % 4) == 0)
								{
									endDate += 29;
								}
								else
								{
									endDate += 28;
								}
							}
							else
							{
								endDate += 30;
							}
						}
						endDate += (runCurrent->repairJobRunner->dayEnd);

						long double startTime = (startDate * 24) + runCurrent->repairJobRunner->timeStart;
						long double endTime = (endDate * 24) + runCurrent->repairJobRunner->timeEnd;

						runCurrent->repairJobRunner->ellapse = (endTime - startTime);
					}
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

void Scribe::RecordInspectionFailure(int airID, string airType, string inspect, int stepNum)
{
	if (runCurrent->inspectionHead == nullptr)
	{
		runCurrent->inspectionHead = new InspectionFailureNode(airID, airType, inspect, stepNum);
		runCurrent->inspectionTail = runCurrent->inspectionHead;
	}
	else
	{
		runCurrent->inspectionTail->next = new InspectionFailureNode(airID, airType, inspect, stepNum);
		runCurrent->inspectionTail = runCurrent->inspectionTail->next;
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

/*Call to change the number of runs in the simulation
  This is meant as an alternative to Scribe(int runs)
*/
void Scribe::SetRuns(int runs)
{
	//Logic checks
	if (runs < 0)
	{
		cout << "Cannot record negative runs.  Setting back to previous value.";
		runs = runNumber;
	}
	else if (runs == 0)
	{
		cout << "Please use a minimum of onr run. Setting back to previous value.";
		runs = runNumber;
	}
	//Expand run list
	if (runs > runNumber)
	{
		//Set i to the current run number and expand list until i equals new runs
		for (int i = runNumber; i <= runs; i++)
		{
			runEnd->next = new runNode();
			runEnd = runEnd->next;
		}

	}
	//Shrink run list
	else if (runs < runNumber)
	{
		runNode* temp;
		runNode* temp2;
		runEnd = runStart;
		//starting from beginning, adavnce through runs until run end reaches the runs run
		for (int i = 1; i < runs; i++)
		{
			runEnd = runEnd->next;
		}

		temp2 = runEnd->next;

		while (runCurrent != nullptr)
		{
			temp = temp2;
			temp2 = temp2->next;
			delete temp;
		}
	}
}

//Increment number of unplanned Repair Jobs by a given integer value
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
	//sql::Connection *conn;
	//temporary string for formatting
	string tempStr = "";
	//integer to count end of list pointers
	int endCount;

	/*//////////////////////////////////
	////// CONNECTING TO DATABASE //////
	////////////////////////////////////
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
//define handles and variables
	SQLHANDLE sqlConnHandle;
	SQLHANDLE sqlStmtHandle;
	SQLHANDLE sqlEnvHandle;
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
	//initializations
	sqlConnHandle = NULL;
	sqlStmtHandle = NULL;
	//allocations
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
		goto COMPLETED;
	if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		goto COMPLETED;
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
		goto COMPLETED;
	//output
	cout << "Attempting connection to SQL Server...";
	cout << "\n";
	//connect to SQL Server
	////Using a trusted connection and port 14808
	////it does not matter if you are using default or named instance
	////just make sure you define the server name and the port
	////We have the option to use a username/password instead of a trusted connection
	//but its more secure to use a trusted connection
	switch (SQLDriverConnect(sqlConnHandle,
		NULL,
		//(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=master;UID=username;PWD=password;",
		//********* Need to set server and database names -->automate this in GUI (if there is one)
		(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=govasim-2;DATABASE=SWASTestDatabase;Trusted=true;",
		SQL_NTS,
		retconstring,
		1024,
		NULL,
		SQL_DRIVER_NOPROMPT)) {
	case SQL_SUCCESS:
		cout << "Successfully connected to SQL Server";
		cout << "\n";
		break;
	case SQL_SUCCESS_WITH_INFO:
		cout << "Successfully connected to SQL Server";
		cout << "\n";
		break;
	case SQL_INVALID_HANDLE:
		cout << "Could not connect to SQL Server";
		cout << "\n";
		goto COMPLETED;
	case SQL_ERROR:
		cout << "Could not connect to SQL Server";
		cout << "\n";
		goto COMPLETED;
	default:
		break;
	}
	//if there is a problem connecting then exit application
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
		goto COMPLETED;
	//output
	cout << "\n";
	cout << "Executing T-SQL query...";
	cout << "\n";
	//if there is a problem executing the query then exit application
	//else display query result
	//////////////////////////////////////////////////////
	//THIS IS AN EXAMPE OF THE CODE (REPLACE SELECT STATEMENT WITH INSERT STATEMENT)
	//if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT name FROM artists", SQL_NTS)) {   ///<<<<<<<<< THIS is an example of SQL Query code that finds the values from aircraft name and prints.
	//	cout << "Error querying SQL Server";
	//	cout << "\n";
	//	goto COMPLETED;
	//}
	//else {
	//	//declare output variable and pointer
	//	SQLCHAR sqlValue[SQL_RESULT_LEN];
	//	SQLINTEGER ptrSqlValue;
	//	while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
	//		SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
	//		//display query result
	//		cout << "\nQuery Result:\n\n";
	//		cout << sqlValue << endl;
	//	}
	//}
	//////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////*/

	//Begin Archive
	fileOut.open(fileName);
	//Simulation data
		//Include Seed value for later analysis
	//for (int i = 0; i < _numRuns; i++) {
		seedVal = Distribution::GetSystemSeed();
		fileOut << (to_string(seedVal) + "\n");
		fileOut << "\n";
		fileOut << "Number of Runs," + to_string(_numRuns) + "\n";
		fileOut << "Warehouse Length," + warehouseL + "\n";
		fileOut << "Warehouse Width," + warehouseW + "\n";
		fileOut << "Runtime Duration," + to_string(totalRuntime) + "\n";
		fileOut << "Number of Planned Repair Jobs," + to_string(planned) + "\n";
		fileOut << "Number of Unplanned Repair Jobs," + to_string(unplanned) + "\n";
		fileOut << "\n";

		/*if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Simulation Info (Seed, Number of Runs, Warehouse Length, Warehouse Width, Runtime Duration, Number of Planned Repair Jobs, Number of Unplanned Repair Jobs) VALUES ( .('" + to_string(seedVal) + "')., '" + to_string(runNumber) + "')., '" + warehouseL + "')., '" + warehouseW + "')., '" + to_string(totalRuntime) + "')., '" + to_string(planned) + "')., '" + to_string(unplanned) + "')").c_str(), SQL_NTS)) {
			cout << "Error querying SQL Server";
			cout << "\n";
			goto COMPLETED;
		}
		else {
			//declare output variable and pointer
			SQLCHAR sqlValue[SQL_RESULT_LEN];
			SQLINTEGER ptrSqlValue;
			while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
				//display query result
				cout << "\nQuery Result:\n\n";
				cout << sqlValue << endl;
			}
		}*/

		//Aircraft data for each run
		fileOut << "Aircraft\n";

		//Create run Headings
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		//Create field Headings
		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Type,Number,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		//Initialize aircraft search pointers
		runCurrent = runStart;
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
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
			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
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
		//} while (endCount < runNumber);
		} while (endCount < _numRuns);
		//Above should result in a blank line separating above from missions below

		//Export to SQL database
		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->aircraftRunner = runCurrent->aircraftHead;
			while (runCurrent->aircraftRunner != nullptr)
			{
				//Example Line
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Aircraft (Type, Number) VALUES ( .('" + (runCurrent->aircraftRunner->type) + "')., '" + to_string(runCurrent->aircraftRunner->count) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->aircraftRunner = runCurrent->aircraftRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/

		//Mission data for each run
		fileOut << "Missions\n";
		tempStr = "";

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",");
		}
		tempStr += "\n";

		fileOut << tempStr;

		//Initialize Mission Search pointers
		runCurrent = runStart;
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			runCurrent->missionRunner = runCurrent->missionHead;
			runCurrent = runCurrent->next;
		}

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;
			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
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
		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->missionRunner = runCurrent->missionHead;
			while (runCurrent->missionRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Missions (Type) VALUES ( .('" + runCurrent->missionRunner->type + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->missionRunner = runCurrent->missionRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/

		//Resource data for each run
		fileOut << "Resources\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,,,,");
			runCurrent->resourceRunner = runCurrent->resourceHead;
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Resource, Initial Count, Utilization Hours, Utilization Percent, Number of Requests, Unsuccessful Requests,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
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

		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->resourceRunner = runCurrent->resourceHead;
			while (runCurrent->resourceRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Resources (Resource, Initial Count, Utilization Hours, Utilization Percent, Number of Requests, Unsuccessful Requests) VALUES ( .('" + (runCurrent->resourceRunner->type) + "')., '" + to_string(runCurrent->resourceRunner->initialCount) + "')., '" + to_string(runCurrent->resourceRunner->utilizationHours) + "')., '" + to_string(runCurrent->resourceRunner->utilizationPercent) + "')., '" + to_string(runCurrent->resourceRunner->requestNumber) + "')., '" + to_string(runCurrent->resourceRunner->unsuccessfulRequests) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->resourceRunner = runCurrent->resourceRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/

		//Failure data for each run
		fileOut << "Resource Failure\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,,");
			runCurrent->failureRunner = runCurrent->failureHead;
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Resource,Failure,Date,Downtime,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
			{
				if (runCurrent->failureRunner == nullptr)
				{
					tempStr += ",,,,";
					endCount++;
				}
				else
				{
					tempStr += ((runCurrent->failureRunner->resourceType) + "," + (runCurrent->failureRunner->failureType) + "," + (runCurrent->failureRunner->date) + "," + to_string(runCurrent->failureRunner->ellapse) + ",");


					runCurrent->failureRunner = runCurrent->failureRunner->next;
				}
				runCurrent = runCurrent->next;
			}
			tempStr += "\n";
			fileOut << tempStr;

		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->failureRunner = runCurrent->failureHead;
			while (runCurrent->failureRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Resource Failure (Resource, Failure, Date, Downtime) VALUES ( .('" + (runCurrent->failureRunner->resourceType) + "')., '" + (runCurrent->failureRunner->failureType) + "')., '" + (runCurrent->failureRunner->date) + "')., '" + to_string(runCurrent->failureRunner->ellapse) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->failureRunner = runCurrent->failureRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/

		//Resource wait data for each run
		fileOut << "Resource Waits\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,,,,,,");
			runCurrent->resourceWaitRunner = runCurrent->resourceWaitHead;
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Aircraft Type,ID,Resource,Date Start,Start,Date End,End,Time,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
			{
				if (runCurrent->resourceWaitRunner == nullptr)
				{
					tempStr += (",,,,,,,,");
					endCount++;
				}
				else
				{
					tempStr += ((runCurrent->resourceWaitRunner->aircraftType) + "," + to_string(runCurrent->resourceWaitRunner->aircraftID) + "," + (runCurrent->resourceWaitRunner->resourceType) + "," +
						to_string(runCurrent->resourceWaitRunner->monthStart) + "/" + to_string(runCurrent->resourceWaitRunner->dayStart) + "/" + to_string(runCurrent->resourceWaitRunner->yearStart) + "," + to_string(runCurrent->resourceWaitRunner->timeStart) + "," +
						to_string(runCurrent->resourceWaitRunner->monthEnd) + "/" + to_string(runCurrent->resourceWaitRunner->dayEnd) + "/" + to_string(runCurrent->resourceWaitRunner->yearEnd) + "," + to_string(runCurrent->resourceWaitRunner->timeEnd) + "," +
						to_string(runCurrent->resourceWaitRunner->ellapse) + ",");


					runCurrent->resourceWaitRunner = runCurrent->resourceWaitRunner->next;
				}
				runCurrent = runCurrent->next;
			}
			tempStr += "\n";
			fileOut << tempStr;

		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->resourceWaitRunner = runCurrent->resourceWaitHead;
			while (runCurrent->resourceWaitRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Resource Waits (Aircraft Type, ID, Resource, Month Start, Day Start, Year Start, Start, Month End, Day End, Year End, End, Time) VALUES ( .('" + (runCurrent->resourceWaitRunner->aircraftType) + "')., '" + to_string(runCurrent->resourceWaitRunner->aircraftID) + "')., '" + (runCurrent->resourceWaitRunner->resourceType) + "')., '" + to_string(runCurrent->resourceWaitRunner->monthStart) + "')., '" + to_string(runCurrent->resourceWaitRunner->dayStart) + "')., '" + to_string(runCurrent->resourceWaitRunner->yearStart) + "')., '" + to_string(runCurrent->resourceWaitRunner->timeStart) + "')., '" + to_string(runCurrent->resourceWaitRunner->monthEnd) + "')., '" + to_string(runCurrent->resourceWaitRunner->dayEnd) + "')., '" + to_string(runCurrent->resourceWaitRunner->yearEnd) + "')., '" + to_string(runCurrent->resourceWaitRunner->timeEnd) + "')., '" + to_string(runCurrent->resourceWaitRunner->ellapse) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->resourceWaitRunner = runCurrent->resourceWaitRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/

		//Service wait data for each run
		fileOut << "Aircraft Wait\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,,,,,,");
			runCurrent->serviceWaitRunner = runCurrent->serviceWaitHead;
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("ID,Type,Location,Date Begin,Time Begin,Date End,Time End,Wait Time,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
			{
				if (runCurrent->serviceWaitRunner == nullptr)
				{
					tempStr += (",,,,,,,,");
					endCount++;
				}
				else
				{
					tempStr += (to_string(runCurrent->serviceWaitRunner->aircraftID) + "," + (runCurrent->serviceWaitRunner->aircraftType) + "," + (runCurrent->serviceWaitRunner->location) + "," +
						to_string(runCurrent->serviceWaitRunner->monthStart) + "/" + to_string(runCurrent->serviceWaitRunner->dayStart) + "/" + to_string(runCurrent->serviceWaitRunner->yearStart) + "," + to_string(runCurrent->serviceWaitRunner->timeStart) + "," +
						to_string(runCurrent->serviceWaitRunner->monthEnd) + "/" + to_string(runCurrent->serviceWaitRunner->dayEnd) + "/" + to_string(runCurrent->serviceWaitRunner->yearEnd) + "," + to_string(runCurrent->serviceWaitRunner->timeEnd) + "," +
						to_string(runCurrent->serviceWaitRunner->ellapse) + ",");


					runCurrent->serviceWaitRunner = runCurrent->serviceWaitRunner->next;
				}
				runCurrent = runCurrent->next;
			}
			tempStr += "\n";
			fileOut << tempStr;

		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->serviceWaitRunner = runCurrent->serviceWaitHead;
			while (runCurrent->serviceWaitRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Aircraft Wait (ID, Type, Location, Month Begin, Day Begin, Year Begin, Time Begin, Month End, Day End, Year End, Time End, Wait Time) VALUES ( .('" + to_string(runCurrent->serviceWaitRunner->aircraftID) + "')., '" + (runCurrent->serviceWaitRunner->aircraftType) + "', '" + (runCurrent->serviceWaitRunner->location) + "', '" + to_string(runCurrent->serviceWaitRunner->monthStart) + "', '" + to_string(runCurrent->serviceWaitRunner->dayStart) + "')., '" + to_string(runCurrent->serviceWaitRunner->yearStart) + "', '" + to_string(runCurrent->serviceWaitRunner->timeStart) + "')., '" + to_string(runCurrent->serviceWaitRunner->monthEnd) + "')., '" + to_string(runCurrent->serviceWaitRunner->dayEnd) + "')., '" + to_string(runCurrent->serviceWaitRunner->yearEnd) + "')., '" + to_string(runCurrent->serviceWaitRunner->timeEnd) + "')., '" + to_string(runCurrent->serviceWaitRunner->ellapse) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->serviceWaitRunner = runCurrent->serviceWaitRunner->next;
			}
			//SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Aircraft Wait (ID, Type, Location, Month Begin, Day Begin, Year Begin, Time Begin, Month End, Day End, Year End, Time End, Wait Time) VALUES ( .('')., '', '', '', '', '', '', '', '', '', '', '')"), SQL_NTS);
			runCurrent = runCurrent->next;
		}*/

		//Repair job data for each run
		fileOut << "Repair Jobs\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,,,,,,");
			runCurrent->repairJobRunner = runCurrent->repairJobHead;
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("ID,Type,Job,Step Number,Step Name,Date Start,Start,Date Finish,Finish,Duration,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
			{
				if (runCurrent->repairJobRunner == nullptr)
				{
					tempStr += (",,,,,,,,,,");
					endCount++;
				}
				else
				{
					tempStr += (to_string(runCurrent->repairJobRunner->aircraftID) + "," + (runCurrent->repairJobRunner->aircraftType) + "," + (runCurrent->repairJobRunner->jobType) + "," + (to_string(runCurrent->repairJobRunner->stepNumber)) + "," + (runCurrent->repairJobRunner->stepName) + "," +
						to_string(runCurrent->repairJobRunner->monthStart) + "/" + to_string(runCurrent->repairJobRunner->dayStart) + "/" + to_string(runCurrent->repairJobRunner->yearStart) + "," + to_string(runCurrent->repairJobRunner->timeStart) + "," +
						to_string(runCurrent->repairJobRunner->monthEnd) + "/" + to_string(runCurrent->repairJobRunner->dayEnd) + "/" + to_string(runCurrent->repairJobRunner->yearEnd) + "," + to_string(runCurrent->repairJobRunner->timeEnd) + "," +
						to_string(runCurrent->repairJobRunner->ellapse) + ",");


					runCurrent->repairJobRunner = runCurrent->repairJobRunner->next;
				}
				runCurrent = runCurrent->next;
			}
			tempStr += "\n";
			fileOut << tempStr;

		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->repairJobRunner = runCurrent->repairJobHead;
			while (runCurrent->repairJobRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Repair Jobs (ID, Type, Job, Step Number, Step Name, Month Start, Day Start, Year Start, Start, Month Finish, Day Finish, Year Finish, Finish, Duration) VALUES ( .('" + to_string(runCurrent->repairJobRunner->aircraftID) + "')., '" + (runCurrent->repairJobRunner->aircraftType) + "')., '" + (runCurrent->repairJobRunner->jobType) + "')., '" + to_string(runCurrent->repairJobRunner->stepNumber) + "')., '" + (runCurrent->repairJobRunner->stepName) + "')., '" + to_string(runCurrent->repairJobRunner->monthStart) + "')., '" + to_string(runCurrent->repairJobRunner->dayStart) + "')., '" + to_string(runCurrent->repairJobRunner->yearStart) + "')., '" + to_string(runCurrent->repairJobRunner->timeStart) + "')., '" + to_string(runCurrent->repairJobRunner->monthEnd) + "')., '" + to_string(runCurrent->repairJobRunner->dayEnd) + "')., '" + to_string(runCurrent->repairJobRunner->yearEnd) + "')., '" + to_string(runCurrent->repairJobRunner->timeEnd) + "')., '" + to_string(runCurrent->repairJobRunner->ellapse) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->repairJobRunner = runCurrent->repairJobRunner->next;
			}
			runCurrent->repairJobRunner = runCurrent->repairJobRunner->next;
		}*/

		//Inspection Data for each run
		fileOut << "Inspection Failures\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,,,,");
			runCurrent->inspectionRunner = runCurrent->inspectionHead;
			runCurrent = runCurrent->next;
		}

		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("ID,Type,Job,Step,Date,Time,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
			{
				if (runCurrent->inspectionRunner == nullptr)
				{
					tempStr += (",,,,,,");
					endCount++;
				}
				else
				{
					tempStr += (to_string(runCurrent->inspectionRunner->craftID) + "," + (runCurrent->inspectionRunner->craftType) + "," + (runCurrent->inspectionRunner->repairJob) + "," + to_string(runCurrent->inspectionRunner->stepNum) + "," +
						(runCurrent->inspectionRunner->date) + "," + to_string(runCurrent->inspectionRunner->time));


					runCurrent->inspectionRunner = runCurrent->inspectionRunner->next;
				}
				runCurrent = runCurrent->next;
			}
			tempStr += "\n";
			fileOut << tempStr;
		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->inspectionRunner = runCurrent->inspectionHead;
			while (runCurrent->inspectionRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Inspection Failures (ID, Type, Job, Step, Date, Time) VALUES ( .('" + to_string(runCurrent->inspectionRunner->craftID) + "')., '" + (runCurrent->inspectionRunner->craftType) + "')., '" + (runCurrent->inspectionRunner->repairJob) + "')., '" + to_string(runCurrent->inspectionRunner->stepNum) + "')., '" + (runCurrent->inspectionRunner->date) + "')., '" + to_string(runCurrent->inspectionRunner->time) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->inspectionRunner = runCurrent->inspectionRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/

		//Rework data for each run
		fileOut << "Reworks\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,,");
			runCurrent->reworkRunner = runCurrent->reworkHead;
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Aircraft or Resource Type,Rework Event,Date,Duration,");
		}
		tempStr += "\n";
		fileOut << tempStr;

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
			{
				if (runCurrent->reworkRunner == nullptr)
				{
					tempStr += ",,,,";
					endCount++;
				}
				else
				{
					tempStr += ((runCurrent->reworkRunner->objectType) + "," + (runCurrent->reworkRunner->reworkEvent) + "," + (runCurrent->reworkRunner->date) + "," + to_string(runCurrent->reworkRunner->ellapse) + ",");


					runCurrent->reworkRunner = runCurrent->reworkRunner->next;
				}
				runCurrent = runCurrent->next;
			}
			tempStr += "\n";
			fileOut << tempStr;

		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->reworkRunner = runCurrent->reworkHead;
			while (runCurrent->reworkRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Reworks (Aircraft or Resource Type, Rework Event, Date, Duration) VALUES ( .('" + (runCurrent->reworkRunner->objectType) + "')., '" + (runCurrent->reworkRunner->reworkEvent) + "')., '" + (runCurrent->reworkRunner->date) + "')., '" + to_string(runCurrent->reworkRunner->ellapse) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->reworkRunner = runCurrent->reworkRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/

		//Part Request data for each run
		fileOut << "Parts Requests\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,,");
			runCurrent->requestsRunner = runCurrent->requestsHead;
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
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

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
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

		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->requestsRunner = runCurrent->requestsHead;
			while (runCurrent->requestsRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Parts Requests (Part, Number Used, Times Requested, Unsuccessful Requests) VALUES ( .('" + (runCurrent->requestsRunner->partType) + "')., '" + to_string(runCurrent->requestsRunner->numberUsed) + "')., '" + to_string(runCurrent->requestsRunner->requestNumber) + "')., '" + to_string(runCurrent->requestsRunner->unsuccessfulRequests) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->requestsRunner = runCurrent->requestsRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/

		//Restock data for each run
		fileOut << "Restocking\n";
		tempStr = "";
		runCurrent = runStart;

		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += ("Run " + to_string(i + 1) + ",,,");
			runCurrent->restockRunner = runCurrent->restockHead;
			runCurrent = runCurrent->next;
		}
		tempStr += "\n";
		fileOut << tempStr;

		tempStr = "";
		//for (int i = 0; i < runNumber; i++)
		for (int i = 0; i < _numRuns; i++)
		{
			tempStr += "Part,Date Ordered,TUA,";
		}
		tempStr += "\n";
		fileOut << tempStr;

		do
		{
			endCount = 0;
			tempStr = "";
			runCurrent = runStart;

			//for (int i = 0; i < runNumber; i++)
			for (int i = 0; i < _numRuns; i++)
			{
				if (runCurrent->restockRunner == nullptr)
				{
					tempStr += (",,,");
					endCount++;
				}
				else
				{
					tempStr += ((runCurrent->restockRunner->partType) + "," + (runCurrent->restockRunner->date) + "," + to_string(runCurrent->restockRunner->restockTime) + ",");


					runCurrent->restockRunner = runCurrent->restockRunner->next;
				}
				runCurrent = runCurrent->next;
			}
			tempStr += "\n";
			fileOut << tempStr;

		//} while (endCount < runNumber);
		} while (endCount < _numRuns);

		/*runCurrent = runStart;
		for (int i = 0; i < runNumber; i++)
		{
			runCurrent->restockRunner = runCurrent->restockHead;
			while (runCurrent->restockRunner != nullptr)
			{
				if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)("INSERT INTO Restocking (Part, Date Ordered, TUA (Time Until Arrival)) VALUES ( .('" + (runCurrent->restockRunner->partType) + "')., '" + (runCurrent->restockRunner->date) + "')., '" + to_string(runCurrent->restockRunner->restockTime) + "')").c_str(), SQL_NTS)) {
					cout << "Error querying SQL Server";
					cout << "\n";
					goto COMPLETED;
				}
				else {
					//declare output variable and pointer
					SQLCHAR sqlValue[SQL_RESULT_LEN];
					SQLINTEGER ptrSqlValue;
					while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
						SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlValue, SQL_RESULT_LEN, &ptrSqlValue);
						//display query result
						cout << "\nQuery Result:\n\n";
						cout << sqlValue << endl;
					}
				}
				runCurrent->restockRunner = runCurrent->restockRunner->next;
			}
			runCurrent = runCurrent->next;
		}*/



		/////////////////////////////////////////
		////// DISCONNECTING FROM DATABASE //////
		/////////////////////////////////////////
	/*
		//close connection and free resources
		COMPLETED:
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		SQLDisconnect(sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

		////pause the console window - exit when key is pressed
		cout << "\nPress enter key to exit...";
		getchar();

	*/
	//////////////////////////////////////////////////////////
		runNumber++;
	//}
	cout << "Archived to: " + fileName;
	system("pause");
}

runNode* Scribe::GetStart()
{
	return runStart;
}

InspectionFailureNode::InspectionFailureNode()
{
	craftID = 0;
	craftType = "";
	repairJob = "";
	stepNum = 0;
	date = "";
	time = 0;

	next = nullptr;
}

InspectionFailureNode::InspectionFailureNode(int id, string aircraft, string inspectionName, int step)
{
	craftID = id;
	craftType = aircraft;
	repairJob = inspectionName;
	stepNum = step;
	date = (to_string(SimExec::GetSimulationTime()._month + 1) + "/" + to_string(SimExec::GetSimulationTime()._day) + "/" + to_string(SimExec::GetSimulationTime()._year));
	time = SimExec::GetSimulationTime()._timeOfDay;

	next = nullptr;
}

InspectionFailureNode::InspectionFailureNode(const InspectionFailureNode& node2)
{
	craftID = node2.craftID;
	craftType = node2.craftType;
	repairJob = node2.repairJob;
	stepNum = node2.stepNum;
	date = node2.date;
	time = node2.time;

	next = node2.next;
}

InspectionFailureNode::~InspectionFailureNode()
{
	delete this;
}