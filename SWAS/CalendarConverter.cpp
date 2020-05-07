//CalendarConverter.cpp: Jordan Smith

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

					if (sTime + divresult.rem > 24) {
						int dividend; 
						do {
							if (Day + 1 > endOfMonth[baseX])
								Day = 0; 
							else
								Day++; 
							dividend = ((int)sTime + divresult.rem) % 24; 
						} while (dividend >= 24); 
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
							timeOfDay = 0 + dividend; 
						}
						else
						{
							timeOfDay = sTime + (int)divresult.rem; 
						}
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
							timeOfDay = 0 + dividend; 
						}
						else
						{
							timeOfDay = sTime + (int)divresult.rem; 
						}
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
					timeOfDay = 0 + dividend; 
				}
				else
				{
					timeOfDay = sTime + (int)divresult.rem; 
				}
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