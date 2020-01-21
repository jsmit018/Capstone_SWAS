#include "warehouse.h"

Warehouse::Warehouse(int length, width){
  _warehouseLength = length;
  _warehouseWidth = width;
}

int Warehouse::GetLength()
{
    return _warehouseLength;
}

void Warehouse::SetLength(int warehouseLength)
{
   _warehouseLength = warehouseLength;
}

void Warehouse::SetWidth(int warehouseWidth)
{
  _warehouseWidth = warehouseWidth;
}

int Warehouse::GetWidth()
{
  _return _warehouseWidth;
}
