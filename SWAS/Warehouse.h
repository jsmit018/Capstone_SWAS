#pragma once
#include "SimObj.h"
#include "InputInterface.h"

class Warehouse
{
public:
	Warehouse(double length, double width);
	static void SetMasterMaps(InputReader inputReader);
	static void SetScribe(Scribe* scribe);
	double GetLength();
	void UpdateListOfObjects(string objectName);
	void SetLength(double hangarLength);
	void SetWidth(double hangarWidth);
	double GetWidth();

	StepResource* GetResourceInformation(string resourceName);
private:
	double _hangarLength;
	double _hangarWidth;
	int _index;
	static InputReader _inputReader;
	static Scribe* _scribe;
	string _listOfObjects[];
};