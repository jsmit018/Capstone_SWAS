#include "Warehouse.h"

//To Do:
//warehouse gui has class that when the program opens, start function (GUI initialization) is called
//start function gets values from here
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
//ties play button in GUI to run simulation (SWAS will have sleep til play function, could be trigger)
//After Sim Stops
    //notifies GUI run is done
    //calls output functions to print values

Warehouse::Warehouse(double length, double width) {
    _hangarLength = length;
    _hangarWidth = width;
    _index = 0;
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

void Warehouse::UpdateListOfObjects(string objectName){
    _listOfObjects[_index] = objectName;
    _index++;
}
