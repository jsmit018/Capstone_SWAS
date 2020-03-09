#include "SimExec.h"
#include "InputInterface.h"


int main() {

	//Where database connection code goes when DES is ready


	InputReader inputReader;
	inputReader.ReadInputData();
//	inputReader.PrintEverything();
	cout << "reading is finished" << endl;

	/*
		SimExec::InitializeSimulation(11, /CalendarConverterDaysArrayHere/);
	*/

	/*
		SimExec::WaitForTrigger(/From Warehouse/)
		SimExec::RunSimulation() || SimExec::RunSimulation(/endTime/);
		Warehouse::Trigger();
		//Print Values here
	*/

	return 0;
}