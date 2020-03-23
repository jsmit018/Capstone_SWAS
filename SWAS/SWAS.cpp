#include "SimExec.h"
#include "InputInterface.h"
#include "SourceTask.h"
#include "RepairJob.h"
#include "Resource.h"
#include "Step.h"
#include "Aircraft.h"
#include "Distribution.h"
#include "SinkTask.h"
#include <map>;

int main() {

	//Where database connection code goes when DES is ready

	InputReader inputReader;

	//populate master map
	inputReader.ReadInputData();
	inputReader.PrintEverything();

	cout << "Master Map has " << inputReader.GetMapSize() << " unique aircraft types." << endl;


	//PUT THIS ALL IN A FUNCTION:
		//Andie - HAVE CHECK FOR SELECTED MISSION/AIRCRAFT TYPES by comparison LL
		//Jordan - HANDLE FUNCTIONALITY FOR GETTING LL FROM UNITY TO DES

	//for all unique aircraft types in the master map, 
	//create the first instance of that particular aircraft type
	map<string, Aircraft*>::const_iterator iter = inputReader.GetMasterMapBegin();
	while (iter != inputReader.GetMasterMapEnd())
	{
		cout << "Creating first instance of: " << iter->first << endl;
		cout << endl;
		//Instantiate an aircraft object for it
		Aircraft* firstAircraft = new Aircraft();

		//Look up all of that particular aircraft type's repair jobs (and associated steps, 
		////resources, parts, etc.) from the master map and copy them to this new aircraft's list 
		firstAircraft->GetMyJobList(iter->first);








		//then schedule the next random arrival, recurring arrival and the calendar arrivals if they have them
			//SourceBlock* unplanArrival = new SourceBlock(/*iat distr and firstAircraft*/);
			//SourceBlock* recurArrival = new SourceBlock(/*recur distr and firstAircraft*/);
			//SourceBlock* calArrival = new SourceBlock(/*date and firstAircraft*/);
		
		//for bool aremoresteps == true (false if no more steps in vector or rjs in myMap)
	//	{	
			// getNextStep() keeps getting the next step til all steps in RJ are done,
				//	can be made to take argument of RJ type (unplan,etc) then get next 
				//	RJ of same type's first step 
			//Step* nextUnplanStep = this aircraft's getNextStep(unplan); 
			//unplanArrival->SetNextTask(nextUnplanStep)		

			//Step* nextRecurStep = this aircraft's getNextStep(recur); 
			//recurArrival->SetNextTask(nextRecurStep)		

			//Step* nextCalStep = this aircraft's getNextStep(cal); 
			//calArrival->SetNextTask(nextCalStep)	
		//}

			//nextUnplan->setnexttask(sink)
		//next is setting next task is departure task
		iter++;
	}
	
	cout << "reading is finished" << endl;

	/*
		SimExec::InitializeSimulation(11, /CalendarConverterDaysArrayHere/);
	*/

	/*
		SimExec::RunSimulation() || SimExec::RunSimulation(/endTime/);
		//Print Values here
	*/

	return 0;
}