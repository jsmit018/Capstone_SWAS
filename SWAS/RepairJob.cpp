#include "RepairJob.h"

map<string, RepairJob*> RepairJob::_resourceRepairMap;

RepairJob::RepairJob()
{
    _schedType = "Unplanned";
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
Step* RepairJob::GetStep(int stepID)
{
    //setting stepID 
    return _vecSteps[stepID];
}

int RepairJob::GetStepVecSize()
{
    return _vecSteps.size();
}

void RepairJob::SetName(string name)
{
    _name = name;
}

string RepairJob::GetName() {
    return _name;
}

void RepairJob::SetPriority(int priority)
{
    _priority = priority;
}

int RepairJob::GetPriority()
{
    return _priority;
}

void RepairJob::SetSchedType(string schedType)
{
    _schedType = schedType;
}

string RepairJob::GetSchedType()
{
    return _schedType;
}

void RepairJob::SetIndoorReq(char indoorReq)
{
    _indoorReq = indoorReq;
}

char RepairJob::GetIndoorReq()
{
    return _indoorReq;
}

void RepairJob::SetCalendarDate(string calendarDate)
{
    _calendarDate = calendarDate;
    //convert in calendar converter
}

string RepairJob::GetCalendarDate()
{
    return _calendarDate;
}

void RepairJob::SetRecurring(double recurringAmt)
{
    _recurringAmt = recurringAmt;
    //change to double
}

double RepairJob::GetRecurring()
{
    return _recurringAmt;
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

void RepairJob::AddStep(Step* step)
{
   //    cout << "adding step \n";
    _vecSteps.push_back(step);
    //    cout << endl << "new size " << vecSteps.size() << endl;
};

//Step* RepairJob::GetSteps()
//{
//    for (int i = 0; i < _vecSteps.size; i++)
//    {
//        return _vecSteps[i];
//    }
//}

void RepairJob::AddResourceRepair(RepairJob* repairJob, string resourceName)
{
    _resourceRepairMap[resourceName] = repairJob;
}

void RepairJob::PrintResourceRepairs()
{
    map<string, RepairJob*>::iterator iter = _resourceRepairMap.begin();

    while (iter != _resourceRepairMap.end())
    {
        cout << iter->second->GetName() << endl;
        iter++;
    }
}

RepairJob* RepairJob::GetResourceRepair(string resourceName)
{
    map<string, RepairJob*>::iterator it = _resourceRepairMap.find(resourceName);
    if (it == _resourceRepairMap.end())
        return nullptr;
    return it->second;
}

void RepairJob::PrintJobProperties()
{
    cout << "   Repair Job Name: " << _name << endl;
    //cout << "   Schedule Type: " << _schedType << endl;
    //cout << "   Repair Job Priority: " << _priority << endl;
    //cout << "   Unplanned Probability: " << _unplannedProb << endl;
    //cout << "   Calendar Occurrence: " << _calendarDate << endl;
    //cout << "   Reccuring Amount: " << _recurringAmt << endl;
    //cout << "   Indoor Requirement? " << _indoorReq << endl;
    cout << endl;

    for (int i = 0; i < _vecSteps.size(); i++)
    {
      //  _vecSteps[i]->Print();
        cout << endl;
        _vecSteps[1]->PrintPools();
    }
    cout << endl;
}

