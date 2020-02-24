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
	Aircraft aircraft;
	string line;


	ifstream dataFile("SWASInputData.csv");
	if (dataFile.is_open())
	{

		while (getline(dataFile, line)) {
		//	cout << line << endl; 
			char c;
			string commas;


			//cout << line << endl;

			//////////////////////////////////////////
			//////////////   RUN INFO    /////////////
			//////////////////////////////////////////

			//Find Run Information Table [Done]
			//Get seed type value [Done]
				//Give to Distribution [Errors]
			//Get number of runs [Done]
				//Give to SimExec []

			if (line.find("Run Info Table") != string::npos) {
//				printf("got Run Info table \n");

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
			//cout << line << endl;



			//////////////////////////////////////////
			//////////////   CALENDAR    /////////////
			//////////////////////////////////////////

			if (line.find("Calendar Table") != string::npos) {
//				printf("got Calendar table \n");

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
					//Unplanned iat []
			//Store new aircraft object into map with type string as key []

			//Aircraft* newAir = new Aircraft();

			if (line.find("Aircraft Info Table") != string::npos) {
//				printf("got Aircraft Info table \n");

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

					//for each index, set each variable
					for (int i = 0; i < row.size(); ++i) {
//						cout << "index [" << i << "]: " << row[i] << endl;

						istringstream iss0(row[0]);
						iss0 >> airType;
						newAir->SetAircraftType(airType);

						istringstream iss1(row[1]);
						iss1 >> airPriority;
						newAir->SetAircraftPriority(airPriority);

						istringstream iss2(row[2]);
						iss2 >> airLength;
						
						istringstream iss3(row[3]);
						iss3 >> airWingspan;

						newAir->SetAircraftFootprint(airLength, airWingspan);

						//masterMap.insert(pair<string, Aircraft*>(airType, newAir));
						//for (map<string, Aircraft*>::const_iterator it = masterMap.begin(); it != masterMap.end(); ++it)
						//{
						//	cout << "MAP: " << it->first << endl;
						//	cout << "OBJ: "; 
						//	it->second->PrintProperties();
						//}
					}
					//cout << "type: " << airType << endl;
					//cout << "properties: ";
					//newAir->PrintProperties();
					//cout << endl;

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
//							cout << "LINE: "<< line << endl;
						}
						else
							getline(ss, line);
					}

					for (int i = 0; i < row.size(); ++i) {

						istringstream iss0(row[0]);
						iss0 >> unplannedType;

						istringstream iss1(row[1]);
						iss1 >> unplannedIAT;	
					}
//					cout << "THE TYPE: " << unplannedType << "	THE IAT: " << unplannedIAT << endl;
					///****IAT IS CUT SHORT, FIGURE OUT WHY

					for (map<string, Aircraft*>::const_iterator it = masterMap.begin(); it != masterMap.end(); ++it)
					{
						//compare to string
						map<string, Aircraft*>::const_iterator iter = masterMap.find(unplannedType);
						Aircraft* currAir = it->second;
						currAir->SetAircraftIAT(unplannedIAT);
						///*********IAT DOESNT MATCH CORRECTLY
						currAir->PrintProperties();
						cout << endl;
					}
				}
			}



			// For example down here if I wanted to find and modify the f-18
//map<string,Aircraft*>::const_iterator it = masterMap.find("F-18");
			// I am not sure if you need the iterator or can just make it equal to an airacraft pointer like
//			Aircraft * currAir = it->second; // this however is, so you can modify that aircraft now

			//////////////////////////////////////////
			/////////   REPAIR JOB & STEPS   /////////
			//////////////////////////////////////////

			////////Planned/////////
			
			// Read in from tables:
			// Planned Maintenance Table -
				// For loop: Compare aircraft type string with string in table to find:
					//Repair Job Name
					//Schedule Type
					//Calendar Schedule Date
					//Recurring Sched Amount
					//Indoor Requirement
				//Create object of repair job
				//Store object in allrepairjobmap with appropriate aircraft type			// Steps Table -
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
			

			///////Unplanned////////

			// Unplanned Maintenance Table -
				// For loop: Compare aircraft name string with string in table to find:
					//Repair Job Type
						//once found, get:
							//Repair Job Name
							//Unplanned Probability
							//Indoor Requirement
			//Create object of repair job
				//Store object in allrepairjobmap with appropriate aircraft type
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


			//////////////////////////////////////////
			/////////////   RESOURCES   //////////////
			//////////////////////////////////////////

			///////Spatial////////
		
			//Resource Table
			// For each of the resources in Step object's vector
				//Match resource name string to string in table
					//Find:
					//Initial Count
					
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




	//read in values

	//for each unique aircraft
	//	store into map as key
	//		for each repair job read in
	//			create object
	//			insert object into hashmap
	//				get steps, store steps into vector

}
