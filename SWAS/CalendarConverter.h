#pragma once
#include <map>
#include "SimExec.h"
#include <math.h>
#include <vector>

using namespace std;

/**
 * Base class for calendar converter to be inherited by all other classes
 */
class CalConverter {
public:
	CalConverter();
	void InsertDays(int month, int numDays);
	int* GetCalArray();
	map<int, int> GetMonthMap();
private:
/**
 * Map stores a number of days for each month (keys: 1-12)
 */
	map<int, int> _monthMap;
	int* _calArray;
};

/**
 * This class is the time converter to Month Day
 */
class TimeConverter {
public:
	TimeConverter();
	static void ConvertDistributionToMonthDay(Time& Month, Time& Day, Time& timeOfDay, int& year, double distributionValue, int baseX,
		int baseY, int* endOfMonth, int recurring = 0, Time simTime = 0);
};

/**
 * class CalendarObj(int numEvents, Time* months, Time* days, Time* timeOfDays, int* years);
 */
class CalendarObj {
public:
   
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