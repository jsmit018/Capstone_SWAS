#pragma once
#include "SimObj.h"

class Warehouse
{
public:
	Warehouse(double length, double width);
	double GetLength();
	void UpdateListOfObjects(string objectName);
	void SetLength(double hangarLength);
	void SetWidth(double hangarWidth);
	double GetWidth();
private:
	double _hangarLength;
	double _hangarWidth;
	int _index;
	string _listOfObjects[];
};
