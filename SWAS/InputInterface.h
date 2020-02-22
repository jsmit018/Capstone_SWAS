//InputInterface.h: Andrea Robey
//This file is responsible for inputting data from the database into necessary formats and data structures
#pragma once
#include <map>
#include <unordered_map>
#include "Aircraft.h"
#include "RepairJob.h"
#include "Step.h"
#include "CalendarConverter.h"
#include "Distribution.h"

using namespace std;

class InputReader
{
public:
	InputReader();
	~InputReader();
	void ReadInputData();

private:
	map<Aircraft *, unordered_map<string, RepairJob>> masterRepairJobMap;		//Master list of all repair job options for each unique aircraft type
	string _seedType; //seed type read in - if statement elsewhere

};

