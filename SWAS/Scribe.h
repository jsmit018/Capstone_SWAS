/*Scribe.h: Kevin Voyt
  This class is meant to store and record data during the simulation runs via a series of linked lists.
  At the end of the simulation, it is also called to record the data to a .csv file.
  The "spine" of this set is made of the simulation runs.
  the "branches" are made of the included aircraft, included missions, included resources, resource failures, wait times due to resources, service wait times,
    repair jobs, rework events, part requests, and restocking events.
*/

#pragma once
#include<string>
#include<sstream>
#include<cmath>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include "SimExec.h"
//#include "InputInterface.h"

using namespace std;

class InputReader;
//Branch nodes
struct aircraftNode
{
public:
    //constructors
    aircraftNode();
    aircraftNode(string aircraftType);
    aircraftNode(const aircraftNode& node2);
    ~aircraftNode();

    string type;
    int count;

    aircraftNode* next;
};

struct missionNode
{
public:
    //constructors
    missionNode();
    missionNode(string missionName);
    missionNode(const missionNode& node2);
    ~missionNode();

    string type;

    missionNode* next;
};

struct resourceNode
{
public:
    //constructors
    resourceNode();
    resourceNode(string resourceName, int initialization);
    resourceNode(const resourceNode& node2);
    ~resourceNode();

    string type;
    int initialCount;
    float lastchange;
    int availability;
    double utilizationHours;
    double utilizationPercent;
    int requestNumber;
    int unsuccessfulRequests;

    resourceNode* next;
};

struct failureNode
{
public:
    //constructors
    failureNode();
    failureNode(string resource, string failure, float time);
    failureNode(const failureNode& node2);
    ~failureNode();

    string resourceType;
    string failureType;
    string date;
    float tStart;
    float tEnd;
    float ellapse;

    failureNode* next;
};

struct resourceWaitNode
{
public:
    //constructors
    resourceWaitNode();
    resourceWaitNode(string aircraft, int ID, string resource, float time);
    resourceWaitNode(const resourceWaitNode& node2);
    ~resourceWaitNode();

    string aircraftType;
    int aircraftID;
    string resourceType;
    float timeStart;
    double dayStart;
    double monthStart;
    double yearStart;
    float timeEnd;
    double dayEnd;
    double monthEnd;
    double yearEnd;
    float ellapse;

    resourceWaitNode* next;
};

struct serviceWaitNode
{
public:
    //constructors
    serviceWaitNode();
    serviceWaitNode(string aircraft, int ID, string spot, float time);
    serviceWaitNode(const serviceWaitNode& node2);
    ~serviceWaitNode();

    string aircraftType;
    int aircraftID;
    string location;
    float timeStart;
    double dayStart;
    double monthStart;
    double yearStart;
    float timeEnd;
    double dayEnd;
    double monthEnd;
    double yearEnd;
    float ellapse;

    serviceWaitNode* next;
};

struct repairJobNode
{
public:
    //constructors
    repairJobNode();
    repairJobNode(string aircraft, int id, string job, int stepNum, string step, float time);
    repairJobNode(const repairJobNode& node2);
    ~repairJobNode();

    string aircraftType;
    int aircraftID;
    string jobType;
    int stepNumber;
    string stepName;
    float timeStart;
    double dayStart;
    double monthStart;
    double yearStart;
    float timeEnd;
    double dayEnd;
    double monthEnd;
    double yearEnd;
    float ellapse;

    repairJobNode* next;
};

struct reworkNode
{
public:
    //constructors
    reworkNode();
    reworkNode(string object, string rework, float time);
    reworkNode(const reworkNode& node2);
    ~reworkNode();

    string objectType;
    string reworkEvent;
    string date;
    float ellapse;

    reworkNode* next;
};

struct partRequestNode
{
public:
    //constructors
    partRequestNode();
    partRequestNode(string part);
    partRequestNode(const partRequestNode& node2);
    ~partRequestNode();

    string partType;
    int numberUsed;
    int requestNumber;
    int unsuccessfulRequests;

    partRequestNode* next;
};

struct restockNode
{
public:
    restockNode();
    restockNode(string part, float time);
    restockNode(const restockNode& node2);
    ~restockNode();

    string partType;
    float restockTime;
    string date;

    restockNode* next;
};

struct InspectionFailureNode
{
public:
    InspectionFailureNode();
    InspectionFailureNode(int id, string aircraft, string inspectionName, int step);
    InspectionFailureNode(const InspectionFailureNode& node2);
    ~InspectionFailureNode();

    int craftID;
    string craftType;
    string repairJob;
    int stepNum;
    string date;
    float time;

    InspectionFailureNode* next;
};

//Node for a single simulation run with pointers to lists of aircraft, missions, resources, failures, resourcewaits, servicewaits, repairjobs, reworks, part requests and restocks
struct runNode
{
public:
    runNode();
    runNode(const runNode& node2);
    ~runNode();

    //pointers to start of branches
    aircraftNode* aircraftHead;
    missionNode* missionHead;
    resourceNode* resourceHead;
    failureNode* failureHead;
    resourceWaitNode* resourceWaitHead;
    serviceWaitNode* serviceWaitHead;
    repairJobNode* repairJobHead;
    reworkNode* reworkHead;
    partRequestNode* requestsHead;
    restockNode* restockHead;
    InspectionFailureNode* inspectionHead;

    //search pointers
    aircraftNode* aircraftRunner;
    missionNode* missionRunner;
    resourceNode* resourceRunner;
    failureNode* failureRunner;
    resourceWaitNode* resourceWaitRunner;
    serviceWaitNode* serviceWaitRunner;
    repairJobNode* repairJobRunner;
    reworkNode* reworkRunner;
    partRequestNode* requestsRunner;
    restockNode* restockRunner;
    InspectionFailureNode* inspectionRunner;

    //Tail Pointers
    aircraftNode* aircraftTail;
    missionNode* missionTail;
    resourceNode* resourceTail;
    failureNode* failureTail;
    resourceWaitNode* resourceWaitTail;
    serviceWaitNode* serviceWaitTail;
    repairJobNode* repairJobTail;
    reworkNode* reworkTail;
    partRequestNode* requestsTail;
    restockNode* restockTail;
    InspectionFailureNode* inspectionTail;

    runNode* next;
};

class Scribe
{
public:

    Scribe();
    Scribe(int runs);
    Scribe(const Scribe& node2);
    ~Scribe();

    //Record methods
	static void SetNumRuns(int numRuns);
    static void RecordAircraft(string aircraft);
    static void TallyAircraft(string aircraft);
    static void RecordMission(string mission);
    static void RecordResource(string resource, int count);
    static void UpdateResourceUtilization(string resource, double usage, float currentTime);
    static void UpdateResourceUtilization();
    static void UpdateResourceRequests(string resource, bool successful);
    static void RecordFailure(string resource, string failure, float time);
    static void RecordRestore(string resource, string failure, float time);
    static void RecordResourceWait(string aircraft, int id, string resource, float start);
    static void RecordResourceWaitEnd(int id, string resource, float end);
    static void RecordServiceWait(string aircraft, int id, string spot, float start);
    static void RecordServiceWaitEnd(int id, string spot, float end);
    static void RecordRepairJob(string aircraft, int id, string job, int stepNum, string stepNme, float start);
    static void RecordRepairEnd(int id, string job, int step, float end);
    static void RecordRework(string object, string rework, float time);
    static void RecordPartRequest(string resource, int usage, bool success);
    static void RecordRestock(string resource, float time);
    static void RecordInspectionFailure(int airID, string airType, string inspect, int stepNum);
    static void AdvanceRun();

    //static void SetWarehousDims(string, string);
    static void SetWarehousDims(double length, double width);
    static void SetRunTime(float runT);
    static void SetPlanned(int known);
    static void SetRuns(int runs);
    static void TallyUnplanned(int unplannedSet);
    static void SetSeed(double value);
    static void SetSaveFile(string file);


    //Output methods
    static void Archive();

    //Access Methods
    static runNode* GetStart();


private:
    static runNode* runStart;
    static runNode* runEnd;
    static runNode* runCurrent;

    static string warehouseL;
    static string warehouseW;
    static string fileName;
    static float runtime;
    static int planned;
    static int unplanned;
    static int runNumber;
    static double seedVal;
    static float totalRuntime;
	static int _numRuns;	
};