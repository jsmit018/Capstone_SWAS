#include "RepairJob.h"

map<string, RepairJob*> RepairJob::_resourceRepairMap;

RepairJob::RepairJob()
{
    _schedType = "Unplanned";
    _calendarDate = "n/a";
    //_recurringAmt = 0.0;
   _unplannedProb = new Constant(0.0);
   _recurringAmt = new Constant(0.0);
}

void RepairJob::CopyRepairJob(const RepairJob& mapRj)
{
    cout << "       Copying " << mapRj._name << " repair job."<< endl;
    _name = mapRj._name;					            // Repair job name
    _priority = mapRj._priority;		        		// Repair job priority
    _schedType = mapRj._schedType;				        // Interarrival schedule type
    _indoorReq = mapRj._indoorReq;				        // Y or N for indoor requirement of repair job
    _calendarDate = mapRj._calendarDate;		    	// Calendar-schedule type repair jobs (date: mmddyyyy)
    _recurringAmt = mapRj._recurringAmt->CopyThis();                // Number of months between Recurring-schedule type repair jobs
    _unplannedProb = mapRj._unplannedProb->CopyThis();	// Distribution for probability of certain repair job after random iat

    //cout << "TEST ";
    //_unplannedProb->PrintDistribution();
    //cout << endl;

    // to ensure that the copies are actual copies and 
    //that the same pointers is not being shared by different repair jobs:

   // cout << "OLD REPAIR STEPS VECtoR SIZE ****: " << mapRj._vecSteps.size() << endl;
    //for copying the vector of steps
    // for number of steps in mapRj._vecSteps
    for (int i = 0; i < mapRj._vecSteps.size(); i++)
    {
        //new step object is created
        Step* newStep = new Step(mapRj._vecSteps[i]->GetServiceTime(), mapRj._vecSteps[i]->GetName());
        //for each index of the old vector, copy the old step object (which catalyzes the sequence of copying objects within step
        newStep->CopyMapStep(*mapRj._vecSteps[i]);
        //store new step copy into new step vector
        _vecSteps.push_back(newStep);
      //  cout << endl;
        //   cout << "  ****      MY STEPS VEC SIZE:  " << _vecSteps.size() << endl;
      //  cout << endl;
    }
    // and also maybe for distribution, but that might not matter?

    //cout << "***Vec Size: " << _vecSteps.size() << endl;
    //cout << "***Vec Size: " << mapRj._vecSteps.size() << endl;
    //cout << endl;
    //cout << "***Map Size: " << _resourceRepairMap.size() << endl;
    //cout << endl;
}

Step* RepairJob::GetStep(int stepID)
{
    if (_vecSteps.size() == 0)
    {
        cout << "NO STEPS!" << endl;
    }
    
    //setting stepID 
     //  cout << "ID IS      " << stepID << endl;
    return _vecSteps[stepID - 1];
    
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

string RepairJob::GetName()
{
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
  //  cout << "TYPE: " << _schedType << endl;
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
}

string RepairJob::GetCalendarDate()
{
    //return tuple
    return _calendarDate;
}

void RepairJob::SetRecurringAmt(double recurringAmt)
{
    _recurringAmt = new Constant(recurringAmt);


 	//cout << "*****************************IAT RECURRING IS: ";
  //     _recurringAmt->PrintDistribution();
  // 	cout << endl;

}


Distribution* RepairJob::GetRecurringAmt()
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
  //  cout << " 99(3(#((##( ";
  //  _unplannedProb->PrintDistribution();
  //  cout << endl;
}


void RepairJob::AddStep(Step* step)
{
    _vecSteps.push_back(step);

  //  cout << "adding step" << _vecSteps.size() << endl;
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
//
//void RepairJob::GetNextStep()
//{
//    for (int i; i < _vecSteps.size(); i++)
//    {
//        if (i = _vecSteps.size() + 1 || )
//        {
//
//        }
//    }
//}

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
    cout << "   Repair Job Priority: " << _priority << endl;
    cout << "   Unplanned Probability: ";
    _unplannedProb->PrintDistribution();
    cout << endl;
    //cout << "   Calendar Occurrence: " << _calendarDate << endl;
    //cout << "   Reccuring Amount: " << _recurringAmt << endl;
    cout << "   Indoor Requirement? " << _indoorReq << endl;
    cout << endl;

    for (int i = 0; i < _vecSteps.size(); i++)
    {
        _vecSteps[i]->Print();
        cout << endl;
//        _vecSteps[1]->PrintPools();
    }
    cout << endl;
}

