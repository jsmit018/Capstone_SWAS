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

void InitializeAircraft()
{
	cout << "reading is finished" << endl;

	//SimExec::SetInputReader(inputReader);
	SimExec::InitializeSimulation(inputReader.GetCalConverter()->GetMonthMap().size(), inputReader.GetCalConverter()->GetCalArray());
	//Setting the Initial System Seed I just picked 8 b/c of the team size
	Distribution::SetSystemSeed(8);
	//inputReader.AddSelectedAircraft("F-35");
	inputReader.AddSelectedAircraft(1);
	//inputReader.AddSelectedAircraft("F-18");
	inputReader.AddSelectedAircraft(3);
	//inputReader.AddSelectedAircraft("Apache");

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

int main()
{
	//////////////////////////////////////
	///// CONNECTING TO THE DATABASE /////
	//////////////////////////////////////

	/////////////////////////////////////

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
			SimExec::RunSimulation(0, 0, 2023);

		cout << endl << endl << endl;

		cout << SimExec::PrintNumInCondES() << endl;

		//For Kevin, this causes an infinite loop
		ScribeSetTerminationTime(SimExec::GetTotalSimulationTime());

	}

	inputReader.GetAirCount();
	Scribe::Archive();

	///////////////////////////////////////
	/// DISCONNECTING FROM THE DATABASE ///
	///////////////////////////////////////	
	


	////////////////////////////////////////

	return 0;
}