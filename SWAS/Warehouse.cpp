#include "Warehouse.h"

Warehouse::Warehouse(double length, double width) {
    _hangarLength = length;
    _hangarWidth = width;
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
