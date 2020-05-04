#include "CalendarConverter.h"
#include <iostream>
#include <stdlib.h>

using namespace std; 

/**
 *CalConverter constructor
 */
CalConverter::CalConverter()
{

}

/**
 * This function inserts days into the month map and prints the map
 */
void CalConverter::InsertDays(int month, int numDays)
{
	_monthMap.insert(pair<int, int>(month, numDays)); 

	//print map
	for (map<int, int>::const_iterator it = _monthMap.begin();it != _monthMap.end();++it)
	{
		//		cout << it->first << " " << it->second << "\n"; 
	}
}

/**
 *  Returns Calendar Array
 */
int* CalConverter::GetCalArray() {
	map<int, int>::const_iterator iter = _monthMap.begin(); 
	_calArray = new int[_monthMap.size()]; 
	while (iter != _monthMap.end()) {
		_calArray[iter->first - 1] = iter->second; 
		//	cout << "Month: " << iter->first << " Days: " << iter->second << endl; 
		iter++; 

	}

	return _calArray; 
}

/**
 * Returns the Month Map
 */
map<int, int> CalConverter::GetMonthMap()
{
	return _monthMap; 
}

/**
 * TimeConverter constructor
 */
TimeConverter::TimeConverter()
{
}

/**
 * Algorithm to covert distribution value to month and day
 */
void TimeConverter::ConvertDistributionToMonthDay(Time& Month, Time& Day, Time& timeOfDay, int& year, double distributionValue,
	int baseX, int baseY, int* endOfMonth, Time sTime, int dayOrHour, int recurring, Time simTime) {
	div_t divresult;// Declaring div_t object to obtain quotient and remainder
	divresult = div(ceil(distributionValue), 24);// Divide the distribution by 24 gives an amount of days + a time)
	//If there is recurrent scheduling
	
	if (recurring == 1) {
		int remainder = 0; 
		if (baseX + ceil(distributionValue) > 11) {
			do {
				year++; 
				remainder = ((baseX + (int)distributionValue) % 12); 
			} while (remainder >= 12); 
			baseX = remainder; 
			if (baseY > endOfMonth[baseX]) {
				int rem = baseY % endOfMonth[baseX]; 
				baseY = 0 + rem; 
			}
			Month = baseX; 
			Day = baseY; 
			timeOfDay = sTime; 
		}
		else {
			baseX += (int)distributionValue; 
			Month = baseX; 
			Day = baseY; 
			timeOfDay = sTime; 
		}
	}
	else {
		if (dayOrHour == 0) {
			if (baseY + (int)divresult.quot > endOfMonth[baseX]) { // Checking to see if the added time will make it advance past a month
				int remainder = 0; 
				remainder = (baseY + divresult.quot) % endOfMonth[baseX];// Check to see how far into the next month to schedule
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
					////div_t tRemainder; 
					//tRemainder = div(divresult.rem, 24); 
					if (sTime + divresult.rem > 24) {
						int dividend; 
						do {
							if (Day + 1 > endOfMonth[baseX])
								Day = 0; 
							else
								Day++; 
							dividend = ((int)sTime + divresult.rem) % 24; 
						} while (dividend >= 24); 
						//Day += 1; 
						timeOfDay = 0 + dividend; 
					}
					else
					{
						timeOfDay = sTime + (int)divresult.rem; 
					}
				}
				else
				{
					if (baseX == 11) { //If we are in december we need to go into January and update the year
						baseX = 0; 
						year++; 
						Month = baseX; 
						Day = remainder; 
						if (sTime + divresult.rem > 24) {
							int dividend; 
							do {
								if (Day + 1 > endOfMonth[baseX])
									Day = 0; 
								else
									Day++; 
								dividend = ((int)sTime + divresult.rem) % 24; 
							} while (dividend >= 24); 
							//Day += 1; 
							timeOfDay = 0 + dividend; 
						}
						else
						{
							timeOfDay = sTime + (int)divresult.rem; 
						}
						//timeOfDay = divresult.rem; 
					}
					else { // Otherwise we just advance the month
						baseX++; 
						Month = baseX; 
						Day = remainder; 
						if (sTime + divresult.rem > 24) {
							int dividend; 
							do {
								if (Day + 1 > endOfMonth[baseX])
									Day = 0; 
								else
									Day++; 
								dividend = ((int)sTime + divresult.rem) % 24; 
							} while (dividend >= 24); 
							//Day += 1; 
							timeOfDay = 0 + dividend; 
						}
						else
						{
							timeOfDay = sTime + (int)divresult.rem; 
						}
						//timeOfDay = (int)divresult.rem; 
					}
				}
			}
			else { // If the scheduling time doesn't advance us into the next month
				Month = baseX; 
				Day = (baseY + divresult.quot); 
				if (sTime + divresult.rem >= 24) {
					int dividend; 
					do {
						if (Day + 1 > endOfMonth[baseX])
							Day = 0; 
						else
							Day++; 
						dividend = ((int)sTime + divresult.rem) % 24; 
					} while (dividend >= 24); 
					//Day += 1; 
					timeOfDay = 0 + dividend; 
				}
				else
				{
					timeOfDay = sTime + (int)divresult.rem; 
				}
				//timeOfDay = divresult.rem; 
			}
		}
		else if (dayOrHour == 1) 
		{ //This is days
			int rem = (int)distributionValue; 
			do{
				if (baseY + 1 > endOfMonth[baseX]) {
					if (baseX == 11) {
						baseX = 0; 
						year++; 
						baseY = 0; 
						rem--; 
					}
					else {
						baseX++; 
						baseY = 0; 
						rem -= 1; 
					}
				}
				else {
					baseY++; 
					rem -= 1; 
				}
			} while (rem != 0); 
			Month = baseX; 
			Day = baseY; 
			timeOfDay = sTime; 
		}
	}
}

/*CalendarObj::CalendarObj(int numEvents, Time* months, Time* days, Time* timeOfDays, int* years)
{
	for (int i = 0;i < numEvents;++i) {
		_months[i] = months[i]; 
		_days[i] = days[i]; 
		_timeOfDays[i] = timeOfDays[i]; 
		_years[i] = years[i]; 
	}
}*/

/**
 * Constructor of CalendarObj that sets _numEvents to zero
 */
CalendarObj::CalendarObj()
{
	_numEvents = 0; 
}

/**
 * Updates _numEvents by incrementing _numEvents by one
 */
void CalendarObj::UpdateNumEvents()
{
	_numEvents++; 
}

/**
 * This function returns _numEvents
 */
int CalendarObj::GetNumEvents()
{
	return _numEvents; 
}