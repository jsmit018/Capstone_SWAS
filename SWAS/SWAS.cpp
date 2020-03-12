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
	inputReader.ReadInputData();
	inputReader.PrintEverything();
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