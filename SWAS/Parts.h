//Parts.h: Andrea Robey

#pragma once
#include "Distribution.h"
#include <string>
#include <sstream>

class Parts
{
public:
	Parts(); 
	void CopyMapParts(const Parts& mapParts); 
	void AcquireParts(int numNeeded); 
	bool AreEnoughParts(); 
	void PrintPartsProperties(); 

	void SetPartsCount(int partsCount); 
	void SetInitPartsCount(int partsCount); 
	void SetNumPartsNeeded(int numNeeded); 
	void SetPartsName(string partsName); 
	void SetThreshold(int orderThreshold); 
	void SetLeadTime(string leadTime); 

	int GetPartsCount(); 
	int GetInitPartsCount(); 
	int GetNumPartsNeeded(); 
	Distribution* GetLeadTime(); 
	string GetPartsName(); 
	int GetThreshold(); 

private:
	int _initCount; 
	int _partsCount; 
	int _numNeeded; 
	string _partsName; 
	int _orderThreshold; 
	Distribution* _leadTime; 

}; 