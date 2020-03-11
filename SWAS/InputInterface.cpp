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

					Distribution::SetSystemSeedType(seedType);
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

					_masterMap.insert(pair<string, Aircraft*>(airType, newAir));
					for (map<string, Aircraft*>::const_iterator it = _masterMap.begin(); it != _masterMap.end(); ++it)
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
					map<string, Aircraft*>::const_iterator iter = _masterMap.find(unplannedType);
					if (iter == _masterMap.end()) {
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

				string plannedType;
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

					plannedType = row[0];
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

										//cout << "PLANNED: " << endl;
										//newJob->PrintJobProperties();
										//cout << endl;


					_masterMap[plannedType]->AddRepairJob(newJob, repairName);
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

										//cout << "UNPLANNED: " << endl;
										//newJob->PrintJobProperties();
										//cout << endl;

					if (unplanType == "All")
					{
						map<string, Aircraft*>::iterator it = _masterMap.begin();
						while (it != _masterMap.end())
						{
							it->second->AddRepairJob(newJob, unRepairName);
							it++;
						}
					}
					else
					{
						//add new repair job to list of unplanType aircraft's repair jobs in the master map
						_masterMap[unplanType]->AddRepairJob(newJob, unRepairName);
					}
				}
			}


			//////////////////////////////////////////
			//////////////   STEPS   /////////////////
			//////////////////////////////////////////

			// Steps Table -
				// Compare repair job name strings to find:
					//Priority
					//For loop: for each step
						//Create Step object
						//Find:
							//Step ID [done]
							//Step Name [done]
							//Step Duration [done]
							//Inspection Failure Distribution [done]
							//Return Step [done]
							//Vector of Required Resources
							//Required Parts
							//Amount of Parts Needed                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
						//Store Step Object in Repair Job's map

			if (line.find("Steps Table") != string::npos) {
				printf("got Steps Table \n \n");

				string currentJob;
				int jobPriority;

				int stepID;
				string stepName;
				string stepType;
				string inspecFailProb;
				int returnStep;
				string stepDur;
				string reqResource;
				string reqParts;
				int numParts;
				int numResources;

				int rowCounter = 0;

				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,,")
						break;
					istringstream iss(line);

					////parsing the whole file and storing individual strings
					while (iss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,,") {


							////breaking up strings by comma
							if (!getline(iss, line, ','))
								break;

							if (line.empty()) {
								//								cout << "Empty cell \n";
															//	break;
							}

							row.push_back(line);
						}
						else
							getline(iss, line);
					}

					Step* newStep = new Step(stepDur, stepName);

					//	for (int i = 0; i < row.size(); ++i) {
						//	cout << "line " << i << ": " << row[i] << endl;
					//	}
//					cout << "row : " << row[0] << endl;
					//	cout << "row size " << row.size() << endl; 
					if (rowCounter == 0) {
						currentJob = row[0];
						rowCounter++;
					}
					if (row[0] != "")
					{
//						cout << "non blank row " << endl;
						currentJob = row[0];
					}

					istringstream ssSteps(row[1]);
					ssSteps >> jobPriority;

					//compare jobname to insert priority to correct map location

					istringstream ssSteps2(row[2]);
					ssSteps2 >> stepID;

					//	stepName = row[3];
					newStep->SetName(row[3]);

					//stepType = row[4];
					newStep->SetType(row[4]);

					//	inspecFailProb = row[5];
					newStep->SetInspecFailProb(row[5]);

					istringstream ssSteps3(row[6]);
					ssSteps3 >> returnStep;
					newStep->SetReturnStep(returnStep);

					//stepDur = row[7];
					newStep->SetServiceTime(row[7]);

					//reqResource = row[8];
					newStep->SetReqResource(row[8]);

					istringstream ssSteps4(row[9]);
					ssSteps4 >> numResources;

					//STORE RESOURCE INTO MAP BELONGING TO STEP


					//reqParts = row[10];
					newStep->SetReqParts(row[10]);

					if (row.size() == 12)
					{
						istringstream ssSteps5(row[11]);
						ssSteps5 >> numParts;
					}

			//		map<string, Resource*>::const_iterator resPoolIter = Step::_resourcePool.begin();

					map<string, Aircraft*>::const_iterator iter = _masterMap.begin();

					//ADDING STEPS INTO REPAIR JOB OF EVERY AIRCRAFT
					//for every aircraft object in the master map
					while (iter != _masterMap.end())
					{

						map<string, RepairJob*>::iterator it = iter->second->GetRJMapBegin();

						while(it != iter->second->GetRJMapEnd())
						{
							// create object, get the name of the repair job in the aircraft object and check that it exists
							if (it->second->GetName() == currentJob)
							{
								//cout << " about to add a step \n" << endl;
								it->second->AddStep(newStep);
								it++;
							}
							else
								it++;
						}
						iter++;
						
					}

				}
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
					//Resource footprint

			//Resource Failure Table
			// For each of the resources in Step object's vector
				//Match resource name string to string in table
					//Find:
					//Failure Name
					//Failure Type
					//Failure Distribution

			if (line.find("Resource Table") != string::npos) {
				printf("got Resource Table \n");

				string resName;
				int resCount;
				double resourceFootprintX;
				double resourceFootprintY;

				getline(dataFile, line);
				//reading row as string
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

					resName = row[0];

					istringstream ssResource1(row[1]);
					ssResource1 >> resCount;

					istringstream ssResource2(row[2]);
					ssResource2 >> resourceFootprintX;

					istringstream ssResource3(row[3]);
					ssResource3 >> resourceFootprintY;
		
					map<string, Aircraft*>::const_iterator masterIter = _masterMap.begin();
					//ITERATE THROUGH MASTER MAP
					while (masterIter != _masterMap.end())
					{
						//ITERATE THROUGH REPAIR JOBS
						map<string, RepairJob*>::const_iterator iter = masterIter->second->GetRJMapBegin();
						while (iter != masterIter->second->GetRJMapEnd())
						{
							//ITERATE THROUGH THEIR STEPS
							for (int i = 0; i < iter->second->GetStepVecSize(); i++)
							{
								map<string, Resource*>::iterator it = iter->second->GetStep(i)->FindResource(resName);
								
								if (iter->second->GetStep(i)->IsResourceMapEnd(it))
									continue;
								
								it->second->SetResourceName(resName);
								it->second->SetResourceCount(resCount);
								it->second->SetResourceFootprint(resourceFootprintX, resourceFootprintY);
				//				it->second->PrintResProperties();
							}

							iter++;
						}

						masterIter++;
					}
				}

			}

			if (line.find("Resource Failure Table") != string::npos) {
				printf("got Resource Failure Table \n");

				string resName;

				getline(dataFile, line);
				//reading row as string
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
							//	cout << line << endl;
						}
						else
							getline(ss, line);
					}
					Resource* newRes = new Resource();
					//		cout << "fail vector size: " << row.size() << endl;

					resName = row[0];

					//cout << "DISTRIBUTION: " << row[3] << endl;

					//newRes->SetFailureName(row[1]);
					//newRes->SetFailureType(row[2]);
					//newRes->SetFailureDistr(row[3]);
					//newRes->SetRepairProcess(row[4]);

					RepairJob* resRepairJob = new RepairJob();
					resRepairJob->SetName(row[4]);
					
					resRepairJob->AddResourceRepair(resRepairJob, resName);

					
					//ADD VALUES OF RESOURCE TO STORED RESOURCE IN MAP BELONGING TO STEP
					map<string, Aircraft*>::const_iterator masterIter = _masterMap.begin();
					//ITERATE THROUGH MASTER MAP
					while (masterIter != _masterMap.end())
					{
						//ITERATE THROUGH REPAIR JOBS
						map<string, RepairJob*>::const_iterator iter = masterIter->second->GetRJMapBegin();
						while (iter != masterIter->second->GetRJMapEnd())
						{
							//ITERATE THROUGH THEIR STEPS
							for (int i = 0; i < iter->second->GetStepVecSize(); i++)
							{
								map<string, Resource*>::iterator it = iter->second->GetStep(i)->FindResource(resName);

								if (iter->second->GetStep(i)->IsResourceMapEnd(it))
									continue;

								it->second->SetFailureName(row[1]);
							//	cout << "ROW 1: " << row[1] << endl;
							//	cout << endl;
								it->second->SetFailureType(row[2]);
							//	cout << "ROW 3: " << row[3] << endl;
								it->second->SetFailureDistr(row[3]);
							//	cout << endl;
								it->second->SetRepairProcess(row[4]);
		//						it->second->PrintResProperties();
							}

							iter++;
						}

						masterIter++;
					}
				}

			}

			//////////////////////////////////////////
			///////////////   PARTS   ////////////////
			//////////////////////////////////////////

			//Parts Table
			// For each of the Step's required part
				//Match part name string to string in table
					//Find:
					//parts name [done]
					//Initial Quantity [done]
					//Replenishment Threshold [done]
					//Lead Time [done]

			if (line.find("Parts Table") != string::npos) {
				printf("got Parts Table \n");

				string partName;
				int count;
				int threshold;

				getline(dataFile, line);
				//reading row as string
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
							//	cout << line << endl;
						}
						else
							getline(ss, line);
					}
					Parts* newParts = new Parts();
					//		cout << "fail vector size: " << row.size() << endl;
					
					partName = row[0];
//					newParts->SetPartsName(partName);

					istringstream ssParts1(row[1]);
					ssParts1 >> count;
//					newParts->SetPartsCount(count);

					istringstream ssParts2(row[2]);
					ssParts2 >> threshold;
//					newParts->SetThreshold(threshold);

//					newParts->SetLeadTime(row[3]);

					//ADD VALUES OF PARTS TO STORED PARTS IN MAP BELONGING TO STEP
					map<string, Aircraft*>::const_iterator masterIter = _masterMap.begin();
					//ITERATE THROUGH MASTER MAP
					while (masterIter != _masterMap.end())
					{
						//ITERATE THROUGH REPAIR JOBS
						map<string, RepairJob*>::const_iterator iter = masterIter->second->GetRJMapBegin();
						while (iter != masterIter->second->GetRJMapEnd())
						{
							//ITERATE THROUGH THEIR STEPS
							for (int i = 0; i < iter->second->GetStepVecSize(); i++)
							{
								map<string, Parts*>::iterator it = iter->second->GetStep(i)->FindParts(partName);

								if (iter->second->GetStep(i)->IsPartsMapEnd(it))
									continue;

								it->second->SetPartsName(partName);
								it->second->SetPartsCount(count);
								it->second->SetThreshold(threshold);
								it->second->SetLeadTime(row[3]);
	//							it->second->PrintPartsProperties();
							}

							iter++;
						}

						masterIter++;
					}


				}

			}


			map<string, Aircraft*>::const_iterator iter = _masterMap.begin();

			while (iter != _masterMap.end())
			{
//				iter->second->PrintProperties();
				iter++;
			}

		}

		dataFile.close();
		cout << "closed file" << endl;
		cout << "_________________________________________________________________________________" << endl;
	}
	else
	{
		cout << "CSV file didn't open!";
	}


	map<string, Aircraft*>::iterator it = _masterMap.begin();
	//	cout << "////////////////////////////////////////////////////// \n";
	//	cout << "////////////////////////////////////////////////////// \n";
	while (it != _masterMap.end())
	{
		//		it->second->PrintProperties();
		it++;
	}
}

void InputReader::PrintEverything()
{
	//Aircraft
	map<string, Aircraft*>::const_iterator iter = _masterMap.begin();

	while (iter != _masterMap.end())
	{
		iter->second->PrintProperties();

		//for loop to get each repair job
		cout << "_________________________________________________________________________________" << endl;
		cout << "_________________________________________________________________________________" << endl;
		iter++;
	}
}