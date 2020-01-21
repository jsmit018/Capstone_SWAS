#pragma once
#include "SimObj.h"

class warehouse
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
