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
	cout << " in sched resource fail " << endl;
	//schedule resource failure logic
	map<string, Resource*>::const_iterator iter = InputReader::GetMasterResMapBegin();
	while (iter != InputReader::GetMasterResMapEnd())
	{
		cout << "" << endl;
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
			//		cout << "Creating first instance of " << firstAircraft->GetAircraftType() << " for copying purposes" << endl;

			firstAircraft->CopyMyJobList(iter->first);

			map<string, RepairJob*>::const_iterator myIter = firstAircraft->GetMyRJMapBegin();

			while (myIter != firstAircraft->GetMyRJMapEnd())
			{
				if (myIter->second->GetSchedType() == "Calendar" && cal == 1)
				{
					////// calendarsourceblock schedules calendar arrival at date 
					cout << "Scheduling calendar arrival for " << firstAircraft->GetAircraftType() << endl;
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
					cout << "Scheduling recurring arrival for " << firstAircraft->GetAircraftType() << endl;
					cout << endl;
					SourceBlock* recurArrival = new SourceBlock(
						firstAircraft->GetRecurIatMap(), //get a map -- The map is set up as <string, RepairJob*> we can pass the repair job along this way << this is not true
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Recurring Arrival"//,
						/*5*/);
				}

				else if (myIter->second->GetSchedType() == "Unplanned" && count == 1)
				{
					////// unplannedsourceblock schedules first arrival at unpl iat  

					SourceBlock* unplanArrival = new SourceBlock(
						firstAircraft->GetAircraftIAT(),
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Unplanned Arrival",
						myIter->second//,
						/*5*/);
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
	//////////////////////////////////////
	///// CONNECTING TO THE DATABASE /////
	//////////////////////////////////////

	#define SQL_RESULT_LEN 240
	#define SQL_RETURN_CODE_LEN 1000

	////define handles and variables
	SQLHANDLE sqlConnHandle;
	SQLHANDLE sqlStmtHandle;
	SQLHANDLE sqlEnvHandle;
	SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];

	////initializations
	sqlConnHandle = NULL;
	sqlStmtHandle = NULL;

	////allocations
	//if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
	//	goto COMPLETED;

	//if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
	//	goto COMPLETED;

	//if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
	//	goto COMPLETED;

	//output
	/*cout << "Attempting connection to SQL Server...";
	cout << "\n";*/

	//connect to SQL Server	
	//Using a trusted connection and port 14808
	//it does not matter if you are using default or named instance
	//just make sure you define the server name and the port
	//We have the option to use a username/password instead of a trusted connection
	//but its more secure to use a trusted connection
	//switch (SQLDriverConnect(sqlConnHandle,
	//	NULL,
	//	//(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=master;UID=username;PWD=password;",

	//	//********* Need to set server and database names -->automate this in GUI (if there is one)
	//	(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=govasim-2;DATABASE=SWASTestDatabase;Trusted=true;",
	//	SQL_NTS,
	//	retconstring,
	//	1024,
	//	NULL,
	//	SQL_DRIVER_NOPROMPT)) {

	//case SQL_SUCCESS:
	//	cout << "Successfully connected to SQL Server";
	//	cout << "\n";
	//	break;

	//case SQL_SUCCESS_WITH_INFO:
	//	cout << "Successfully connected to SQL Server";
	//	cout << "\n";
	//	break;

	//case SQL_INVALID_HANDLE:
	//	cout << "Could not connect to SQL Server";
	//	cout << "\n";
	//	goto COMPLETED;

	//case SQL_ERROR:
	//	cout << "Could not connect to SQL Server";
	//	cout << "\n";
	//	goto COMPLETED;

	//default:
	//	break;
	//}

	////if there is a problem connecting then exit application
	//if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
	//	goto COMPLETED;

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
	
	//close connection and free resources
	//COMPLETED:
	//	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	//	SQLDisconnect(sqlConnHandle);
	//	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	//	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

	////pause the console window - exit when key is pressed
	//cout << "\nPress enter key to exit...";
	//getchar();

	////////////////////////////////////////

	return 0;
}