#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Distribution.h"
//#include "Step.h"

class Step;

class RepairJob {
public:
	//RepairJob(string name);
	RepairJob();
	string GetName();
	void SetName(string name);
	void GetNextStep();
	void AddStep(Step* step, int listSpot);
private:
	string	_name;					// Repair job name
	int		_priority;				// Repair job priority
	string	_schedType;				// Interarrival schedule type
	char	_indoorReq;				// Y or N for indoor requirement of repair job
	string	_calendarSchedDate;		// Calendar-schedule type repair jobs (date: mmddyyyy)
	int		_recurSchedAmt;			// Number of months between Recurring-schedule type repair jobs
	double	_unplannedProb;			// Distribution for probability of certain repair job after random iat
	vector<string> vecSteps;		// RepairJob has a vector of step objects

	//int _activeStep;
	////Task** _stepList;
	//Step** _stepList;
	//RepairJob* _nextJob;
	//int _size;
};
