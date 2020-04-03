#pragma once
#include "Task.h"
//#include "SimObj.h"
#include "SimExec.h"

/**
* Class for SinkTask that inherits from Task
*/
class SinkBlock : public Task {
public:
	/**
	* Constructor for sink
	*/
	SinkBlock(string name);
	/**
	* Deletes aircraft
	*/
	void Execute(Aircraft* aircraft);
	/**
	* Returns the sink name
	*/
	string GetName();
	/**
	* Sets the sink name
	*/
	void SetName(string name);
	/**
	* Returns the terminated aircraft
	*/
	int GetTerminatedAircraft();
private:
	string _name;
	int _terminatedAircraft;
};