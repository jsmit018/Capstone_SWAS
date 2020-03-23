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
	static int* GetCalArray();
	static map<int, int> GetMonthMap();
private:
	static map<int, int> _monthMap;	// Map stores a number of days for each month (keys: 1-12)
	static int* _calArray;
};

class TimeConverter {
public:
	TimeConverter();
	static void ConvertDistributionToMonthDay(Time& Month, Time& Day, Time& timeOfDay, int& year, double distributionValue, int baseX,
		int baseY, int* endOfMonth, int recurring = 0, Time simTime = 0);
};

class CalendarObj {
public:
	//CalendarObj(int numEvents, Time* months, Time* days, Time* timeOfDays, int* years);
	CalendarObj();
	void UpdateNumEvents();
	int GetNumEvents();
	std::vector<Time> _months;
	std::vector<Time> _days;
	std::vector<Time> _timeOfDays;
	std::vector<int> _year;
private:
	int _numEvents;
};