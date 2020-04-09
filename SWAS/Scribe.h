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
#include<cstdlib>
#include<iostream>
#include<fstream>

using namespace std;


//Branch nodes
struct aircraftNode
{
public:
    //constructors
    aircraftNode();
    aircraftNode(string);
    aircraftNode(const aircraftNode&);
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
    missionNode(string);
    missionNode(const missionNode&);
    ~missionNode();

    string type;

    missionNode* next;
};

struct resourceNode
{
public:
    //constructors
    resourceNode();
    resourceNode(string, int);
    resourceNode(const resourceNode&);
    ~resourceNode();

    string type;
    int initialCount;
    float lastchange;
    int availability;
    float utilizationHours;
    float utilizationPercent;
    int requestNumber;
    int unsuccessfulRequests;

    resourceNode* next;
};

struct failureNode
{
public:
    //constructors
    failureNode();
    failureNode(string, string, float);
    failureNode(const failureNode&);
    ~failureNode();

    string resourceType;
    string failureType;
    float duration;

    failureNode* next;
};

struct resourceWaitNode
{
public:
    //constructors
    resourceWaitNode();
    resourceWaitNode(string, int, string, float);
    resourceWaitNode(const resourceWaitNode&);
    ~resourceWaitNode();

    string aircraftType;
    int aircraftID;
    string resourceType;
    float timeStart;
    float timeEnd;
    float duration;

    resourceWaitNode* next;
};

struct serviceWaitNode
{
public:
    //constructors
    serviceWaitNode();
    serviceWaitNode(string, int, string, float);
    serviceWaitNode(const serviceWaitNode&);
    ~serviceWaitNode();

    string aircraftType;
    int aircraftID;
    string location;
    float timeStart;
    float timeEnd;
    float duration;

    serviceWaitNode* next;
};

struct repairJobNode
{
public:
    //constructors
    repairJobNode();
    repairJobNode(string, int, string, float);
    repairJobNode(const repairJobNode&);
    ~repairJobNode();

    string aircraftType;
    int aircraftID;
    string jobType;
    float timeStart;
    float timeEnd;
    float duration;

    repairJobNode* next;
};

struct reworkNode
{
public:
    //constructors
    reworkNode();
    reworkNode(string, string, float);
    reworkNode(const reworkNode&);
    ~reworkNode();

    string objectType;
    string reworkEvent;
    float duration;

    reworkNode* next;
};

struct partRequestNode
{
public:
    //constructors
    partRequestNode();
    partRequestNode(string);
    partRequestNode(const partRequestNode&);
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
    restockNode(string, float);
    restockNode(const restockNode&);
    ~restockNode();

    string partType;
    float restockTime;

    restockNode* next;
};

//Spine node
struct runNode
{
public:
    runNode();
    runNode(const runNode&);
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

    runNode* next;
};

class Scribe
{
public:

    Scribe();
    Scribe(int);
    Scribe(const Scribe&);
    ~Scribe();

    //Record methods
    void RecordAircraft(string);
    void TallyAircraft(string);
    void RecordMission(string);
    void RecordResource(string, int);
    void UpdateResourceUtilization(string, int, float);
    void UpdateResourceUtilization();
    void UpdateResourceRequests(string, bool);
    void RecordFailure(string, string, float);
    void RecordResourceWait(string, int, string, float);
    void RecordResourceWaitEnd(int, string, float);
    void RecordServiceWait(string, int, string, float);
    void RecordServiceWaitEnd(int, string, float);
    void RecordRepairJob(string, int, string, float);
    void RecordRepairEnd(int, string, float);
    void RecordRework(string, string, float);
    void RecordPartRequest(string, int, bool);
    void RecordRestock(string, float);
    void AdvanceRun();

    void SetWarehousDims(string, string);
    void SetRunTime(float);
    void SetPlanned(int);
    void TallyUnplanned(int);
    void SetSeed(double);
    void SetSaveFile(string);


    //Output methods
    void Archive();

    //Access Methods
    runNode* GetStart();


private:
    runNode* runStart;
    runNode* runEnd;
    runNode* runCurrent;

    string warehouseL, warehouseW;
    string fileName;
    float runtime;
    int planned, unplanned;
    int runNumber;
    double seedVal;
    float totalRuntime;
};