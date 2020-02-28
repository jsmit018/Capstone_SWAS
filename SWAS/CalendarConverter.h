#pragma once
#include <map>
#include <vector>
#include "SimObj.h"

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
	static void ConvertDistributionToMonthDayHour(double timeToConvert, Time &month, Time &day, Time &timeOfDay);
};