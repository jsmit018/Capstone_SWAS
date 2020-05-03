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

using namespace std ;


//Branch nodes
struct aircraftNode
{
public:
    //constructors
    aircraftNode() ;
    aircraftNode(string) ;
    aircraftNode(const aircraftNode&) ;
    ~aircraftNode() ;

    string type ;
    int count ;

    aircraftNode* next ;
} ;

struct missionNode
{
public:
    //constructors
    missionNode() ;
    missionNode(string) ;
    missionNode(const missionNode&) ;
    ~missionNode() ;

    string type ;

    missionNode* next ;
} ;

struct resourceNode
{
public:
    //constructors
    resourceNode() ;
    resourceNode(string, int) ;
    resourceNode(const resourceNode&) ;
    ~resourceNode() ;

    string type ;
    int initialCount ;
    float lastchange ;
    int availability ;
    double utilizationHours ;
    double utilizationPercent ;
    int requestNumber ;
    int unsuccessfulRequests ;

    resourceNode* next ;
} ;

struct failureNode
{
public:
    //constructors
    failureNode() ;
    failureNode(string, string, float) ;
    failureNode(const failureNode&) ;
    ~failureNode() ;

    string resourceType ;
    string failureType ;
    string date ;
    float tStart ;
    float tEnd ;
    float ellapse ;

    failureNode* next ;
} ;

struct resourceWaitNode
{
public:
    //constructors
    resourceWaitNode() ;
    resourceWaitNode(string, int, string, float) ;
    resourceWaitNode(const resourceWaitNode&) ;
    ~resourceWaitNode() ;

    string aircraftType ;
    int aircraftID ;
    string resourceType ;
    float timeStart ;
    double dayStart ;
    double monthStart ;
    double yearStart ;
    float timeEnd ;
    double dayEnd ;
    double monthEnd ;
    double yearEnd ;
    float ellapse ;

    resourceWaitNode* next ;
} ;

struct serviceWaitNode
{
public:
    //constructors
    serviceWaitNode() ;
    serviceWaitNode(string, int, string, float) ;
    serviceWaitNode(const serviceWaitNode&) ;
    ~serviceWaitNode() ;

    string aircraftType ;
    int aircraftID ;
    string location ;
    float timeStart ;
    double dayStart ;
    double monthStart ;
    double yearStart ;
    float timeEnd ;
    double dayEnd ;
    double monthEnd ;
    double yearEnd ;
    float ellapse ;

    serviceWaitNode* next ;
} ;

struct repairJobNode
{
public:
    //constructors
    repairJobNode() ;
    repairJobNode(string, int, string, int, string, float) ;
    repairJobNode(const repairJobNode&) ;
    ~repairJobNode() ;

    string aircraftType ;
    int aircraftID ;
    string jobType ;
    int stepNumber ;
    string stepName ;
    float timeStart ;
    double dayStart ;
    double monthStart ;
    double yearStart ;
    float timeEnd ;
    double dayEnd ;
    double monthEnd ;
    double yearEnd ;
    float ellapse ;

    repairJobNode* next ;
} ;

struct reworkNode
{
public:
    //constructors
    reworkNode() ;
    reworkNode(string, string, float) ;
    reworkNode(const reworkNode&) ;
    ~reworkNode() ;

    string objectType ;
    string reworkEvent ;
    string date ;
    float ellapse ;

    reworkNode* next ;
} ;

struct partRequestNode
{
public:
    //constructors
    partRequestNode() ;
    partRequestNode(string) ;
    partRequestNode(const partRequestNode&) ;
    ~partRequestNode() ;

    string partType ;
    int numberUsed ;
    int requestNumber ;
    int unsuccessfulRequests ;

    partRequestNode* next ;
} ;

struct restockNode
{
public:
    restockNode() ;
    restockNode(string, float) ;
    restockNode(const restockNode&) ;
    ~restockNode() ;

    string partType ;
    float restockTime ;
    string date ;

    restockNode* next ;
} ;

struct InspectionFailureNode
{
public:
    InspectionFailureNode() ;
    InspectionFailureNode(int, string, string, int) ;
    InspectionFailureNode(const InspectionFailureNode&) ;
    ~InspectionFailureNode() ;

    int craftID ;
    string craftType ;
    string repairJob ;
    int stepNum ;
    string date ;
    float time ;

    InspectionFailureNode* next ;
} ;

//Node for a single simulation run with pointers to lists of aircraft, missions, resources, failures, resourcewaits, servicewaits, repairjobs, reworks, part requests and restocks
struct runNode
{
public:
    runNode() ;
    runNode(const runNode&) ;
    ~runNode() ;

    //pointers to start of branches
    aircraftNode* aircraftHead ;
    missionNode* missionHead ;
    resourceNode* resourceHead ;
    failureNode* failureHead ;
    resourceWaitNode* resourceWaitHead ;
    serviceWaitNode* serviceWaitHead ;
    repairJobNode* repairJobHead ;
    reworkNode* reworkHead ;
    partRequestNode* requestsHead ;
    restockNode* restockHead ;
    InspectionFailureNode* inspectionHead ;

    //search pointers
    aircraftNode* aircraftRunner ;
    missionNode* missionRunner ;
    resourceNode* resourceRunner ;
    failureNode* failureRunner ;
    resourceWaitNode* resourceWaitRunner ;
    serviceWaitNode* serviceWaitRunner ;
    repairJobNode* repairJobRunner ;
    reworkNode* reworkRunner ;
    partRequestNode* requestsRunner ;
    restockNode* restockRunner ;
    InspectionFailureNode* inspectionRunner ;

    //Tail Pointers
    aircraftNode* aircraftTail ;
    missionNode* missionTail ;
    resourceNode* resourceTail ;
    failureNode* failureTail ;
    resourceWaitNode* resourceWaitTail ;
    serviceWaitNode* serviceWaitTail ;
    repairJobNode* repairJobTail ;
    reworkNode* reworkTail ;
    partRequestNode* requestsTail ;
    restockNode* restockTail ;
    InspectionFailureNode* inspectionTail ;

    runNode* next ;
} ;

class Scribe
{
public:

    Scribe() ;
    Scribe(int) ;
    Scribe(const Scribe&) ;
    ~Scribe() ;

    //Record methods
    static void RecordAircraft(string) ;
    static void TallyAircraft(string) ;
    static void RecordMission(string) ;
    static void RecordResource(string, int) ;
    static void UpdateResourceUtilization(string, double, float) ;
    static void UpdateResourceUtilization() ;
    static void UpdateResourceRequests(string, bool) ;
    static void RecordFailure(string, string, float) ;
    static void RecordRestore(string, string, float) ;
    static void RecordResourceWait(string, int, string, float) ;
    static void RecordResourceWaitEnd(int, string, float) ;
    static void RecordServiceWait(string, int, string, float) ;
    static void RecordServiceWaitEnd(int, string, float) ;
    static void RecordRepairJob(string, int, string, int, string, float) ;
    static void RecordRepairEnd(int, string, int, float) ;
    static void RecordRework(string, string, float) ;
    static void RecordPartRequest(string, int, bool) ;
    static void RecordRestock(string, float) ;
    static void RecordInspectionFailure(int, string, string, int) ;
    static void AdvanceRun() ;

    //static void SetWarehousDims(string, string) ;
    static void SetWarehousDims(double, double) ;
    static void SetRunTime(float) ;
    static void SetPlanned(int) ;
    static void SetRuns(int) ;
    static void TallyUnplanned(int) ;
    static void SetSeed(double) ;
    static void SetSaveFile(string) ;


    //Output methods
    static void Archive() ;

    //Access Methods
    static runNode* GetStart() ;


private:
    static runNode* runStart ;
    static runNode* runEnd ;
    static runNode* runCurrent ;

    static string warehouseL ;
    static string warehouseW ;
    static string fileName ;
    static float runtime ;
    static int planned ;
    static int unplanned ;
    static int runNumber ;
    static double seedVal ;
    static float totalRuntime ;
} ;