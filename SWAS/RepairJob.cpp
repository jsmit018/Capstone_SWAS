#include "RepairJob.h"

RepairJob::RepairJob()
{
    _schedType = "n/a";
    _calendarDate = "n/a";
    _recurringAmt = 0.0;
    _unplannedProb = "1";
}

//RepairJob::RepairJob(string name, int listSize) {
//    _name = name;
//    _activeStep = 0;
//    _stepList = new Step*[listSize];
//}
//


void RepairJob::SetName(string name) 
{
    _name = name;
}

//string RepairJob::GetName() {
//    return _name;
//}

void RepairJob::SetPriority(int priority)
{  
    _priority = priority;
}

void RepairJob::SetSchedType(string schedType)
{
    _schedType = schedType;
}

void RepairJob::SetIndoorReq(char indoorReq)
{
    _indoorReq = indoorReq;
}

void RepairJob::SetCalendarDate(string calendarDate)
{
    _calendarDate = calendarDate;
    //convert in calendar converter
}

void RepairJob::SetRecurring(double recurringAmt)
{
    _recurringAmt = recurringAmt;
    //change to double
}

void RepairJob::SetUnplannedProb(string unplannedProb)
{
    _unplannedProb = unplannedProb;
    //change to distribution
}

//
//void RepairJob::GetNextStep() {
//    _activeStep++;
//}
//
//void RepairJob::AddStep(Step* step, int listSpot) {
//    _stepList[listSpot] = step;
//}

void RepairJob::PrintJobProperties()
{
    cout << "job name: " << _name << endl;
    cout << "priority: " << _priority << endl;
    cout << "probability: " << _unplannedProb << endl;
    cout << "sched type: " << _schedType << endl;
    cout << "cal: " << _calendarDate << endl;
    cout << "recur: " << _recurringAmt << endl;
    cout << "indoor req: " << _indoorReq << endl;
    cout << endl;
}