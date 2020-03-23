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
#include <map>;

using namespace std;

///////////////////////////////////////////////////
///	Temporary for testing linked list searching ///
///			until GUI LL is available			///
///////////////////////////////////////////////////

/* Link list node */
class Node
{
public:
	string key;
	Node* next;
};

/* Given a reference (pointer to pointer) to the head
of a list and an int, push a new node on the front
of the list. */
void push(Node** head_ref, string new_key)
{
	/* allocate node */
	Node* new_node = new Node();

	/* put in the key */
	new_node->key = new_key;

	/* link the old list off the new node */
	new_node->next = (*head_ref);

	/* move the head to point to the new node */
	(*head_ref) = new_node;
}

/* Checks whether the value x is present in linked list */
bool search(Node* head, string x)
{
	Node* current = head; // Initialize current  
	while (current != NULL)
	{
		if (current->key == x)
			return true;
		current = current->next;
	}
	return false;
}
///////////////////////////////////////////////////



void ArriveAircraft()
{
	///////////////////////////////////////
	///	TEMP DRIVER CODE FOR TESTING LL	///
	///////////////////////////////////////

	/* Start with the empty list */
	Node* head = NULL;
	string x = "F-35";

	/* Use push() to construct */
	push(&head, "F-18");
	push(&head, "F-15");
	push(&head, "Apache");
	///////////////////////////////////////
	///////////////////////////////////////

	InputReader inputReader;

	//populate master map
	inputReader.ReadInputData();
	inputReader.PrintEverything();

	cout << "Master Map has " << inputReader.GetMapSize() << " unique aircraft types." << endl;

	//for all unique aircraft types in the master map 
	map<string, Aircraft*>::const_iterator iter = inputReader.GetMasterMapBegin();
	while (iter != inputReader.GetMasterMapEnd())
	{
		//if the current aircraft matches one in the linked list, create instance etc.
		if (search(head, iter->first) == true)
		{
			cout << "Creating first instance of: " << iter->first << endl;
			cout << endl;

			/* Create the first instance of that particular aircraft type */
			Aircraft* firstAircraft = new Aircraft();

			/* Look up all of that particular aircraft type's repair jobs (and associated steps, 
			resources, parts, etc.) from the master map and copy them to this new aircraft's list */ 
			firstAircraft->CopyMyJobList(iter->first);

			firstAircraft->GetAircraftRecurIAT();
			/* TO DO: IMPLEMENT GetNumCalEvents() AND GetCalendarObj() */

			/* Then schedule the next unplanned, recurring, and calendar arrivals if they have them */
			//SourceBlock* unplanArrival = new SourceBlock(/*recuring distribution*/, //iat distribution, there is none for cal
			//											firstAircraft, // aircraft*
			//											firstAircraft->GetAircraftType(), //aircraft name
			//											firstAircraft->GetNumCalEvents(), //num cal events to sched
			//											firstAircraft->GetCalendarObj(), //calobj*
			//											10000);// num to generate - shouldn't need this
			//
			//SourceBlock* recurArrival = new SourceBlock(firstAircraft->GetAircraftIAT(), //iat distribution, there is none for cal
			//											firstAircraft, // aircraft*
			//											firstAircraft->GetAircraftType(), //aircraft name
			//											firstAircraft->GetNumCalEvents(), //num cal events to sched
			//											firstAircraft->GetCalendarObj(), //calobj*
			//											10000);// num to generate - shouldn't need this
			//
			//SourceBlock* calArrival = new SourceBlock(NULL, //iat distribution, there is none for cal
			//											firstAircraft, // aircraft*
			//											firstAircraft->GetAircraftType(), //aircraft name
			//											firstAircraft->GetNumCalEvents(), //num cal events to sched
			//											firstAircraft->GetCalendarObj(), //calobj*
			//											10000);// num to generate - shouldn't need this

			//SinkBlock* depart = new SinkBlock(firstAircraft->GetAircraftType());

			///* TO DO: IMPLEMENT AreMoreSteps() AND GetNextStep() */
			//if (firstAircraft->AreMoreSteps() == true) //(false if no more steps in vector or rjs in myMap)
			//{	
			//	/* getNextStep() keeps getting the next step til all steps in RJ are done,
			//	can be made to take argument of RJ type (unplan,etc) then get next 
			//	RJ of same type's first step */
			//	
			//	Step* nextUnplanStep = firstAircraft->GetNextStep("Unplanned"); 
			//	unplanArrival->SetNextTask(nextUnplanStep);
			//	nextUnplanStep->SetNextTask(depart);

			//	Step* nextRecurStep = firstAircraft->GetNextStep("Recurring"); 
			//	recurArrival->SetNextTask(nextRecurStep);		
			//	nextRecurStep->SetNextTask(depart);

			//	Step* nextCalStep = firstAircraft->GetNextStep("Calendar"); 
			//	calArrival->SetNextTask(nextCalStep);	
			//	nextCalStep->SetNextTask(depart);

			//}
		}

		iter++;
	}

	Warehouse::SetMasterMaps(inputReader);

	cout << "reading is finished" << endl;
}


int main() {

	ArriveAircraft();

	SimExec::InitializeSimulation(CalConverter::GetMonthMap().size(), CalConverter::GetCalArray());

	//This is done through the GUI visualization
	/*
		SimExec::RunSimulation() || SimExec::RunSimulation(/endTime/);
		//Print Values here
	*/

	return 0;
}