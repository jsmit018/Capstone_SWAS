#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Distribution.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Step.h"
#include "Resource.h"

class Step;

class RepairJob {
public:
	//RepairJob(string name);
	RepairJob();
	void SetName(string name);
	void SetPriority(int priority);
	void SetSchedType(string schedType);
	void SetIndoorReq(char indoorReq);
	void SetCalendarDate(string calendarDate);
	void SetRecurring(double recurringAmt);
	void SetUnplannedProb(string unplannedProb); //switch to distribution pointer
	void PrintJobProperties();

	string GetName();
	int GetPriority();
	string GetSchedType();
	char GetIndoorReq();
	string GetCalendarDate();
	double GetRecurring();
	double GetUnplannedProb(); 
	void GetNextStep();
	void AddStep(Step* step);
	void AddResourceRepair(RepairJob* repairJob, string resourceName);
	RepairJob* GetResourceRepair(string resourceName);

private:
	string	_name;									// Repair job name
	int		_priority;								// Repair job priority
	string	_schedType;								// Interarrival schedule type
	char	_indoorReq;								// Y or N for indoor requirement of repair job
	string	_calendarDate;							// Calendar-schedule type repair jobs (date: mmddyyyy)
	double	_recurringAmt;							// Number of months between Recurring-schedule type repair jobs
	string	_unplannedProb;							// Distribution for probability of certain repair job after random iat
//	Distribution* _unplannedProb;					// replace string with this
	vector<Step*> vecSteps;							// RepairJob has a vector of step objects
	static map<string, RepairJob*> _resourceRepairMap;		//FOR EACH RESOURCE TYPE, MAKE REPAIR JOB

	//int _activeStep;
	////Task** _stepList;
	//Step** _stepList;
	//RepairJob* _nextJob;
	//int _size;
};