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

int* CalConverter::GetCalArray() {
	map<int, int>::const_iterator iter = _monthMap.begin();
	_calArray = new int[_monthMap.size()];
	while (iter != _monthMap.end()) {
		_calArray[iter->first - 1] = iter->second;
		iter++;
	}

	return _calArray;
}

map<int, int> CalConverter::GetMonthMap()
{
	return _monthMap;
}

TimeConverter::TimeConverter()
{
}

void TimeConverter::ConvertDistributionToMonthDay(Time& Month, Time& Day, Time& timeOfDay, int& year, double distributionValue,
	int baseX, int baseY, int* endOfMonth, int recurring, Time simTime) {
	div_t divresult; // Declaring div_t object to obtain quotient and remainder
	divresult = div(ceil(distributionValue), 24); // Divide the distribution by 24 gives an amount of days + a time)
	//If there is recurrent scheduling
	if (recurring == 1) {
		//int remainder = 0;
		if (baseX + ceil(distributionValue) > 11) {
			baseX = ((baseX + (int)distributionValue) % 12);
			if (baseY > endOfMonth[baseX])
				baseY = 0;
			Month = baseX;
			Day = baseY;
			timeOfDay = simTime;
		}
		else {
			baseX += (int)distributionValue;
			Month = baseX;
			Day = baseY;
			timeOfDay = simTime;
		}
	}
	else {
		if (baseY + (int)divresult.quot > endOfMonth[baseX]) { // Checking to see if the added time will make it advance past a month
			int remainder = 0;
			remainder = (baseY + divresult.quot) % endOfMonth[baseX]; // Check to see how far into the next month to schedule
			if (baseY + divresult.quot % endOfMonth[baseX] > (endOfMonth[baseX] + endOfMonth[baseX + 1])) {
				while (remainder > endOfMonth[baseX]) {
					if (baseX == 11) {
						baseX = 0;
						year++;
					}

					else
						baseX++;
					remainder %= endOfMonth[baseX];
				}
				Month = baseX;
				Day = remainder;
				timeOfDay = divresult.rem;
			}
			else
			{
				if (baseX == 11) { //If we are in december we need to go into January and update the year
					baseX = 0;
					year++;
					Month = baseX;
					Day = remainder;
					timeOfDay = divresult.rem;
				}
				else { // Otherwise we just advance the month
					baseX++;
					Month = baseX;
					Day = remainder;
					timeOfDay = (int)divresult.rem;
				}
			}
		}
		else { // If the scheduling time doesn't advance us into the next month
			Month = baseX;
			Day = (baseY + divresult.quot);
			timeOfDay = divresult.rem;
		}
	}
}

/*CalendarObj::CalendarObj(int numEvents, Time* months, Time* days, Time* timeOfDays, int* years)
{
	for (int i = 0; i < numEvents; ++i) {
		_months[i] = months[i];
		_days[i] = days[i];
		_timeOfDays[i] = timeOfDays[i];
		_years[i] = years[i];
	}
}*/

CalendarObj::CalendarObj()
{
	_numEvents = 0;
}

void CalendarObj::UpdateNumEvents()
{
	_numEvents++;
}

int CalendarObj::GetNumEvents()
{
	return _numEvents;
}
