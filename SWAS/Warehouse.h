#pragma once
#include "SimObj.h"

class Warehouse
{
public:
	Warehouse(double length, double width);
	double GetLength();
	void SetLength(double hangarLength);
	void SetWidth(double hangarWidth);
	double GetWidth();
private:
	double _hangarLength;
	double _hangarWidth;
	string listOfObjects[];
};
