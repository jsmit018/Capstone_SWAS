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

		//	Find Run Information Table
		//	 get seed type value and num runs

			if (line.find("Run Info Table") != string::npos) {
				printf("got Run Info table \n");

				int numRuns;
				string seedType;

				getline(dataFile, line);

				if (line != ",,,,,,,,,," && line.find("Number of Runs") != string::npos) {
					dataFile >> numRuns >> c >> seedType;

					//removed 10 commas from string
					seedType = seedType.erase(seedType.length() - 10);

						cout << "num of runs: " << numRuns << " seed type: " << seedType << endl;

						////**********TO DO: give numRuns to simExec, give seedType to distribution

					//	Distribution::SetSystemSeedType(seedType);
						//in distribution file, there will be an if statement that determines whether 
						//the operator(generator) function will be called or if the seed version
				}
				getline(dataFile, line);
			}
			//cout << line << endl;

			//////////////////////////////////////////
			//////////////   CALENDAR    /////////////
			//////////////////////////////////////////
			//while (line.find("Calendar Table") == string::npos) {
			//	getline(dataFile, line);
			//}

			//getline(dataFile, line);
			//cout << line << endl;

			if (line.find("Calendar Table") != string::npos) {
				printf("got Calendar table \n");

				int month = INT_MAX;
				int numDays = INT_MAX;

				
				///// Beginining of a test /////

				getline(dataFile, line);
				getline(dataFile, line);
				while (month != 0)
				{
					stringstream str(line);

					str >> month;
					if (str.peek() == ',')
						str.ignore();
					str >> numDays;

					cout << "month " << month << " days, " << numDays << endl; 

					calConvert.InsertDays(month, numDays);
					getline(dataFile, line);
				}
			}







			//////////////////////////////////////////
			//////////////   AIRCRAFT    /////////////
			//////////////////////////////////////////
			if (line.find("Aircraft Info Table") != string::npos) {
				printf("got Aircraft Info table \n");

				string aircraftName;
				int aircraftPriority;
				double aircraftFootprintX;
				double aircraftFootprintY;
				double iat_unplanned;

				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						Aircraft* newAir = new Aircraft();
						newAir->SetAircraftID(0);
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,,") {
							////breaking up strings by comma
							if (!getline(ss, line, ','))
								break;

							if (line.empty())
								break;

							row.push_back(line);
							cout << line << endl;
						}
						else
							getline(ss, line);
						//*****************************************************************
						// call resource function to store resource name & resource quantity
					}

				}
			}
				//////////////////////////////////////////
				//////////////   RESOURCES   /////////////
				//////////////////////////////////////////

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
							cout << line << endl;

							//*****************************************************************
							//use pointer to get vector values = to resource name and quantity
							//*****************************************************************
						}
						else
							getline(ss, line);
						//*****************************************************************
						// call resource function to store resource name & resource quantity
					}

					cout << "vector size: " << row.size() << endl;

					for (int i = 0; i < row.size(); ++i) {
						cout << "index [" << i << "]: " << row[i] << endl;
					}
					cout << "after for loop \n";
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
