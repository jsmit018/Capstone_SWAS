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
