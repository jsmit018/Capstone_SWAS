#pragma once
#include <map>
#include <vector>

using namespace std;

class CalConverter {
public:
	CalConverter();
	void InsertDays(int month, int numDays);
private:
	map<int, int> _monthMap;	// Map stores a number of days for each month (keys: 1-12)
};