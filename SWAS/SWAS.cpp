#include "SimExec.h"
#include "InputInterface.h"
#include "SourceTask.h"
#include "RepairJob.h"
#include "Resource.h"
#include "Step.h"
#include "Aircraft.h"
#include "Distribution.h"
#include "SinkTask.h"
#include "Warehouse.h"
#include "Scribe.h"
#include <map>

using namespace std;

void ScribeSetDimension(double length, double width) {
	Scribe::SetWarehousDims(length, width);
}

void ScribeAdvanceRun() {
	Scribe::AdvanceRun();
}

void ScribeSetTerminationTime(double termTime) {
	Scribe::SetRunTime(termTime);
}

//had to make global to isolate ReadInputData() so that it's not repeated in multiple runs.
InputReader inputReader;


///Jordan: 
void SchedResourceFailure()
{
	//schedule resource failure logic
	map<string, Resource*>::const_iterator iter = InputReader::GetMasterResMapBegin();
	while (iter != InputReader::GetMasterResMapEnd())
	{
		cout << "" << endl;
		//schedule iter's first failure in iter->second->GetFailureDistr()
		if (iter->first == "S Bay" || iter->first == "M Bay" || iter->first == "L Bay") {
		//if (iter->second->GetFailureName() == 		
			//continue;
		}
		else
		{
			//iter->second->ScheduleFirstFailures(iter->second);
			//iter++;
		}
		iter++;
	}

}

void InitializeAircraft()
{
	//	InputReader inputReader;

	/*Populates master map*/
	//	inputReader.ReadInputData();
	//inputReader.PrintEverything();
	cout << "reading is finished" << endl;

	//SimExec::SetInputReader(inputReader);
	SimExec::InitializeSimulation(inputReader.GetCalConverter()->GetMonthMap().size(), inputReader.GetCalConverter()->GetCalArray());
	//Setting the Initial System Seed I just picked 8 b/c of the team size
	Distribution::SetSystemSeed(8);
	inputReader.AddSelectedAircraft("F-35");
	inputReader.AddSelectedAircraft("F-18");
	inputReader.AddSelectedAircraft("Apache");

	SinkBlock* depart = new SinkBlock("SWAS System Sink");

	SimExec::SetSystemSink(depart);

	//cout << "Master Map has " << inputReader.GetMapSize() << " unique aircraft types." << endl;

	//for all unique aircraft types in the master map 
	map<string, Aircraft*>::const_iterator iter = inputReader.GetMasterMapBegin();
	while (iter != inputReader.GetMasterMapEnd())
	{
		/*If the current aircraft matches one in the linked list, create instance etc.*/
		//if (search(head, iter->first) == true)
		if (inputReader.FindSelectedAircraft(iter->first) == true)
		{
			//iter->second->PrintProperties(); 
			//iter->second->GetAircraftIAT()->PrintDistribution();

			/* Create the first instance of that particular aircraft type by copying from master map */
			//Test count//
			int count = 1;
			//____________
			Aircraft* firstAircraft = new Aircraft(*iter->second);
			//		cout << "Creating first instance of " << firstAircraft->GetAircraftType() << " for copying purposes" << endl;

			firstAircraft->CopyMyJobList(iter->first);

			map<string, RepairJob*>::const_iterator myIter = firstAircraft->GetMyRJMapBegin();

			while (myIter != firstAircraft->GetMyRJMapEnd())
			{
				if (myIter->second->GetSchedType() == "Calendar")
				{
					////// calendarsourceblock schedules calendar arrival at date 
					//(sourceblock schedules arrival, arrival happens once)
					//cout << endl;
					cout << "Scheduling calendar arrival for " << firstAircraft->GetAircraftType() << endl;
					cout << endl;
					SourceBlock* calArrival = new SourceBlock(
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Calendar Arrival",
						firstAircraft->GetCalendarObj(),
						myIter->second);
				}

				else if (myIter->second->GetSchedType() == "Recurring")
				{
					////// recurringsourceblock schedules first arrival at recur iat 
					//(sourceblock schedules arrival, arrival schedules next arrival)
					cout << endl;
					cout << "Scheduling recurring arrival for " << firstAircraft->GetAircraftType() << endl;
					cout << endl;
					SourceBlock* recurArrival = new SourceBlock(
						firstAircraft->GetRecurIatMap(), //get a map -- The map is set up as <string, RepairJob*> we can pass the repair job along this way << this is not true
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Recurring Arrival",
						10);
				}

				else if (myIter->second->GetSchedType() == "Unplanned" && count == 1)
				{
					////// unplannedsourceblock schedules first arrival at unpl iat  
					//(sourceblock schedules arrival, arrival schedules next arrival
					//cout << "Scheduling first unplanned arrival for " << firstAircraft->GetAircraftType() << endl;
					//cout << "Scheduling " << count << " unplanned arrival for " << firstAircraft->GetAircraftType() << endl;


					SourceBlock* unplanArrival = new SourceBlock(
						firstAircraft->GetAircraftIAT(),
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Unplanned Arrival",
						myIter->second,
						10);
					count++;

				}

				myIter++;
			}

		}

		iter++;
	}

	Warehouse::SetMasterMaps(inputReader);


}


int main()
{
	inputReader.ReadInputData();

	Scribe::SetSaveFile("Output.csv");
	//Step::PrintPools();
	/*For handling multiple runs -- currently set as 1 in file for testing purposes*/
	//*Note: Let tyler know this function name so he can add it to his unity logic
	for (int i = 0; i < inputReader.GetNumRuns(); i++)
	{

		if (i > 0)
			ScribeAdvanceRun();

		InitializeAircraft();
		//SchedResourceFailure();
		//InitalizeAircraft(GetScribe());

		///Included for simulation testing purposes -> will be moved during GUI integration
		while (SimExec::GetSimulationFlag())
			SimExec::RunSimulation(0, 0, 2045);

		cout << endl << endl << endl;

		cout << SimExec::PrintNumInCondES() << endl;

		//For Kevin, this causes an infinite loop
		ScribeSetTerminationTime(SimExec::GetTotalSimulationTime());

	}

	Scribe::Archive();

	

	return 0;
}