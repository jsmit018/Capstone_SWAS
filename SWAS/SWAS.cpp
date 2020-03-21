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
	SourceBlock* F_35 = NULL;
	SourceBlock* F_18 = NULL;
	SourceBlock* Fixed_Wing = NULL;
	SourceBlock* Apache = NULL;


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

		//Aircraft* firstAircraft = new Aircraft();
		//firstAircraft->GetMyJobList(iter->first);
		//Mayhaps
		if (iter->first == "F-35") {
			map<string, RepairJob*>::const_iterator it = InputReader::GetAircraft(iter->first)->GetRJMapBegin();
			F_35 = new SourceBlock(new Constant(it->second->GetRecurring()), iter->first, iter->second, iter->first + " Source Object", inputReader.GetF35Calendar()->GetNumEvents(), inputReader.GetF35Calendar());
		}
		else if (iter->first == "F-18") {
			map<string, RepairJob*>::const_iterator it = InputReader::GetAircraft(iter->first)->GetRJMapBegin();
			F_18 = new SourceBlock(new Constant(it->second->GetRecurring()), iter->first, iter->second, iter->first + " Source Object", inputReader.GetF18Calendar()->GetNumEvents(), inputReader.GetF18Calendar());
		}
		else if (iter->first == "Fixed Wing") {
			map<string, RepairJob*>::const_iterator it = InputReader::GetAircraft(iter->first)->GetRJMapBegin();
			Fixed_Wing = new SourceBlock(new Constant(it->second->GetRecurring()), iter->first, iter->second, iter->first + " Source Object", inputReader.GetFixedWingCalendar()->GetNumEvents(), inputReader.GetFixedWingCalendar());
		}
		else if (iter->first == "Apache") {
			map<string, RepairJob*>::const_iterator it = InputReader::GetAircraft(iter->first)->GetRJMapBegin();
			Apache = new SourceBlock(new Constant(it->second->GetRecurring()), iter->first, iter->second, iter->first + " Source Object", inputReader.GetApacheCalendar()->GetNumEvents(), inputReader.GetApacheCalendar());
		}
	}
	cout << "reading is finished" << endl;

	SimExec::InitializeSimulation(inputReader.GetCalConverter()->GetMonthMap().size(), inputReader.GetCalConverter()->GetCalArray());

	//@TODO Insantiate Objects here
	Step* SystemMaintenance = new Step("5", "Warehouse Maintenance System");
	SinkBlock* SystemSink = new SinkBlock("System Sink");

	//Source Objects Next Task = Step
	//Step Next Task = Sink
	F_35->SetNextTask(SystemMaintenance);
	F_18->SetNextTask(SystemMaintenance);
	Fixed_Wing->SetNextTask(SystemMaintenance);
	Apache->SetNextTask(SystemMaintenance);
	SystemMaintenance->SetNextTask(SystemSink);

	//SimExec::RunSimulation();
	//SimExec::RunSimulation(/endTime/);
	//Print Values here
	return 0;
}
