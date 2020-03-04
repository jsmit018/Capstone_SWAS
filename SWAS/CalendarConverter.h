#pragma once
#include <map>
#include "SimExec.h"
#include <math.h>
#include <vector>

using namespace std;

class CalConverter {
public:
	CalConverter();
	void InsertDays(int month, int numDays);
private:
	map<int, int> _monthMap;	// Map stores a number of days for each month (keys: 1-12)
};

class TimeConverter {
public:
	TimeConverter();
	void ConvertDistributionToMonthDay(Time &Month, Time &Day, Time &timeOfDay, int year, double distributionValue, int baseX,
										int baseY, int* endOfMonth);
};