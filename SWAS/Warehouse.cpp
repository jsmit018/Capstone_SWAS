//Warehouse.cpp: Jordan Smith

#include "Warehouse.h"

InputReader Warehouse::_inputReader; 
Scribe* Warehouse::_scribe = new Scribe(); 

Warehouse::Warehouse(double length, double width) {
    _hangarLength = length; 
    _hangarWidth = width; 
    _index = 0; 
}

void Warehouse::SetMasterMaps(InputReader inputReader)
{
	_inputReader = inputReader; 
}

void Warehouse::SetScribe(Scribe* scribe)
{
	_scribe = scribe; 
}

double Warehouse::GetLength()
{
    cout << "Returning Hangar Length" << endl; 
    return _hangarLength; 
}

void Warehouse::SetLength(double hangarLength)
{
    cout << "Updating Hangar length" << endl; 
    _hangarLength = hangarLength; 
}

void Warehouse::SetWidth(double warehouseWidth)
{
    cout << "Updating Hangar Width" << endl; 
    _hangarWidth = warehouseWidth; 
}

double Warehouse::GetWidth()
{
    cout << "Returning Hangar Width" << endl; 
    return _hangarWidth; 
}

StepResource* Warehouse::GetResourceInformation(string resourceName)
{
	map<string, StepResource*>::const_iterator resIter = _inputReader.GetMasterResourceMap().find(resourceName); 
	return resIter->second; 
}

void Warehouse::UpdateListOfObjects(string objectName){
    _listOfObjects[_index] = objectName; 
    _index++; 
}
