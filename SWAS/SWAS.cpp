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
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <map>
#include <time.h>

using namespace std;

/**
 * Function for GUI, allows the setting of dimensions of warehouse into database
 */
void ScribeSetDimension(double length, double width) {
	Scribe::SetWarehousDims(length, width);
}

/**
 * Function that advances simulation run to the next for tables
 */
void ScribeAdvanceRun() {
	Scribe::AdvanceRun();
}

/**
 * Function that sets the time at which the simulation terminated
 */
void ScribeSetTerminationTime(double termTime) {
	Scribe::SetRunTime(termTime);
}

/**
 * Read Resources
 */
void ScribeReadResources() {
	Step::ReadResources();
}

/**
 * Global Variable so that information is not duplicated
 */
//had to make global to isolate ReadInputData() so that it's not repeated in multiple runs.
InputReader inputReader;

/**
 * Schedules the initial recurring resource failures for the duration of the simulation
 */
void SchedResourceFailure()
{
	//cout << " in sched resource fail " << endl;
	//schedule resource failure logic
	map<string, StepResource*>::const_iterator iter = InputReader::GetMasterResMapBegin();
	while (iter != InputReader::GetMasterResMapEnd())
	{
		//cout << "" << endl;
		if (iter->second->GetFailureName() != "")
		//schedule iter's first failure in iter->second->GetFailureDistr()
		iter->second->ScheduleFirstFailures(iter->second);
		iter++;
	}
}

/**
 * Reads in information from the SQL database, intializes then simulation executive
 * and based on the Associated Aircraft that have been selected it then schedules the
 * one unplanned repair job with the highest priority, each indivdual calendar repair job,
 * and the Recurring RepairJobs
 */
void InitializeAircraft()
{
	cout << "reading is finished" << endl;

	//SimExec::SetInputReader(inputReader);
	SimExec::InitializeSimulation(inputReader.GetCalConverter()->GetMonthMap().size(), inputReader.GetCalConverter()->GetCalArray());
	//Setting the Initial System Seed I just picked 8 b/c of the team size

	//inputReader.AddSelectedAircraft("F-35");
	inputReader.AddSelectedAircraft(1);
	//inputReader.AddSelectedAircraft("F-18");
	inputReader.AddSelectedAircraft(2);
	//inputReader.AddSelectedAircraft("Apache");
	inputReader.AddSelectedAircraft(3);

	SinkBlock* depart = new SinkBlock("SWAS System Sink");

	SimExec::SetSystemSink(depart);

	//for all unique aircraft types in the master map 
	map<string, Aircraft*>::const_iterator iter = inputReader.GetMasterMapBegin();
	while (iter != inputReader.GetMasterMapEnd())
	{
		/*If the current aircraft matches one in the linked list, create instance etc.*/
		//if (search(head, iter->first) == true)
		if (inputReader.FindSelectedAircraft(iter->first) == true)
		{
			/* Create the first instance of that particular aircraft type by copying from master map */
			//Test count//
			int count = 1;
			int cal = 1;
			//____________
			Aircraft* firstAircraft = new Aircraft(*iter->second);
			//cout << "Creating first instance of " << firstAircraft->GetAircraftType() << " for copying purposes" << endl;

			firstAircraft->CopyMyJobList(iter->first);

			map<string, RepairJob*>::const_iterator myIter = firstAircraft->GetMyRJMapBegin();

			while (myIter != firstAircraft->GetMyRJMapEnd())
			{
				if (myIter->second->GetSchedType() == "Calendar" && cal == 1)
				{
					////// calendarsourceblock schedules calendar arrival at date 
					//cout << "Scheduling calendar arrival for " << firstAircraft->GetAircraftType() << endl;
					cout << firstAircraft->GetAircraftType() << " ";
					firstAircraft->GetCalendarObj()->_months;
					firstAircraft->GetCalendarObj()->_days;
					cout << endl;
					cout << endl;
					SourceBlock* calArrival = new SourceBlock(
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Calendar Arrival",
						firstAircraft->GetCalendarObj(),
						myIter->second);
					cal++;
				}

				else if (myIter->second->GetSchedType() == "Recurring")
				{
					////// recurringsourceblock schedules first arrival at recur iat 
					cout << endl;
					//cout << "Scheduling recurring arrival for " << firstAircraft->GetAircraftType() << endl;
					cout << endl;
					SourceBlock* recurArrival = new SourceBlock(
						firstAircraft->GetRecurIatMap(), //get a map -- The map is set up as <string, RepairJob*> we can pass the repair job along this way << this is not true
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Recurring Arrival"/*,
						15*/);
				}

				else if (myIter->second->GetSchedType() == "Unplanned" && count == 1)
				{
					////// unplannedsourceblock schedules first arrival at unpl iat  

					SourceBlock* unplanArrival = new SourceBlock(
						firstAircraft->GetAircraftIAT(),
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Unplanned Arrival",
						myIter->second/*,
						15*/);
					count++;

				}

				myIter++;
			}

		}

		iter++;
	}

	Warehouse::SetMasterMaps(inputReader);


}

/**
 * Schedules the initialization of shift changes at the beginning of the simulation
 */
void ScheduleFirstShiftChange()
{

	//cout << "in schedule first shift change()" << endl;
	//if wartime, schedule at 0600 and 1800
	if (InputReader::IsWartime() == true)
	{
		//cout << "scheduling first wartime shift" << endl;
		Step::ScheduleFirstWartimeShifts();
		return;
	}
	//else if peacetime, schedule at 0600, 1400, and 2200
	else
	{
		Step::ScheduleFirstPeacetimeShifts();
		return;
	}
}

/**
 * Main: Links to the SQL Database, calls InitializeAircraft, ScheduleResourceFailures, and ScheduleFirstShiftChange(),
 * It compares and evaluates the system seeds and updates this information as necessary
 * Runs the simulation, and then archives all of the relevant simulation information at the end of the Simulation run.
 */
int main()
{
	//////////////////////////////////////
	///// CONNECTING TO THE DATABASE /////
	//////////////////////////////////////

	/////////////////////////////////////

	inputReader.ReadInputData();

	Scribe::SetSaveFile("Output.csv");
	Scribe::SetRuns(inputReader.GetNumRuns());
	Scribe::SetNumRuns(inputReader.GetNumRuns());
	//Step::PrintPools();
	inputReader.PrintEverything();

	/*For handling multiple runs -- currently set as 1 in file for testing purposes*/
	//*Note: Let tyler know this function name so he can add it to his unity logic
	for (int i = 0; i < inputReader.GetNumRuns(); i++)
	{

		if (i > 0) {
			ScribeAdvanceRun();
			ScribeReadResources();
		}

		InitializeAircraft();
		SchedResourceFailure();
		ScheduleFirstShiftChange();

		//InitalizeAircraft(GetScribe());
		
		//If System Seed is the same vs. Different --If Random Generate a new system seed. --If the same don't worry about it
		if (Distribution::GetSystemSeedType() == "random" || Distribution::GetSystemSeedType() == "Random") {
			srand((unsigned)time(0));
			int result = (rand() % INT_MAX);
			Distribution::SetSystemSeed(result);
		}
		else if (Distribution::GetSystemSeedType() == "same" || Distribution::GetSystemSeedType() == "Same") { // I know this may seem redundant b/c system seed is what it is, but for verification purposes for FTI it'll work
			Distribution::SetSystemSeed(Distribution::GetSystemSeed());
		}


		///Included for simulation testing purposes -> will be moved during GUI integration
		while (SimExec::GetSimulationFlag())
			SimExec::RunSimulation(inputReader.GetTerminationObj()->_months[i], inputReader.GetTerminationObj()->_days[i], inputReader.GetTerminationObj()->_year[i]);

		cout << endl << endl << endl;

		cout << SimExec::PrintNumInCondES() << endl;

		//For Kevin, this causes an infinite loop
		ScribeSetTerminationTime(SimExec::GetTotalSimulationTime());
		//cout << Step::GetResourcePool().find("L Bay")->second->GetResourceCount() << endl;
		map<string, StepResource*>::const_iterator it = Step::GetResourcePoolBegin();
		while (it != Step::GetResourcePoolEnd()) {
			cout << "Name: " << it->first << ", count: " << it->second->GetResourceCount() << endl;
			it++;
		}
		if (inputReader.GetNumRuns() > 1) {
			if (i == inputReader.GetNumRuns() - 1)
				break;
			else {
				inputReader.GetAirCount();
				SimExec::ReinitalizeSimulation();
				Step::ResetPools();
				cout << "Prepping and Starting Next Run" << endl << endl << endl;
				inputReader.ResetAirCount();
				inputReader.ClearGuiAircraft();
				Aircraft::ResetAircraftIDs();
				InputReader::ResetIDCount();
			}
		}

	}

	inputReader.GetAirCount();
	Scribe::Archive();

	///////////////////////////////////////
	/// DISCONNECTING FROM THE DATABASE ///
	///////////////////////////////////////	
	


	////////////////////////////////////////

	return 0;
}