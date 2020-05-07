//SWAS.cpp: Andrea Robey and Jordan Smith

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
	//schedule resource failure logic
	map<string, StepResource*>::const_iterator iter = InputReader::GetMasterResMapBegin();
	while (iter != InputReader::GetMasterResMapEnd())
	{
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
	cout << "Reading is finished" << endl;

	SimExec::InitializeSimulation(inputReader.GetCalConverter()->GetMonthMap().size(), inputReader.GetCalConverter()->GetCalArray());
	//Setting the Initial System Seed I just picked 8 b/c of the team size

	//alternatives for hardcoded aircraft selection -> names versus number identifiers
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
		if (inputReader.FindSelectedAircraft(iter->first) == true)
		{
			/* Create the first instance of that particular aircraft type by copying from master map */
			//Test count//
			int count = 1;
			int cal = 1;
			//____________
			Aircraft* firstAircraft = new Aircraft(*iter->second);
			firstAircraft->CopyMyJobList(iter->first);

			map<string, RepairJob*>::const_iterator myIter = firstAircraft->GetMyRJMapBegin();
			while (myIter != firstAircraft->GetMyRJMapEnd())
			{
				if (myIter->second->GetSchedType() == "Calendar" && cal == 1)
				{
					//calendarsourceblock schedules calendar arrival at date 
					firstAircraft->GetCalendarObj()->_months;
					firstAircraft->GetCalendarObj()->_days;
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
					// recurringsourceblock schedules first arrival at recur iat 
					SourceBlock* recurArrival = new SourceBlock(
						firstAircraft->GetRecurIatMap(),
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Recurring Arrival"/*,
						15*/);
				}

				else if (myIter->second->GetSchedType() == "Unplanned" && count == 1)
				{
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

	//if wartime, schedule at 0600 and 1800
	if (InputReader::IsWartime() == true)
	{
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
	inputReader.ReadInputData();

	Scribe::SetSaveFile("Output.csv");
	Scribe::SetRuns(inputReader.GetNumRuns());
	Scribe::SetNumRuns(inputReader.GetNumRuns());


	/*For handling multiple runs -- currently set as 1 in file for testing purposes*/
	for (int i = 0; i < inputReader.GetNumRuns(); i++)
	{

		if (i > 0) {
			ScribeAdvanceRun();
			ScribeReadResources();
		}

		InitializeAircraft();
		SchedResourceFailure();
		ScheduleFirstShiftChange();
		
		//If System Seed is the same vs. Different --If Random Generate a new system seed. --If the same don't worry about it
		if (Distribution::GetSystemSeedType() == "random" || Distribution::GetSystemSeedType() == "Random") {
			srand((unsigned)time(0));
			int result = (rand() % INT_MAX);
			Distribution::SetSystemSeed(result);
		}
		else if (Distribution::GetSystemSeedType() == "same" || Distribution::GetSystemSeedType() == "Same") {
			Distribution::SetSystemSeed(Distribution::GetSystemSeed());
		}


		///Included for simulation testing purposes
		while (SimExec::GetSimulationFlag())
			SimExec::RunSimulation(inputReader.GetTerminationObj()->_months[i], inputReader.GetTerminationObj()->_days[i], inputReader.GetTerminationObj()->_year[i]);

		cout << endl;

		cout << "Number of conditional events in queue at termination: " << SimExec::PrintNumInCondES() << endl;

		ScribeSetTerminationTime(SimExec::GetTotalSimulationTime());
		map<string, StepResource*>::const_iterator it = Step::GetResourcePoolBegin();
		cout << "Current counts of resources at termination: " << endl;

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

	return 0;
}