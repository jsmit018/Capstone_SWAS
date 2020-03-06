#include "CalendarConverter.h"
#include <iostream>
#include <stdlib.h>

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

void TimeConverter::ConvertDistributionToMonthDay(Time &Month, Time &Day, Time &timeOfDay, int &year, double distributionValue,
	int baseX, int baseY, int* endOfMonth) {
	div_t divresult; // Declaring div_t object to obtain quotient and remainder
	divresult = div(ceil(distributionValue), 24); // Divide the distribution by 24 gives an amount of days + a time)
	//divresult = div(distributionValue, 24);
	if (baseY + (int)divresult.quot > endOfMonth[baseX]) { // Checking to see if the added time will make it advance past a month
		int remainder = 0;
		remainder = (baseY + divresult.quot) % endOfMonth[baseX]; // Check to see how far into the next month to schedule
		if (baseX == 11){ //If we are in december we need to go into January and update the year
			baseX = 0;
			year++;
			Month = baseX;
			Day = remainder;
			timeOfDay = divresult.rem;
		}
		else{ // Otherwise we just advance the month
			baseX++;
			Month = baseX;
			Day = remainder;
			timeOfDay = (int)divresult.rem;
		}
	}
	else{ // If the scheduling time doesn't advance us into the next month
		Month = baseX;
		Day = (baseY + divresult.quot);
		timeOfDay = divresult.rem;
	}
}

CalendarObj::CalendarObj(int numEvents, Time* months, Time* days, Time* timeOfDays, int* years)
{
	for (int i = 0; i < numEvents; ++i) {
		_months[i] = months[i];
		_days[i] = days[i];
		_timeOfDays[i] = timeOfDays[i];
		_years[i] = years[i];
	}
}
