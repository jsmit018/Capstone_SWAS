#include "SimExec.h"
#include "InputInterface.h"
#include "SourceTask.h"
#include "RepairJob.h"
#include "Resource.h"
#include "Step.h"
#include "Aircraft.h"
#include "Distribution.h"
#include "SinkTask.h"
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
	////////////////////////////////////////////
	///	TEMPORARY DRIVER CODE FOR TESTING LL ///
	////////////////////////////////////////////

	/* Start with the empty list */
	Node* head = NULL;
	string x = "F-35";

	/* Use push() to construct */
	push(&head, "F-18");
	push(&head, "F-15");
	push(&head, "Apache");
	///////////////////////////////////////
	///////////////////////////////////////

	//Jordan TO DO: set up GUI to DES linked list in place of one above (maybe set it up in Input Interface?)
	//Andie TO DO: set up logic for comparing linked list values to master map to pick correct aircraft [done]

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
			/* Create the first instance of that particular aircraft type */
			Aircraft* firstAircraft = new Aircraft(*iter->second);
			
			cout << "Creating first instance of: " << firstAircraft->GetAircraftType() << endl;
			cout << endl;

			/* Look up all of that particular aircraft type's repair jobs (and associated steps, 
			resources, parts, etc.) from the master map and copy them to this new aircraft's list */ 
			firstAircraft->CopyMyJobList(iter->first);
			cout << "MY RJ MAP SIZE : " << firstAircraft->GetMyRJMapSize() << endl;

			cout << "FIRST AIRCRAFT IAT IS:" << firstAircraft->GetAircraftIAT() << endl;
			//Andie TO DO: IMPLEMENT GetNumCalEvents()
						 //IMPLEMENT Get/Set vector of recurring iats in Aircraft.cpp
			//Jordan TO DO: IMPLEMENT Set/GetCalendarObj in Aircraft.cpp 
						//ADD logic for getting each value from the recurring IAT vector being passed to SourceBlock constructor of recurring

			/* Then schedule the next unplanned, recurring, and calendar arrivals if they have them */
			/* Unplanned */
			//SourceBlock* unplanArrival = new SourceBlock(
			//	firstAircraft->GetAircraftIAT(),
			//	firstAircraft->GetAircraftType(),
			//	firstAircraft,
			//	"WHAT IS NAME",
			//	10000); // need to make sure there's an unlimited option
			//	
			///* Recurring */
			//SourceBlock* recurArrival = new SourceBlock(
			//	firstAircraft->GetRecurringIATs(),
			//	firstAircraft->GetAircraftType(),
			//	firstAircraft,
			//	"WHAT IS NAME",
			//	10000);

			///* Calendar */
			//SourceBlock* calArrival = new SourceBlock(
			//	firstAircraft->GetAircraftType(),
			//	firstAircraft,
			//	"WHAT IS NAME",
			//	firstAircraft->GetNumCalEvents(),
			//	firstAircraft->GetCalendarObj(),
			//	10000);
				

			SinkBlock* depart = new SinkBlock(firstAircraft->GetAircraftType());

			//Andie TO DO: IMPLEMENT AreMoreSteps() AND GetNextStep()
			if (firstAircraft->AreMoreSteps() == true) //(false if no more steps in vector or rjs in myMap)
			{	
				/* getNextStep() keeps getting the next step til all steps in RJ are done,
				can be made to take argument of RJ type (unplan,etc) then get next 
				RJ of same type's first step */
				
			/*	Step* nextUnplanStep = firstAircraft->GetNextStep("Unplanned"); 
				unplanArrival->SetNextTask(nextUnplanStep);
				nextUnplanStep->SetNextTask(depart);

				Step* nextRecurStep = firstAircraft->GetNextStep("Recurring"); 
				recurArrival->SetNextTask(nextRecurStep);		
				nextRecurStep->SetNextTask(depart);

				Step* nextCalStep = firstAircraft->GetNextStep("Calendar"); 
				calArrival->SetNextTask(nextCalStep);	
				nextCalStep->SetNextTask(depart);*/

			}
		}

		iter++;
	}

	cout << "reading is finished" << endl;
}


int main() {

	ArriveAircraft();

	/*
		SimExec::InitializeSimulation(11, /CalendarConverterDaysArrayHere/);
	*/

	/*
		SimExec::RunSimulation() || SimExec::RunSimulation(/endTime/);
		//Print Values here
	*/

	return 0;
}