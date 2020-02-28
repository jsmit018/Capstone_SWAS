#include "Warehouse.h"

Warehouse::Warehouse(double length, double width) {
    _hangarLength = length;
    _hangarWidth = width;
}

double Warehouse::GetLength()
{
    return _hangarLength;
}

void Warehouse::SetLength(double hangarLength)
{
    _hangarLength = hangarLength;
}

void Warehouse::SetWidth(double warehouseWidth)
{
    _hangarWidth = warehouseWidth;
}

double Warehouse::GetWidth()
{
    return _hangarWidth;
}