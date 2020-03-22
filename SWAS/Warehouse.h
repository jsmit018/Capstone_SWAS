#pragma once
#include "SimObj.h"
#include "InputInterface.h"

class Warehouse
{
public:
	Warehouse(double length, double width);
	static void SetMasterMaps(InputReader inputReader);
	double GetLength();
	void UpdateListOfObjects(string objectName);
	void SetLength(double hangarLength);
	void SetWidth(double hangarWidth);
	double GetWidth();

	Resource* GetResourceInformation(string resourceName);
private:
	double _hangarLength;
	double _hangarWidth;
	int _index;
	static InputReader _inputReader;
	string _listOfObjects[];
};
