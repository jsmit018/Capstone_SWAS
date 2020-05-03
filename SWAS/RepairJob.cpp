#include "RepairJob.h"



RepairJob::RepairJob()
{
    _schedType = "Unplanned" ; 
    _calendarDate = "n/a" ; 
    //_recurringAmt = 0.0 ; 
    _unplannedProb = new Constant(0.0) ; 
    _recurringAmt = new Constant(0.0) ; 
}

void RepairJob::CopyRepairJob(const RepairJob& mapRj)
{
    //   cout << "       Copying " << mapRj._name << " repair job."<< endl ; 
     //   cout << endl ; 
    _name = mapRj._name ; 					            // Repair job name
    _priority = mapRj._priority ; 		        		// Repair job priority
    _schedType = mapRj._schedType ; 				        // Interarrival schedule type
    _indoorReq = mapRj._indoorReq ; 				        // Y or N for indoor requirement of repair job
    _calendarDate = mapRj._calendarDate ; 		    	// Calendar-schedule type repair jobs (date: mmddyyyy)
    _recurringAmt = mapRj._recurringAmt->CopyThis() ; // Number of months between Recurring-schedule type repair jobs
    _unplannedProb = mapRj._unplannedProb->CopyThis() ; 	// Distribution for probability of certain repair job after random iat


    for (int i = 0 ;i < mapRj._vecSteps.size() ;i++)
    {

        //new step object is created
        Step* newStep = new Step(mapRj._vecSteps[i]->GetServiceTime(), mapRj._vecSteps[i]->GetName()) ; 
        //for each index of the old vector, copy the old step object (which catalyzes the sequence of copying objects within step
        newStep->CopyMapStep(*mapRj._vecSteps[i]) ; 
        //store new step copy into new step vector
        _vecSteps.push_back(newStep) ; 

    }

}

int RepairJob::GetMyReturnStep()
{
    int returnStep ; 

    for (int i = 0 ;i < GetStepVecSize() ;i++)
    {
        if (_vecSteps[i]->GetReturnStep() != 0)
            returnStep = _vecSteps[i]->GetReturnStep() ; 
    }
    return returnStep ; 
}

Step* RepairJob::GetStep(int stepID)
{
    if (_vecSteps.size() == 0)
    {
        cout << "NO STEPS!" << endl ; 
    }

    //setting stepID 
   // cout << "-----JOB TYPE " << this->GetName() << endl ; 
   // cout << "ID IS      " << stepID << endl ; 

    return _vecSteps[stepID - 1] ; 

}

Step* RepairJob::GetFirstStep()
{
    return _vecSteps[0] ; 


}

bool RepairJob::WillSchedule()
{
    //if (_unplannedProb->GetRV() >= 0.51)
    //{
    return true ; 
    //}
    //else
    //    return false ; 
}

Distribution* RepairJob::GetUnplannedProb()
{
    return _unplannedProb ; 
}

int RepairJob::GetStepVecSize()
{
    /*   cout << " xxxxxx IN GET STEP VEC SIZE " <<
           this->GetName() << " STEP VEC SIZE IS " <<
           _vecSteps.size() << endl ; 
    */   return _vecSteps.size() ; 
}

void RepairJob::SetName(string name)
{
    _name = name ; 
}

string RepairJob::GetName()
{
    return _name ; 
}

//void RepairJob::SetPriority(int priority)
//{
//    _priority = this->GetStep(1)->GetRJPriority() ; 
//    cout << endl ; 
//    cout << endl ; 
//    cout << "&%&%&%&%&%&%& SETTER PRIORITY " << _priority << endl ; 
//
//    //_priority = priority ; 
//}

int RepairJob::GetPriority()
{
    _priority = this->GetStep(1)->GetRJPriority() ; 
    // cout << endl ; 
     //cout << endl ; 
   //  cout << "&%&%&%&%&%&%& GETTER PRIORITY " << _priority << endl ; 
     //system("PAUSE") ; 
    return _priority ; 
}

void RepairJob::SetSchedType(string schedType)
{
    _schedType = schedType ; 
    // cout << " ++++++++++++++++++IN SET SCHED TYPE PRIORITY IS " << _priority << endl ; 
    // cout << " ++++++++++++++++++IN SET SCHED TYPE TYPE IS " << _schedType << endl ; 
}

string RepairJob::GetSchedType()
{
    /*   cout << endl ; 
       cout << endl ; 
       cout << endl ; 
       cout << endl ; 
       cout << endl ; 
       cout << endl ; 
       cout << "*************#*#*#*#*#**#*##" << endl ; 
      cout << "TYPE: " << _schedType << endl ; */
      // cout << "********" << _name << " " << _priority << endl ; 

       //cout << "*************IN GET SCHED TYPE: " << _name << " " << _schedType << endl ; 
    return _schedType ; 


}

void RepairJob::SetIndoorReq(char indoorReq)
{
    _indoorReq = indoorReq ; 
}

char RepairJob::GetIndoorReq()
{
    // cout << "INDOOR REQ IS " << _indoorReq << endl ; 
    return _indoorReq ; 
}

void RepairJob::SetCalendarDate(string calendarDate)
{
    _calendarDate = calendarDate ; 
}

string RepairJob::GetCalendarDate()
{
    //return tuple
    return _calendarDate ; 
}

void RepairJob::SetRecurringAmt(double recurringAmt)
{
    _recurringAmt = new Constant(recurringAmt) ; 


    /*  cout << "*****************************IAT RECURRING IS: " ; 
          _recurringAmt->PrintDistribution() ; 
       cout << endl ; */

}


Distribution* RepairJob::GetRecurringAmt()
{
    //  _recurringAmt->PrintDistribution() ; 
    return _recurringAmt ; 
}

void RepairJob::SetUnplannedProb(string unplannedProb)
{
    istringstream unProb(unplannedProb) ; 
    string firstHalf ; 
    string secHalf ; 

    getline(unProb, firstHalf, '(') ; 
    getline(unProb, secHalf, ')') ; 
    //	cout << "first: " << firstHalf << endl ; 
    //	cout << "sec: " << secHalf << endl ; 

    istringstream nums(secHalf) ; 
    if (firstHalf == "Triangular" || firstHalf == "Tri")
    {
        double min, expected, max ; 
        nums >> min ; 
        nums >> expected ; 
        nums >> max ; 
        _unplannedProb = new Triangular(min, expected, max) ; 
    }

    else if (firstHalf == "Exponential" || firstHalf == "Exp")
    {
        double mean ; 
        nums >> mean ; 

        _unplannedProb = new Exponential(mean) ; 
    }

    else if (firstHalf == "Uniform" || firstHalf == "Uni")
    {
        double min, max ; 
        nums >> min >> max ; 

        _unplannedProb = new Uniform(min, max) ; 
    }

    else if (firstHalf == "Normal" || firstHalf == "Norm")
    {
        double mean, stdev ; 
        nums >> mean >> stdev ; 

        _unplannedProb = new Normal(mean, stdev) ; 
    }

    else if (firstHalf == "Poisson")
    {
        double mean ; 
        nums >> mean ; 

        _unplannedProb = new Poisson(mean) ; 
    }

    else if (firstHalf == "Constant" || firstHalf == "Fixed")
    {
        double mean ; 
        nums >> mean ; 

        _unplannedProb = new Constant(mean) ; 
    }

    else if (firstHalf == "Weibull")
    {
        double scale, shape ; 
        nums >> scale >> shape ; 

        _unplannedProb = new Weibull(scale, shape) ; 
    }

    //Determines correct distribution and prints
  //  cout << " 99(3(#((##( " ; 
  //  _unplannedProb->PrintDistribution() ; 
  //  cout << endl ; 
}


void RepairJob::AddStep(Step* step)
{
    _vecSteps.push_back(step) ; 

    //  cout << "adding step" << _vecSteps.size() << endl ; 
    step->SetStepID(_vecSteps.size()) ; 
    /*int stepID ; 
    for (int i = 0 ;i < _vecSteps.size() ;i++)
    {
        stepID = i + 1 ; 
        cout << "step id from add step is " << stepID ; 
        step->SetStepID(stepID) ; 
    }*/

    //    cout << endl << "new size " << vecSteps.size() << endl ; 
} ; 

//Step* RepairJob::GetSteps()
//{
//    for (int i = 0 ;i < _vecSteps.size ;i++)
//    {
//        return _vecSteps[i] ; 
//    }
//}

void RepairJob::AddResourceRepair(RepairJob* repairJob, string resourceName)
{
    _resourceRepairMap[resourceName] = repairJob ; 
}

void RepairJob::PrintResourceRepairs()
{
    map<string, RepairJob*>::iterator iter = _resourceRepairMap.begin() ; 

    while (iter != _resourceRepairMap.end())
    {
        cout << iter->second->GetName() << endl ; 
        iter++ ; 
    }
}

RepairJob* RepairJob::GetResourceRepair(string resourceName)
{
    map<string, RepairJob*>::iterator it = _resourceRepairMap.find(resourceName) ; 
    if (it == _resourceRepairMap.end())
        return nullptr ; 
    return it->second ; 
}

void RepairJob::PrintJobProperties()
{

    cout << "   Repair Job Name: " << _name << endl ; 
    cout << "   Schedule Type: " << _schedType << endl ; 
    cout << "   Repair Job Priority: " << _priority << endl ; 
    cout << "   Unplanned Probability: " ; 
    _unplannedProb->PrintDistribution() ; 
    cout << endl ; 
    //cout << "   Calendar Occurrence: " << _calendarDate << endl ; 
    //cout << "   Reccuring Amount: " << _recurringAmt << endl ; 
    cout << "   Indoor Requirement? " << _indoorReq << endl ; 
    cout << endl ; 

    for (int i = 0 ;i < _vecSteps.size() ;i++)
    {
        _vecSteps[i]->Print() ; 
        cout << endl ; 
        //_vecSteps[1]->PrintPools() ; 
    }
    cout << endl ; 
}

RepairJob* RepairJob::GetMyResRepairJobObj(string name)
{
    map<string, RepairJob*>::iterator it = _resourceRepairMap.find(name) ; 
    if (it == _resourceRepairMap.end())
    {
        return nullptr ; 
    }
    else
    {
        return it->second ; 
    }
}