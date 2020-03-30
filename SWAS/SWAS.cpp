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
#include <map>

using namespace std;

///////////////////////////////////////////////////
///	Temporary for testing linked list searching ///
///			until GUI LL is available			///
///////////////////////////////////////////////////

/* Link list node */
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

void ArriveAircraft()
{
	////////////////////////////////////////////
	///	TEMPORARY DRIVER CODE FOR TESTING LL ///
	////////////////////////////////////////////

	///* Start with the empty list */
	//Node* head = NULL;
	//string x = "F-35";

	///* Use push() to construct */
	//push(&head, "F-18");
	//push(&head, "F-15");
	//push(&head, "Apache");

	///////////////////////////////////////
	///////////////////////////////////////

	InputReader inputReader;

	//populate master map
	inputReader.ReadInputData();
	//inputReader.PrintEverything();

	//SimExec::SetInputReader(inputReader);
	SimExec::InitializeSimulation(inputReader.GetCalConverter()->GetMonthMap().size(), inputReader.GetCalConverter()->GetCalArray());
	inputReader.AddSelectedAircraft("F-35");
	inputReader.AddSelectedAircraft("F-18");
	inputReader.AddSelectedAircraft("Apache");

	cout << "Master Map has " << inputReader.GetMapSize() << " unique aircraft types." << endl;

	//for all unique aircraft types in the master map 
	map<string, Aircraft*>::const_iterator iter = inputReader.GetMasterMapBegin();
	while (iter != inputReader.GetMasterMapEnd())
	{
		//if the current aircraft matches one in the linked list, create instance etc.
		// if (search(head, iter->first) == true)

		//Jordan: this find aircraft function is not working 100%
		//	We are receiving the first aircraft found and iterating, but not getting any more aircraft afterwards 
		//	After comparing it to my test LL search function, may need to pass head as a param, but 
		//	Can't with it being a private struct to this input reader)
		
		// 3/30: AddSelectedAircraft function has been updated to correctly reflect additions [Jordan}
		// 3/30: Tested FindSelectedAircraft Function w/ updated AddSelectedAircraft, and the two work together.

		if(inputReader.FindSelectedAircraft(iter->first) == true)
		{
			/* Create the first instance of that particular aircraft type */
			Aircraft* firstAircraft = new Aircraft(*iter->second);
			cout << "_____________________________________________" << endl;
			cout << "_____________________________________________" << endl;
			cout << "Creating first instance of: " << firstAircraft->GetAircraftType() << endl;

			/* Look up all of that particular aircraft type's repair jobs (and associated steps, 
			resources, parts, etc.) from the master map and copy them to this new aircraft's list */ 
			firstAircraft->CopyMyJobList(iter->first);
		//	cout << "MY RJ MAP SIZE : " << firstAircraft->GetMyRJMapSize() << endl;

			/* Then schedule the next unplanned, recurring, and calendar arrivals if they have them: */
			
			///* Unplanned */
			//SourceBlock* unplanArrival = new SourceBlock(
			//	firstAircraft->GetAircraftIAT(),
			//	firstAircraft->GetAircraftType(),
			//	firstAircraft,
			//	"Unplanned Arrival",
			//	10000); 
			//	

			///* Recurring */
			//SourceBlock* recurArrival = new SourceBlock(
			//	firstAircraft->GetRecurringIATs(), //get a map
			//	firstAircraft->GetAircraftType(),
			//	firstAircraft,
			//	"Recurring Arrival",
			//	10000);

			///* Calendar */
			//SourceBlock* calArrival = new SourceBlock(
			//	firstAircraft->GetAircraftType(),
			//	firstAircraft,
			//	"Calendar Arrival",
			//	1, //should only be one instance of this calendar event arrival
			//	firstAircraft->GetCalendarObj(),
			//	10000);
			

			//SinkBlock* depart = new SinkBlock(firstAircraft->GetAircraftType());

			//unplanArrival->SetNextTask(depart);
			//recurArrival->SetNextTask(depart);
			//calArrival->SetNextTask(depart);

	

			////potential future logic below, unused now that first steps will lead to last step of last repair job:

			//////if more steps
			////	/* getNextStep() keeps getting the next step til all steps in RJ are done,
			////	can be made to take argument of RJ type (unplan,etc) then get next
			////	RJ of same type's first step */

			////	//	Step* nextUnplanStep = firstAircraft->GetNextStep("Unplanned");
			////	//	unplanArrival->SetNextTask(nextUnplanStep);
			////	//	nextUnplanStep->SetNextTask(depart);

			////	//	Step* nextRecurStep = firstAircraft->GetNextStep("Recurring");
			////	//	recurArrival->SetNextTask(nextRecurStep);
			////	//	nextRecurStep->SetNextTask(depart);

			////	//	Step* nextCalStep = firstAircraft->GetNextStep("Calendar");
			////	//	calArrival->SetNextTask(nextCalStep);
			////	//	nextCalStep->SetNextTask(depart);

		}

		iter++;
	}

	Warehouse::SetMasterMaps(inputReader);

	cout << "reading is finished" << endl;
}


int main() {

	ArriveAircraft();

	//SimExec::InitializeSimulation(SimExec::GetInputReader().GetCalConverter()->GetMonthMap().size(), SimExec::GetInputReader().GetCalConverter()->GetCalArray());

	//This is done through the GUI visualization
	/*
		SimExec::RunSimulation() || SimExec::RunSimulation(/endTime/);
		//Print Values here
	*/

	//while (SimExec::GetSimulationFlag())
		//SimExec::RunSimulation(0, 0, 2021);

	return 0;
}