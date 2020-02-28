//InputInterface.cpp: Andrea Robey

#include "InputInterface.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

InputReader::InputReader()
{

}

InputReader::~InputReader()
{
	// Iterate through the whole map and deallocate all pointer objects using the syntaxt delete objectPointer;
	// for each map element
	//	Aircraft * toDelete = map::iterator->first;
	// if there are pointers within that object that pointer is pointing to
		// first iterate through those to deallocate them
	// delete toDelete; 
}

void InputReader::ReadInputData() //initialization for getting data
{
	CalConverter calConvert;
	//Step step;
	Resource resource;
	string line;


	ifstream dataFile("SWASInputData.csv");
	if (dataFile.is_open())
	{

		while (getline(dataFile, line)) {
		//	cout << line << endl; 
			char c;
			string commas;


			//////////////////////////////////////////
			//////////////   RUN INFO    /////////////
			//////////////////////////////////////////

			//Find Run Information Table [Done]
			//Get seed type value [Done]
				//Give to Distribution [Errors]
			//Get number of runs [Done]
				//Give to SimExec []

			if (line.find("Run Info Table") != string::npos) {
				printf("got Run Info table \n");

				int numRuns;
				string seedType;

				getline(dataFile, line);

				if (line != ",,,,,,,,,," && line.find("Number of Runs") != string::npos) {
					dataFile >> numRuns >> c >> seedType;

					//removed 10 commas from string
					seedType = seedType.erase(seedType.length() - 10);

//						cout << "num of runs: " << numRuns << " seed type: " << seedType << endl;

				//		Distribution::SetSystemSeedType(seedType);
						//in distribution file, there will be an if statement that determines whether 
						//the operator(generator) function will be called or if the seed version
				}
				getline(dataFile, line);
			}


			//////////////////////////////////////////
			//////////////   CALENDAR    /////////////
			//////////////////////////////////////////

			if (line.find("Calendar Table") != string::npos) {
				printf("got Calendar table \n");

				int month = INT_MAX;
				int numDays = INT_MAX;

				getline(dataFile, line);
				getline(dataFile, line);
				while (month != 0)
				{
					stringstream str(line);

					str >> month;
					if (str.peek() == ',')
						str.ignore();
					str >> numDays;

//					cout << "month " << month << " days, " << numDays << endl; 

					calConvert.InsertDays(month, numDays);
					getline(dataFile, line);
				}
			}


			//////////////////////////////////////////
			//////////////   AIRCRAFT    /////////////
			//////////////////////////////////////////

			// Read in from tables:
			// Aircraft Info - 
				//Type [done]
				//Priority [done]
				//Footprint [done]
			// Unplanned Arrivals -
				// Compare type strings to find:
					//Unplanned iat [done]
			//Store new aircraft object into map with type string as key [done]

			if (line.find("Aircraft Info Table") != string::npos) {
				printf("got Aircraft Info table \n");

				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,,") {
							////breaking up strings by comma
							if (!getline(ss, line, ','))
								break;

							if (line.empty())
								break;

							row.push_back(line);
//							cout << line << endl;
						}
						else
							getline(ss, line);
					}

//					cout << "vector size: " << row.size() << endl;
				
					string airType;
					int airPriority;
					double airLength;
					double airWingspan;

					Aircraft* newAir = new Aircraft();

				//	for (int i = 0; i < row.size(); ++i) {
					//	cout << "line " << i << ": " << row[i] << endl;
				//	}

					//for each index, set each variable
					//	istringstream iss0(row[0]);
					airType = row[0];
					newAir->SetAircraftType(airType);

					istringstream iss1(row[1]);
					iss1 >> airPriority;
					newAir->SetAircraftPriority(airPriority);

					istringstream iss2(row[2]);
					iss2 >> airLength;
						
					istringstream iss3(row[3]);
					iss3 >> airWingspan;

					newAir->SetAircraftFootprint(airLength, airWingspan);

					masterMap.insert(pair<string, Aircraft*>(airType, newAir));
					for (map<string, Aircraft*>::const_iterator it = masterMap.begin(); it != masterMap.end(); ++it)
					{
//						cout << "MAP: " << it->first << endl;
//						cout << "OBJ: ";
//						it->second->PrintProperties();
//						cout << endl;
					}

//					cout << "after aircraft for loop \n";	
				}
			}


			if (line.find("Unplanned Arrivals Table") != string::npos) {
				printf("got Unplanned Arrivals Table \n");

				string unplannedType;
				string unplannedIAT;
			
				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,,") {
							////breaking up strings by comma
							if (!getline(ss, line, ','))
								break;

							if (line.empty())
								break;

							row.push_back(line);
						}
						else
							getline(ss, line);
					}
					//	for (int i = 0; i < row.size(); ++i) {
						//	cout << "line " << i << ": " << row[i] << endl;
					//	}

						unplannedType = row[0];

						unplannedIAT = row[1];

						//compare to string
						map<string, Aircraft*>::const_iterator iter = masterMap.find(unplannedType);
						if (iter == masterMap.end()) {
							printf("end of map \n");
							cout << "unplanned type: " << unplannedType << endl;
						}

						iter->second->SetAircraftIAT(unplannedIAT);
//						iter->second->PrintProperties();
//						cout << endl;
				}
			}

			//////////////////////////////////////////
			///////////   REPAIR JOBS   //////////////
			//////////////////////////////////////////

			////////Planned/////////

			//Planned Maintenance Table - 
				// For loop: Compare aircraft type string with string in table to find:
					//Repair Job Name [done]
					//Schedule Type [done]
					//Calendar Schedule Date [done]
					//Recurring Sched Amount [done]
					//Indoor Requirement [done]
				//Create object of repair job
				//Store object in allrepairjobmap with appropriate aircraft type	
			
			if (line.find("Planned Maintenance Table") != string::npos) {
				printf("got Planned Maintenance Table \n");

				string planType;
				string repairName;
				string schedType;
				string schedCal;
				double schedRecur;
				char indoorReq;

				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,,") {
							////breaking up strings by comma
							if (!getline(ss, line, ','))
								break;

							if (line.empty())
								break;

							row.push_back(line);
						}
						else
							getline(ss, line);
					}

					RepairJob* newJob = new RepairJob();

					//	for (int i = 0; i < row.size(); ++i) {
						//	cout << "line " << i << ": " << row[i] << endl;
					//	}

					planType = row[0];
//					cout << "craft type: " << unAirType << endl;

					repairName = row[1];
//					cout << "rj type: " << repairName << endl;
					newJob->SetName(repairName);

					schedType = row[2];
//					cout << "sched type: " << schedType << endl;
					newJob->SetSchedType(schedType);

					if (schedType == "Calendar") {
						schedCal = row[3];
//						cout << "calendar date: " << schedCal << endl;						newJob->SetSchedType(schedType);
						newJob->SetCalendarDate(schedCal);
					}

					else if (schedType == "Recurring") {
						istringstream unss3(row[3]);
						unss3 >> schedRecur;
//						cout << "recur: " << schedRecur << endl;
						newJob->SetRecurring(schedRecur);
					}

					istringstream unss4(row[4]);
					unss4 >> indoorReq;
//					cout << "indoor req: " << indoorReq << endl << endl;
					newJob->SetIndoorReq(indoorReq);

//					cout << "PLANNED: " << endl;
//					newJob->PrintJobProperties();
//					cout << endl;
				

					masterMap[planType]->AddRepairJobMaster(newJob, repairName);
				}
			}
				///////Unplanned////////

				// Unplanned Maintenance Table -
					// For loop: Compare aircraft name string with string in table to find:
						//Repair Job Type
							//once found, get:
								//Repair Job Name [done]
								//Unplanned Probability [done]
								//Indoor Requirement [done]
				//Create object of repair job [done]
					//Store object in allrepairjobmap with appropriate aircraft type [done]

			if (line.find("Unplanned Maintenance Table") != string::npos) {
				printf("got Unplanned Maintenance Table \n");

				string unplanType;
				string unRepairName;
				string repJobProb;
				char unIndoorReq;

				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,,") {
							////breaking up strings by comma
							if (!getline(ss, line, ','))
								break;

							if (line.empty())
								break;

							row.push_back(line);
						}
						else
							getline(ss, line);
					}

					RepairJob* newJob = new RepairJob();

					//	for (int i = 0; i < row.size(); ++i) {
						//	cout << "line " << i << ": " << row[i] << endl;
					//	}

					unplanType = row[0];
//					cout << "craft type: " << unplanType << endl;
//TO DO:			if "all", set up logic

					unRepairName = row[1];
				//	cout << "unplanned rj name: " << unRepairName << endl;
					newJob->SetName(unRepairName);

					repJobProb = row[2];
				//	cout << "probability: " << repJobProb << endl;
					newJob->SetUnplannedProb(repJobProb);

					istringstream unss(row[3]);
					unss >> unIndoorReq;
				//	cout << "indoor req: " << unIndoorReq << endl << endl;
					newJob->SetIndoorReq(unIndoorReq);

//					cout << "UNPLANNED: " << endl;
//					newJob->PrintJobProperties();
//					cout << endl;
					
					if (unplanType == "All")
					{
						map<string, Aircraft*>::iterator it = masterMap.begin();
						while (it != masterMap.end())
						{
							it->second->AddRepairJobMaster(newJob, unRepairName);
							it++;
						}
					}
					else
					{
						masterMap[unplanType]->AddRepairJobMaster(newJob, unRepairName);
					}
				}
			}


			//////////////////////////////////////////
			//////////////   STEPS   /////////////////
			//////////////////////////////////////////

			// Steps Table -
				// Compare repair job name strings to find:
					//Priority, store as key in allrepairjobsmap
					//For loop: for each step
						//Create Step object
						//Find:
							//Step ID
							//Step Name
							//Step Duration
							//Inspection Failure Distribution
							//Return Step
							//Vector of Required Resources
							//Required Part
							//Amount of Parts Used
						//Store Step Object in Repair Job's map


						// if it exists, we add the step
						job->AddStep(newStep);
						iter++;
					}
				}
			}

			map<string, Aircraft*>::const_iterator iter = masterMap.begin();

			while (iter != masterMap.end())
			{
//				iter->second->PrintProperties();
				iter++;
			}

			//////////////////////////////////////////
			/////////////   RESOURCES   //////////////
			//////////////////////////////////////////

			///////Spatial////////
		
			//Resource Table
			// For each of the resources in Step object's vector
				//Match resource name string to string in table
					//Find:
					//Resource Name [done]
					//Initial Count [done]
					
			//Resource Failure Table
			// For each of the resources in Step object's vector
				//Match resource name string to string in table
					//Find:
					//Failure Name
					//Failure Type
					//Failure Distribution

			//////Consumable//////

			//Parts Table
			// For each of the Step's required part
				//Match part name string to string in table
					//Find:
					//Initial Quantity
					//Replenishment Threshold
					//Lead Time

			if (line.find("Resource Table") != string::npos) {
				printf("got Resource table \n");

				string resName;
				int resQuantity;
				double resourceFootprintX;
				double resourceFootprintY;

				//reading row as string
				vector <string> row;
		 
				while (line != ",,,,,,,,,,,")
				{
					row.clear();
					
					getline(dataFile, line);
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,,") {
							////breaking up strings by comma
							if (!getline(ss, line, ','))
								break;

							if (line.empty())
								break;

							row.push_back(line);
						//	cout << line << endl;

							//*****************************************************************
							//use pointer to get vector values = to resource name and quantity
							//*****************************************************************
						}
						else
							getline(ss, line);
						//*****************************************************************
						// call resource function to store resource name & resource quantity
					}

				//	cout << "vector size: " << row.size() << endl;

					for (int i = 0; i < row.size(); ++i) {
				//		cout << "index [" << i << "]: " << row[i] << endl;
					}
				//	cout << "after for loop \n";
				}
			}
		}

		dataFile.close();
		cout << "closed file" << endl;
	}
	else
	{
		cout << "CSV file didn't open!";
	}


	map<string, Aircraft*>::iterator it = masterMap.begin();
//	cout << "////////////////////////////////////////////////////// \n";
//	cout << "////////////////////////////////////////////////////// \n";
	while (it != masterMap.end())
	{
//		it->second->PrintProperties();
		it++;
	}




	//read in values

	//for each unique aircraft
	//	store into map as key
	//		for each repair job read in
	//			create object
	//			insert object into hashmap
	//				get steps, store steps into vector

}
