#pragma once
#include "SimObj.h"

class Warehouse
{
public:
	Warehouse(int length, int width);
	int GetLength();
	void SetLength(int hangarLength);
	void SetWidth(int hangarWidth);
	int GetWidth();
private:
	double _hangarLength;
	double _hangarWidth;
	string listOfObjects[];
};
