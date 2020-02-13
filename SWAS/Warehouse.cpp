#include "Warehouse.h"

Warehouse::Warehouse(int length, int width) {
    _hangarLength = length;
    _hangarWidth = width;
}

int Warehouse::GetLength()
{
    return _hangarLength;
}

void Warehouse::SetLength(int hangarLength)
{
    _hangarLength = hangarLength;
}

void Warehouse::SetWidth(int warehouseWidth)
{
    _hangarWidth = warehouseWidth;
}

int Warehouse::GetWidth()
{
    return _hangarWidth;
}
