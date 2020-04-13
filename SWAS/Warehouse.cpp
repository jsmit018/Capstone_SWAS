#include "Warehouse.h"

//To Do:
//warehouse gui has class that when the program opens, start function (GUI initialization) is called
//start function gets values from here [DONE] -- Gets all of the relevant information from the master maps.
    //these values are: 
        //resources
            //footprint
            //name
            //count
        //parts
            //name
            //count
        //extra items (i.e. office space, parts storage)
            //footprint
            //name
            //count
//ties play button in GUI to run simulation (SWAS will have sleep til play function, could be trigger) [DONE]
//***SimExec hanldes this through PlayButton function in simulation executive that runs the simulation.
//After Sim Stops
    //notifies GUI run is done [DONE]
	//***SimExec does this by returning a Flag 0 for sim is still running returns a 3 if the simulation is terminated.
    //calls output functions to print values [ ]

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

Resource* Warehouse::GetResourceInformation(string resourceName)
{
	map<string, Resource*>::const_iterator resIter = _inputReader.GetMasterResourceMap().find(resourceName);
	return resIter->second;
}

void Warehouse::UpdateListOfObjects(string objectName){
    _listOfObjects[_index] = objectName;
    _index++;
}
