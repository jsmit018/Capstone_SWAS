#pragma once
#include "SimObj.h"
#include "Task.h"
#include "Aircraft.h"
#include "Distribution.h"
#include <iostream>

/**
* Class for DelayTask that inherits from Task
*/
class DelayBlock : public Task {
public:
	/**
	* Constructor for delay
	*/
	DelayBlock(string name, Distribution* delayTime);
	/**
	* Returns the name of the delay
	*/
	string GetName();
	/**
	* Sets the name of the delay
	*/
	void SetName(string name);
	/**
	* Schedules a delay event at a specified time based on aircraft priority
	*/
	void Execute(Aircraft* aircraft);


private:
	Distribution* _delayTime;
	string _name;
	/**
	* Schedules a start delay event method
	*/
	class StartDelayEA;
	/**
	* Schedules an end delay event method
	*/
	class EndDelayEA;
	/**
	* Schedules a finish delay event at a specified time based on aircraft priority
	*/
	void StartDelayEM(Aircraft* aircraft);
	/**
	* Aircraft depart once delay is finished
	*/
	void EndDelayEM(Aircraft* aircraft);
};