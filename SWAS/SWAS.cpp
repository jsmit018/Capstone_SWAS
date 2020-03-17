#include "SimExec.h"
#include "InputInterface.h"
#include "SourceTask.h"
#include "RepairJob.h"
#include "Resource.h"
#include "Step.h"
#include "Aircraft.h"
#include "Distribution.h"
#include "SinkTask.h"

int main() {

	//Where database connection code goes when DES is ready


	InputReader inputReader;

	//populate master map
	inputReader.ReadInputData();
	inputReader.PrintEverything();

	//for all unique aircraft types in the master map, create the first aircraft
	map<string, Aircraft*>::const_iterator iter = inputReader.GetMasterMapBegin();
	while (iter != inputReader.GetMasterMapEnd())
	{
		cout << "Master Map size: " << inputReader.GetMapSize() << endl;

		cout << "Creating first " << iter->first << endl;

		Aircraft* firstAircraft = new Aircraft();
		firstAircraft->GetMyJobList(iter->first);
		//Mayhaps
		if (iter->first == "F-35"){
			//SourceBlock* F_35 = new (new Constant(inputReader.GetRecurring()), it->first, it->second,
				//it->first + " Source Object", #, /**/);
		}
		else if (iter->first == "F-18"){
			//SourceBlock* F_18 = new (new Constant(inputReader.GetRecurring()), it->first, it->second,
				//it->first + " Source Object", #, /**/);
		}
		else if (iter->first == "Fixed Wing"){
			//SourceBlock* Fixed_Wing = new (new Constant(inputReader.GetRecurring()), it->first, it->second,
				//it->first + " Source Object", #, /**/);
		}
		else if (iter->first == "Apache"){
			//SourceBlock* Apache = new (new Constant(inputReader.GetRecurring()), it->first, it->second,
				//it->first + " Source Object", #, /**/);	
		}
	}
	cout << "reading is finished" << endl;

	/*
		SimExec::InitializeSimulation(11, /CalendarConverterDaysArrayHere/);
	*/
	
	/*
		@TODO Insantiate Objects here from inputReader
		Step* SystemMaintenance = new(inputReader.GetServiceTime(), "Warehouse Maintenance System");
		Sink* SystemSink = new("System Sink");
		
	*/
	/*
		Source Objects Next Task = Step
		Step Next Task = Sink
		SystemMaintenance->SetNextTask(SystemSink);
	/*
	
	/*
		SimExec::RunSimulation() || SimExec::RunSimulation(/endTime/);
		//Print Values here
	*/

	return 0;
}
