#include "RepairJob.h"

map<string, RepairJob*> RepairJob::_resourceRepairMap;

RepairJob::RepairJob()
{
    _schedType = "Unplanned";
    _calendarDate = "n/a";
    _recurringAmt = 0.0;
  //  _unplannedProb = "1";
}

RepairJob::RepairJob(const RepairJob& mapRj)
{
    _name = mapRj._name;					            // Repair job name
    _priority = mapRj._priority;		        		// Repair job priority
    _schedType = mapRj._schedType;				        // Interarrival schedule type
    _indoorReq = mapRj._indoorReq;				        // Y or N for indoor requirement of repair job
    _calendarDate = mapRj._calendarDate;		    	// Calendar-schedule type repair jobs (date: mmddyyyy)
    _recurringAmt = mapRj._recurringAmt;		    	// Number of months between Recurring-schedule type repair jobs
    _unplannedProb = mapRj._unplannedProb;		    	// Distribution for probability of certain repair job after random iat
    _vecSteps = mapRj._vecSteps;		                // RepairJob has a vector of step objects

    // there is a need for a copy constructor for steps and you have to iterate through the vectors 
    // and maps, to ensure that the copies are actual copies and not that the same pointers is not being shared 
    // by different repair jobs

    // for number of steps in mapRj._vecSteps
    // Step * newStep = new Step(mapRj._vecSteps[i])
    // _vecSteps.push_back(newStep)

    // same thing as above for the map. so you haeve to iterate through the map and just do the copy 
    // for each repair job

    // and also maybe do it for distribution, but that might not matter honestly


 //   cout << "***Vec Size: " << _vecSteps.size() << endl;
   // cout << "***Vec Size: " << mapRj._vecSteps.size() << endl;
    //cout << endl;
    _resourceRepairMap = mapRj._resourceRepairMap;		//FOR EACH RESOURCE TYPE, MAKE REPAIR JOB
//    cout << "***Map Size: " << _resourceRepairMap.size() << endl;
 //   cout << endl;
}

Step* RepairJob::GetStep(int stepID)
{
    //setting stepID 
    return _vecSteps[stepID];
    cout << "ID " << stepID << endl;
}

Step* RepairJob::GetFirstStep()
{
    return _vecSteps[0];
}

bool RepairJob::WillSchedule()
{
    if (_unplannedProb->GetRV() >= 0.51)
    {
        return true;
    }
    else
        return false;
}

Distribution* RepairJob::GetUnplannedProb()
{
    return _unplannedProb;
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
    cout << "TYPE: " << _schedType << endl;
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
    istringstream unProb(unplannedProb);
    string firstHalf;
    string secHalf;

    getline(unProb, firstHalf, '(');
    getline(unProb, secHalf, ')');
    //	cout << "first: " << firstHalf << endl;
    //	cout << "sec: " << secHalf << endl;

    istringstream nums(secHalf);
    if (firstHalf == "Triangular" || firstHalf == "Tri")
    {
        double min, expected, max;
        nums >> min;
        nums >> expected;
        nums >> max;
        _unplannedProb = new Triangular(min, expected, max);
    }

    else if (firstHalf == "Exponential" || firstHalf == "Exp")
    {
        double mean;
        nums >> mean;

        _unplannedProb = new Exponential(mean);
    }

    else if (firstHalf == "Uniform" || firstHalf == "Uni")
    {
        double min, max;
        nums >> min >> max;

        _unplannedProb = new Uniform(min, max);
    }

    else if (firstHalf == "Normal" || firstHalf == "Norm")
    {
        double mean, stdev;
        nums >> mean >> stdev;

        _unplannedProb = new Normal(mean, stdev);
    }

    else if (firstHalf == "Poisson")
    {
        double mean;
        nums >> mean;

        _unplannedProb = new Poisson(mean);
    }

    else if (firstHalf == "Constant" || firstHalf == "Fixed")
    {
        double mean;
        nums >> mean;

        _unplannedProb = new Constant(mean);
    }

    else if (firstHalf == "Weibull")
    {
        double scale, shape;
        nums >> scale >> shape;

        _unplannedProb = new Weibull(scale, shape);
    }

    //Determines correct distribution and prints
  //  _unplannedProb->PrintDistribution();
}

//
//void RepairJob::GetNextStep() {
//    _activeStep++;
//}
//

void RepairJob::AddStep(Step* step)
{
    _vecSteps.push_back(step);

    cout << "adding step" << _vecSteps.size() << endl;
    step->SetStepID(_vecSteps.size());
    /*int stepID;

    for (int i = 0; i < _vecSteps.size(); i++)
    {
        stepID = i + 1;
        cout << "step id from add step is " << stepID; 
        step->SetStepID(stepID);
    }*/

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
    cout << "   Schedule Type: " << _schedType << endl;
  //  cout << "   Repair Job Priority: " << _priority << endl;
    cout << "   Unplanned Probability: " << GetUnplannedProb() << endl;
    //cout << "   Calendar Occurrence: " << _calendarDate << endl;
    //cout << "   Reccuring Amount: " << _recurringAmt << endl;
    //cout << "   Indoor Requirement? " << _indoorReq << endl;
    cout << endl;

    for (int i = 0; i < _vecSteps.size(); i++)
    {
        _vecSteps[i]->Print();
        cout << endl;
     //   _vecSteps[1]->PrintPools();
    }
    cout << endl;
}

