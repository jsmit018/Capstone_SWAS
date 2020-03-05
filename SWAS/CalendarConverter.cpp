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
/*
//Decimal is put first, integer or whole number is put second
or 
/>>> a = 1234.5678
>>> (lambda x, y: (int(x), int(x*y) % y/y))(a, 1e0)
(1234, 0.0)
>>> (lambda x, y: (int(x), int(x*y) % y/y))(a, 1e1)
(1234, 0.5)
>>> (lambda x, y: (int(x), int(x*y) % y/y))(a, 1e15)
(1234, 0.5678) /
 x, y = math.modf();
*/

TimeConverter::TimeConverter()
{
}

void TimeConverter::ConvertDistributionToMonthDay(Time &Month, Time &Day, Time &timeOfDay, int year, double distributionValue,
	int baseX, int baseY, int* endOfMonth) {
	//double decimal = 0;
	//int dayInteger = 0;
	int hourInteger = ceil(distributionValue);
	int remainder = hourInteger % 24;
	//decimal, dayInteger = modf(distributionValue);
	//dayInteger *= 24; // 24 hours in a day, to determine how many days in advance to schedule
	//decimal = ceil(decimal * 24); // Converting the decimal portion to a specefic time of day
	if (baseY + dayInteger > endOfMonth[baseX]) {
		int remainder = 0;
		remainder = (baseY + dayInteger) % endOfMonth[baseX];
	}
}
