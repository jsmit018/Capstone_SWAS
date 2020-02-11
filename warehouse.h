#pragma once
#include "SimObj.h"

class Warehouse
{
public:
	Warehouse();
	void GetLength();
	void SetLength();
	void SetWidth();
	void GetWidth();
private:
	double hangarLength;
	double hangarWidth;
	string listOfObjects[];
};
