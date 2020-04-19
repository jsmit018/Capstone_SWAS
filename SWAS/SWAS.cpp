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

//Scribe* outputReader;

//Scribe is now static we don't need object refference
//void InitializeScribe() {
//	outputReader = new Scribe();
//}

//Scribe* GetScribe() {
//	return outputReader;
//}

void ScribeSetDimension(double length, double width) {
	Scribe::SetWarehousDims(length, width);
}

void ScribeAdvanceRun() {
	Scribe::AdvanceRun();
}

void ScribeSetTerminationTime(double termTime) {
	Scribe::SetRunTime(termTime);
}
///////////////////////////////////////////////////
///	Temporary for testing linked list searching ///
///			until GUI LL is available			///
///////////////////////////////////////////////////

///* Link list node */
//class Node
//{
//public:
//	string key;
//	Node* next;
//};
//
///* Given a reference (pointer to pointer) to the head
//of a list and an int, push a new node on the front
//of the list. */
//void push(Node** head_ref, string new_key)
//{
//	/* allocate node */
//	Node* new_node = new Node();
//
//	/* put in the key */
//	new_node->key = new_key;
//
//	/* link the old list off the new node */
//	new_node->next = (*head_ref);
//
//	/* move the head to point to the new node */
//	(*head_ref) = new_node;
//}
//
///* Checks whether the value x is present in linked list */
//bool search(Node* head, string x)
//{
//	Node* current = head; // Initialize current  
//	while (current != NULL)
//	{
//		if (current->key == x)
//			return true;
//		current = current->next;
//	}
//	return false;
//}
///////////////////////////////////////////////////


/////In InitializeAircraft() (Previously named ArriveAircraft())
//for each aircraft in GUI selection
	//instantiates it by copying from master map
		//calls copymyjobs() which
			//iterates through master map to find its repairjobs list
			//iterates through its list of jobs and copy to _myjobsmap
	//iterates through _myjobs (map of all of its jobs - i.e. 2 unplanned, 1 recurring, 2 calendar)
		//if job is calendar event
			//calls calendar sourceblock to schedule this aircraft type's calendar repairjob to arrive at date. this will happen for all calendar events in mymap
						//**this is why you still need the sourceblock calendar constructor i made. It needs to be scheduled through source, even though its only once.
						//**because of this, you do NOT need to specify any numbers. it will always be 1 event per calendar repairjob
						//**I've removed the calling of schedulefirststep from Step so that we can remove the need for a counter i was trying to describe to you
						//**so this is what needs to be happen in SourceTask:
						//**in whatever you're calling your "arrival" EM, it won't "schedule next calendar event" because there's only one per cal job. 
						//**it needs to call the appropriate function schedulefirststep() -- if you pass the repairjob, you can access this function 
						//**it was a good idea to have separate schedulefirststep functions - recurring and calendar could have the same since they require specific repair jobs
						//**the step logic takes care of the rest of the steps.

		//if job is recurring event
			//calls recurring sourceblock schedules first arrival at recur iat 
				//**in whatever youre calling your arrival EM, it still schedules next recurring event in the recurring iat amount from the map i've provided
				//**now it needs to call the function schedulefirststep() -- if you pass the repairjob, you can access this function

		//if job is unplanned event
			//calls unplanned sourceblock schedules first arrival at unplanned iat  
				//**in whatever youre calling your arrival EM, it still schedules next unplanned event in the unplanned iat
				//**now it needs to call the function schedulefirststep() -- i've modified it to handle the logic for "am i the random job being scheduled"

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
		if (iter->second->GetFailureName() == "")
		{

		}
		else
		{
			iter->second->ScheduleFirstFailures(iter->second);
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

	cout << "Master Map has " << inputReader.GetMapSize() << " unique aircraft types." << endl;

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
					//cout << "Scheduling calendar arrival for " << firstAircraft->GetAircraftType() << endl;
					//cout << endl;
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
				/*	cout << endl;
					cout << "Scheduling recurring arrival for " << firstAircraft->GetAircraftType() << endl;
					cout << endl;*/
					SourceBlock* recurArrival = new SourceBlock(
						firstAircraft->GetRecurIatMap(), //get a map -- The map is set up as <string, RepairJob*> we can pass the repair job along this way << this is not true
						firstAircraft->GetAircraftType(),
						firstAircraft,
						"Recurring Arrival",
						10);
				}

				else if (myIter->second->GetSchedType() == "Unplanned")
				{
					////// unplannedsourceblock schedules first arrival at unpl iat  
					//(sourceblock schedules arrival, arrival schedules next arrival)
					cout << endl;
					cout << "Scheduling first unplanned arrival for " << firstAircraft->GetAircraftType() << endl;
					cout << "Scheduling " << count << " unplanned arrival for " << firstAircraft->GetAircraftType() << endl;
					cout << endl;
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


//CODE COMMENTS KEPT JUST IN CASE
//			/* Unplanned */
//			//Set Number of Aircraft Arrivals		
//		//	firstAircraft->GetAircraftIAT()->PrintDistribution();
//		//	cout << endl;
//
//			//Debugged and Confirmed this Schedules correctly
//			SourceBlock* unplanArrival = new SourceBlock(
//				firstAircraft->GetAircraftIAT(),
//				firstAircraft->GetAircraftType(),
//				firstAircraft,
//				"Unplanned Arrival",
//				10); 
//
//
//			//Infinite number of aircraft arrivals
//			/*SourceBlock* unplanArrival = new SourceBlock(
//				firstAircraft->GetAircraftIAT(),
//				firstAircraft->GetAircraftType(),
//				firstAircraft,
//				"Unplanned Arrival");*/
//
//			//______________________________________________________________________________
//			///* Recurring */

//			//Infinite number of recurring aircraft arrival
//			//SourceBlock* recurArrival = new SourceBlock(
//			//	firstAircraft->GetRecurIatMap(), //get a map
//			//	firstAircraft->GetAircraftType(),
//			//	firstAircraft,
//			//	"Recurring Arrival",
//			//	1000);

//
//
//			////POTENTIAL future logic below, unused now that first steps will lead to last step of last repair job:
//
//			//////if more steps
//			////	/* getNextStep() keeps getting the next step til all steps in RJ are done,
//			////	can be made to take argument of RJ type (unplan,etc) then get next
//			////	RJ of same type's first step */
//
//			////	//	Step* nextUnplanStep = firstAircraft->GetNextStep("Unplanned");
//			////	//	unplanArrival->SetNextTask(nextUnplanStep);
//			////	//	nextUnplanStep->SetNextTask(depart);
//
//			////	//	Step* nextRecurStep = firstAircraft->GetNextStep("Recurring");
//			////	//	recurArrival->SetNextTask(nextRecurStep);
//			////	//	nextRecurStep->SetNextTask(depart);
//
//			////	//	Step* nextCalStep = firstAircraft->GetNextStep("Calendar");
//			////	//	calArrival->SetNextTask(nextCalStep);
//			////	//	nextCalStep->SetNextTask(depart);
//



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
		/*
		cout << endl;
		cout << endl;
		cout << endl;
		cout << "RUN NUMBER " << i + 1 << endl;
		*/
		InitializeAircraft();
		SchedResourceFailure();
		//InitalizeAircraft(GetScribe());

		///Included for simulation testing purposes -> will be moved during GUI integration
		while (SimExec::GetSimulationFlag())
			SimExec::RunSimulation(0, 0, 2030);

		cout << endl << endl << endl;

		cout << SimExec::PrintNumInCondES() << endl;

		//For Kevin, this causes an infinite loop
		ScribeSetTerminationTime(SimExec::GetTotalSimulationTime());

	}

	Scribe::Archive();

	return 0;
}