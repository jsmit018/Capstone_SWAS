#include "CalendarConverter.h"
#include <iostream>

using namespace std;

CalConverter::CalConverter()
{

}

void CalConverter::InsertDays(int month, int numDays)
{
	_monthMap.insert(pair<int, int>(month, numDays));

	//print map
	for (map<int, int>::const_iterator it = _monthMap.begin(); it != _monthMap.end(); ++it)
	{
//		cout << it->first << " " << it->second << "\n";
	}
}

TimeConverter::TimeConverter() {

}

void TimeConverter::ConvertDistributionToMonthDayHour(double timeToConvert, Time& month, Time& day, Time& timeOfDay)
{
}
