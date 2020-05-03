//InputInterface.cpp: Andrea Robey

#include "InputInterface.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

map<string, Aircraft*> InputReader::_masterMap;
map<string, StepResource*> InputReader::_masterResourceMap;
map<string, Parts*> InputReader::_masterPartsMap;
int InputReader::_numRuns;
int InputReader::_airCount;
int InputReader::_IDcount;
double  InputReader::_shiftOneStartTime;
double  InputReader::_shiftTwoStartTime;
double  InputReader::_shiftThreeStartTime;
int InputReader::_wartimeFlag;


struct InputReader::GUISelectedAircraft {
	GUISelectedAircraft(string aircraftName) {
		_aircraftName = aircraftName;
		_nextAircraft = NULL;
	}
	string _aircraftName;
	GUISelectedAircraft* _nextAircraft;
};

InputReader::InputReader()
{
	calConvert = new CalConverter();
}

InputReader::~InputReader()
{/*
	// Iterate through the whole map and deallocate all pointer objects
	map <string, Aircraft*>::const_iterator airIt = _masterMap.begin();
	// for each aircraft element
	while (airIt != _masterMap.end())
	{
		//for each repairjob element
		map <string, RepairJob*>::const_iterator rjIt = airIt->second->GetRJMapBegin();
		while (rjIt != airIt->second->GetRJMapEnd())
		{
			//for each step element
			for (int i = 0; i < rjIt->second->GetStepVecSize(); i++)
			{
				//for each resource element
				map <string, Resource*>::const_iterator resIt = rjIt->second->GetStep(i+1)->GetResourceMapBegin();
				while (resIt != rjIt->second->GetStep(i+1)->GetResourceMapEnd())
				{
					Resource* toDelete = resIt->second;
					delete toDelete;
				}
				//for each part element
				map <string, Parts*>::const_iterator partsIt = rjIt->second->GetStep(i+1)->GetPartsMapBegin();
				while (partsIt != rjIt->second->GetStep(i+1)->GetPartsMapEnd())
				{
					Parts* toDelete = partsIt->second;
					delete toDelete;
				}
				Step* toDelete = rjIt->second->GetStep(i+1);
				delete toDelete;
			}
			RepairJob* toDelete = rjIt->second;
			delete toDelete;
		}
		Aircraft* toDelete = airIt->second;
		delete toDelete;*/
		//}
}

void InputReader::ReadInputData() //initialization for getting data
{
	//CalConverter calConvert;
	//Step step;
	StepResource resource;
	string line;

	ifstream dataFile("SWAS_Input_Final.csv");
	//ifstream dataFile("SWASInputData_Chris.csv");
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
				//Make loop for main[Done]

			if (line.find("Run Info Table") != string::npos) {
				printf("got Run Info table \n");

				int numRuns;
				string seedType;

				getline(dataFile, line);

				if (line != ",,,,,,,,,," && line.find("Number of Runs") != string::npos) {
					dataFile >> numRuns >> c >> seedType;
					SetNumRuns(numRuns);

					//removed 10 commas from string
					seedType = seedType.erase(seedType.length() - 9);

					//cout << "num of runs: " << numRuns << " seed type: " << seedType << endl;

					Distribution::SetSystemSeedType(seedType);
					//in distribution file, there will be an if statement that determines whether 
					//the operator(generator) function will be called or if the seed version
				}
				getline(dataFile, line);
			}

			//////////////////////////////////////////
			///////////    MISSION TYPE    ///////////
			//////////////////////////////////////////
			
			//Find Mission Type (Wartime or Peacetime)
			//get line
				//if line = Wartime
					//set IsWartTime = true;
					//Find Wartime Shifts table
						//set shift1startTime
						//set shift2startTime
				//else if line = Peacetime
					//Find Peacetime Shifts table
						//set shift1startTime
						//set shift2startTime
						//set shift3startTime

			if (line.find("Mission Type Table (Wartime or Peacetime)") != string::npos)
			{
				printf("got Mission Type table \n");

				getline(dataFile, line);

				line = line.erase(line.length() - 10);

				if (line == "Wartime" || line == "wartime")
				{
					cout << "wartime!" << endl;
					SetWartimeFlag(1);
				}
				else
				{
					SetWartimeFlag(0);
				}
	
				double shift1start;
				double shift2start;
				double shift3start;

				if (IsWartime() == true)
				{

					//cout << "war time!" << endl;
					while (dataFile)
					{
						if (line.find("Wartime Shifts Table") != string::npos)
						{
							printf("got Wartime Shifts table \n");
							
							getline(dataFile, line);

							istringstream ss(line);
							ss >> shift1start;
							//cout << "shift 1 starts at ";
							SetShiftOneStartTime(shift1start);
							//cout << GetShiftOneStartTime() << endl;

							getline(dataFile, line);

							istringstream ss2(line);
							ss2 >> shift2start;
							//cout << "shift 2 starts at ";
							SetShiftTwoStartTime(shift2start);
							//cout << GetShiftTwoStartTime() << endl;
							break;
						}
						else
							getline(dataFile, line);
					}
				}
				else
				{
					//cout << "peace time!" << endl;
					while (dataFile)
					{
						if (line.find("Peacetime Shifts Table") != string::npos)
						{
							printf("got Peacetime Shifts table \n");

							getline(dataFile, line);

							stringstream ss(line);
							ss >> shift1start;
							//cout << "shift 1 starts at ";
							SetShiftOneStartTime(shift1start);
							//cout << GetShiftOneStartTime() << endl;

							getline(dataFile, line);

							stringstream ss2(line);
							ss2 >> shift2start;
							//cout << "shift 2 starts at ";
							SetShiftTwoStartTime(shift2start);
							//cout << GetShiftTwoStartTime() << endl;

							getline(dataFile, line);

							stringstream ss3(line);
							ss3 >> shift3start;
							//cout << "shift 3 starts at ";
							SetShiftThreeStartTime(shift3start);
							//cout << GetShiftThreeStartTime() << endl;
							break;
						}
						else
							getline(dataFile, line);
					}
				}
			}
			//}




			/*if (_wartimeFlag == 1)
			{
				getline(dataFile, line);
				getline(dataFile, line);

				if (line.find("Shifts Table") != string::npos) 
				{
					printf("got Shifts table \n");
						
					getline(dataFile, line);
					getline(dataFile, line);

					stringstream ss(line);
					ss >> shift1start;
					cout << "shift 1 starts at " ;
					SetShiftOneStartTime(shift1start);
					cout << GetShiftOneStartTime() << endl;

					getline(dataFile, line);

					stringstream ss2(line);
					ss2 >> shift2start;
					cout << "shift 2 starts at ";
					SetShiftTwoStartTime(shift2start);
					cout << GetShiftTwoStartTime() << endl;
				}
			}
			else
			{					
				if (line.find("Peacetime Shifts") != string::npos) 
				{
					printf("got Peacetime Shifts table \n");

					getline(dataFile, line);
					getline(dataFile, line);

					stringstream ss(line);
					ss >> shift1start;
					cout << "shift 1 starts at ";
					SetShiftOneStartTime(shift1start);
					cout << GetShiftOneStartTime() << endl;

					getline(dataFile, line);

					stringstream ss2(line);
					ss2 >> shift2start;
					cout << "shift 2 starts at ";
					SetShiftTwoStartTime(shift2start);
					cout << GetShiftTwoStartTime() << endl;

					getline(dataFile, line);

					stringstream ss3(line);
					ss2 >> shift3start;
					cout << "shift 3 starts at ";
					SetShiftThreeStartTime(shift3start);
					cout << GetShiftThreeStartTime() << endl;
				}
			}*/
			

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
					if (month == 0)
						break;
					str >> numDays;

					//	cout << "month " << month << " days, " << numDays << endl; 

					calConvert->InsertDays(month, numDays);
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
				
				//while not the end of the table
				while (line != ",,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,")
						break;
					istringstream ss(line);
					
					////parsing the whole row and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,") {
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
					int aircraftNumber;

					Aircraft* newAir = new Aircraft();

					//	for (int i = 0; i < row.size(); ++i) {
						//	cout << "line " << i << ": " << row[i] << endl;
					//	}

						//for each index, set each variable
						//	istringstream iss0(row[0]);
					airType = row[0];
					//cout << "ROW " << row[0];
					newAir->SetAircraftType(airType);

					istringstream iss1(row[1]);
					iss1 >> airPriority;
					newAir->SetAircraftPriority(airPriority);

					istringstream iss2(row[2]);
					iss2 >> airLength;

					istringstream iss3(row[3]);
					iss3 >> airWingspan;

					newAir->SetAircraftFootprint(airLength, airWingspan);

					newAir->SetBaySizeReq(row[4]);

					istringstream iss5(row[5]);
					iss5 >> aircraftNumber;

					_addedAircraft.insert(pair<int, string>(aircraftNumber, airType));
					_masterMap.insert(pair<string, Aircraft*>(airType, newAir));
				}
			}


			if (line.find("Unplanned Arrivals Table") != string::npos) {
				printf("got Unplanned Arrivals Table \n");

				string unplannedType;
				string unplannedIAT;

				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,")
				{
					row.clear();
					getline(dataFile, line);
					if (line == ",,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,") {
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


					unplannedType = row[0];

					unplannedIAT = row[1];
					//compare to string
					map<string, Aircraft*>::const_iterator iter = _masterMap.find(unplannedType);
					if (iter == _masterMap.end()) {
						printf("end of map \n");
						cout << "unplanned type: " << unplannedType << endl;
					}

					iter->second->SetAircraftIAT(unplannedIAT);
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
				int numPlanned = 0;

				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,") {
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

					plannedType = row[0];

					repairName = row[1];
					newJob->SetName(repairName);

					schedType = row[2];

					newJob->SetSchedType(schedType);

					if (schedType == "Calendar") {
						schedCal = row[3];
						newJob->SetCalendarDate(schedCal);
					}

					else if (schedType == "Recurring") {

						istringstream unss3(row[3]);
						unss3 >> schedRecur;
						newJob->SetRecurringAmt(schedRecur);

						map<string, Aircraft*>::const_iterator iter = _masterMap.find(plannedType);
						if (iter != _masterMap.end())
						{
							iter->second->AddRecurIAT(repairName, newJob->GetRecurringAmt());

							iter++;
						}

					}

					istringstream unss4(row[4]);
					unss4 >> indoorReq;
					newJob->SetIndoorReq(indoorReq);

					_masterMap[plannedType]->AddRepairJob(newJob, repairName);
					numPlanned++;
				}
				Scribe::SetPlanned(numPlanned);
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

				while (line != ",,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,") {
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

					unplanType = row[0];

					unRepairName = row[1];
					newJob->SetName(unRepairName);

					repJobProb = row[2];
					newJob->SetUnplannedProb(repJobProb);

					istringstream unss(row[3]);
					unss >> unIndoorReq;
					newJob->SetIndoorReq(unIndoorReq);

					if (unplanType == "All")
					{
						map<string, Aircraft*>::iterator it = _masterMap.begin();
						while (it != _masterMap.end())
						{
							RepairJob* anotherOne = new RepairJob(*newJob);
							it->second->AddRepairJob(anotherOne, unRepairName);
							it++;
						}
					}
					else
					{
						RepairJob* anotherOne = new RepairJob(*newJob);
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
				string stepNameTemp;
				string stepType;
				string inspecFailProb;
				int returnStep;
				Distribution* stepDurTemp = 0;
				string stepDur;
				string reqResource;
				string reqParts;
				int numParts;
				int numResources;

				int rowCounter = 0;

				getline(dataFile, line);
				vector <string> row;

				while (line != ",,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);

					if (line == ",,,,,,,,,,")
						break;
					istringstream iss(line);

					////parsing the whole file and storing individual strings
					while (iss)
					{
						//csv empty cell has 10 commas
						if (line != ",,,,,,,,,,") {


							////breaking up strings by comma
							if (!getline(iss, line, ','))
								break;

							if (line.empty()) {
								//	break;
							}

							row.push_back(line);
						}
						else
							getline(iss, line);
					}

					Step* newStep = new Step(stepDurTemp, stepNameTemp);
					StepResource* newRes = new StepResource();
					Parts* newParts = new Parts();

					//if on the first line of the table, current job is first line, row[0] and job priority is first line, row[1]
					if (rowCounter == 0) {
						currentJob = row[0];

						istringstream ssSteps(row[1]);
						ssSteps >> jobPriority;

						rowCounter++;
					}
					//if new line's job name value isn't empty, new job name value is current job
					if (row[0] != "")
					{
						currentJob = row[0];
					}
					//if new line's priority value isn't empty, new prioruty value is current priority
					if (row[1] != "")
					{
						istringstream ssSteps(row[1]);
						ssSteps >> jobPriority;
					}

					newStep->SetMyRJName(currentJob);

					newStep->SetRJPriority(jobPriority);

					//compare jobname to insert priority to correct map location
					istringstream ssSteps2(row[2]);
					ssSteps2 >> stepID;

					newStep->SetName(row[3]);

					newStep->SetType(row[4]);

					newStep->SetInspecFailProb(row[5]);

					istringstream ssSteps3(row[6]);
					ssSteps3 >> returnStep;
					//cout << "OH job is " << currentJob << " return step is " << row[6] << " TRANSLATED TO " << returnStep << endl;
					newStep->SetReturnStep(returnStep);

					newStep->SetServiceTime(row[7]);

					map<string, Aircraft*>::const_iterator iter = _masterMap.begin();

					//ADDING STEPS INTO REPAIR JOB OF EVERY AIRCRAFT
					//for every aircraft object in the master map
					while (iter != _masterMap.end())
					{
						map<string, RepairJob*>::iterator it = iter->second->GetRJMapBegin();

						while (it != iter->second->GetRJMapEnd())
						{
							if (it->second->GetName() == currentJob)
							{
								newStep->SetReqResource(row[8]);

								if (row.size() == 11)
								{
									istringstream ssSteps5(row[10]);
									ssSteps5 >> numParts;
									newStep->SetReqParts(row[9], numParts);
								}

								//add this step to the repair job's list
								it->second->AddStep(newStep);

							}

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
					//Resource footprint [done]

			//Resource Failure Table
			// For each of the resources in Step object's vector
				//Match resource name string to string in table
					//Find:
					//Failure Name [done]
					//Failure Type [done]
					//Failure Distribution [done]

			if (line.find("Resource Table") != string::npos) {
				printf("got Resource Table \n");

				string resName;
				double resCountShift1;
				double resCountShift2;
				double resCountShift3;
				double resourceFootprintX;
				double resourceFootprintY;

				getline(dataFile, line);
				//reading row as string
				vector <string> row;

				while (line != ",,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,") {
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
					ssResource1 >> resCountShift1;

					istringstream ssResource2(row[2]);
					ssResource2 >> resCountShift2;
					
					
					//cout << row[2] << " " << resCountShift2 << endl;

					if (row.size() == 6)
					{
						istringstream ssResource3(row[3]);
						ssResource3 >> resCountShift3;

						istringstream ssResource4(row[4]);
						ssResource4 >> resourceFootprintX;

						istringstream ssResource5(row[5]);
						ssResource5 >> resourceFootprintY;
					}
					else
					{
						istringstream ssResource3(row[3]);
						ssResource3 >> resourceFootprintX;

						istringstream ssResource4(row[4]);
						ssResource4 >> resourceFootprintY;
					}

					//istringstream ssResource2(row[2]);
					//ssResource2 >> resourceFootprintX;

					//istringstream ssResource3(row[3]);
					//ssResource3 >> resourceFootprintY;

					StepResource* res = new StepResource();
					res->SetResourceName(resName);
					res->SetResourceCount(resCountShift1);

					res->SetShiftOneCount(resCountShift1);
					res->SetShiftTwoCount(resCountShift2);
					res->SetShiftThreeCount(resCountShift3);
					res->SetResourceFootprint(resourceFootprintX, resourceFootprintY);


					if (resName == "S Bay" || resName == "M Bay" || resName == "L Bay")
					{
						res->SetNumResNeeded(1);
						Step::AddToResPool(res, resName);
					}

					_masterResourceMap.insert(pair<string, StepResource*>(resName, res));

					
					//TODO UPDATE SCRIBE FOR SHIFT COUNTS
					Scribe::RecordResource(resName, resCountShift1);

					//Step::AddToResPool(res, res->GetResourceName());

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
								map<string, StepResource*>::iterator it = iter->second->GetStep(i + 1)->FindResource(resName);

								if (iter->second->GetStep(i + 1)->IsResourceMapEnd(it))
									continue;
								it->second->SetResourceName(resName);
								SetMasterResNum(resName, iter->second->GetStep(i + 1)->GetResourceObj(resName)->GetNumberOfResroucesNeeded());
								it->second->SetResourceCount(resCountShift1);
								if (IsWartime() == true)
								{
									//cout << "its true" << endl;
									it->second->SetShiftOneCount(resCountShift1);
									it->second->SetShiftTwoCount(resCountShift2);
									it->second->SetShiftThreeCount(0);
								}
								else 
								{
									it->second->SetShiftOneCount(resCountShift1);
									it->second->SetShiftTwoCount(resCountShift2);
									it->second->SetShiftThreeCount(resCountShift3);
								}
								it->second->SetResourceFootprint(resourceFootprintX, resourceFootprintY);
								//it->second->PrintResProperties();
								//cout << "RES MAP SIZE " << iter->second->GetStep(i + 1)->GetResMapSize() << endl;
								/*cout << it->second->GetResourceName() << " count 1 " << it->second->GetShiftOneCount() << endl;
								cout << it->second->GetResourceName() << " count 2 " << it->second->GetShiftTwoCount() << endl;
								cout << it->second->GetResourceName() << " count 3 " << it->second->GetShiftThreeCount() << endl;*/

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

				while (line != ",,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,") {
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
					StepResource* newRes = new StepResource();

					resName = row[0];

					RepairJob* resRepairJob = new RepairJob();
					resRepairJob->SetName(row[4]);

					resRepairJob->AddResourceRepair(resRepairJob, resName);



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
								//map<string, Resource*>::iterator it = iter->second->GetStep(i + 1)->FindResource(row[0]);
								map<string, StepResource*>::iterator it = iter->second->GetStep(i + 1)->GetResourceMapBegin();
								while (it != iter->second->GetStep(i + 1)->GetResourceMapEnd())
									/*if (iter->second->GetStep(i + 1)->IsResourceMapEnd(it))
										continue;*/
								{

									iter->second->GetStep(i + 1)->SetStepIndoorReq(iter->second->GetIndoorReq());
									iter->second->GetStep(i + 1)->SetRJPriority(iter->second->GetPriority());

									if (it->first == row[0])
									{
										it->second->SetFailureName(row[1]);
										it->second->SetFailureType(row[2]);
										it->second->SetFailureDistr(row[3]);
										it->second->SetRepairProcess(row[4]);
									}

									map<string, StepResource*>::iterator masterResIt;
									masterResIt = _masterResourceMap.find(row[0]);
									if (masterResIt != _masterResourceMap.end())
									{
										masterResIt->second->SetFailureName(row[1]);
										masterResIt->second->SetFailureType(row[2]);
										masterResIt->second->SetFailureDistr(row[3]);
										masterResIt->second->SetRepairProcess(row[4]);
									}

									it++;
								}
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

				while (line != ",,,,,,,,,,")
				{
					row.clear();

					getline(dataFile, line);
					if (line == ",,,,,,,,,,")
						break;
					istringstream ss(line);

					////parsing the whole file and storing individual strings
					while (ss)
					{
						//csv empty cell has 11 commas
						if (line != ",,,,,,,,,,") {
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
					newParts->SetPartsName(partName);

					istringstream ssParts1(row[1]);
					ssParts1 >> count;
					newParts->SetPartsCount(count);
					newParts->SetInitPartsCount(count);

					istringstream ssParts2(row[2]);
					ssParts2 >> threshold;
					newParts->SetThreshold(threshold);

					newParts->SetLeadTime(row[3]);
					_masterPartsMap.insert(pair<string, Parts*>(partName, newParts));


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
								map<string, Parts*>::iterator it = iter->second->GetStep(i + 1)->FindParts(partName);

								if (iter->second->GetStep(i + 1)->IsPartsMapEnd(it))
									continue;
								it->second->SetPartsName(partName);
								it->second->SetPartsCount(count);
								it->second->SetInitPartsCount(count);
								SetMasterPartsNum(partName, iter->second->GetStep(i + 1)->GetPartsObj(partName)->GetNumPartsNeeded());
								it->second->SetThreshold(threshold);
								it->second->SetLeadTime(row[3]);

								//it->second->PrintPartsProperties();

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

	while (it != _masterMap.end())
	{

		//		it->second->PrintProperties();
		it++;
	}

	map<string, Parts*>::const_iterator partsIter = _masterPartsMap.begin();
	while (partsIter != _masterPartsMap.end())
	{
		map<string, Parts*> tempMap = Step::GetPartsPool();
		if (tempMap.find(partsIter->first) == tempMap.end())
		{
			Parts* partCopy = new Parts();
			partCopy->CopyMapParts(*partsIter->second);
			Step::AddToPartsPool(partCopy, partsIter->first);
		}
		partsIter++;
	}


	map<string, StepResource*>::const_iterator resIter = _masterResourceMap.begin();
	while (resIter != _masterResourceMap.end())
	{
		map<string, StepResource*> tempMap = Step::GetResPool();
		if (tempMap.find(resIter->first) == tempMap.end())
		{
			//cout << "in input inter for pools " << resIter->second->GetNumberOfResroucesNeeded() << endl;
			StepResource* resCopy = new StepResource();
			resCopy->CopyMapResource(*resIter->second);
			//cout << "IN RES COPY, NUM NEEDED IS " << resCopy->GetNumberOfResroucesNeeded() << endl;
			Step::AddToResPool(resCopy, resIter->first);
		}
		resIter++;
	}

	//cout << "RES POOL SIZE IS " << Step::GetResPoolSize()<< endl;
	//cout << "PARTS POOL SIZE IS " << Step::GetPartsPoolSize()<< endl;

	//cout << "PRINTING POOLS" << endl;
	//Step::PrintPools();


}


map<string, Aircraft*>::iterator InputReader::GetMasterMapBegin()
{
	return _masterMap.begin();
}

map<string, Aircraft*>::iterator InputReader::GetMasterMapEnd()
{
	return _masterMap.end();
}

void InputReader::SetNumRuns(int numRuns)
{
	_numRuns = numRuns;
}

int InputReader::GetNumRuns()
{
	return _numRuns;
}

int InputReader::GetAirCount()
{
	cout << " AIRCRAFT COUNT IS " << _airCount << endl;
	return _airCount;
}

void InputReader::AddAirCount()
{
	_airCount++;
}

int InputReader::GetIDcount()
{
	//cout << "****Number of 'read in' Aircraft IDs " << _IDcount << endl;
	return _IDcount;
}

void InputReader::CalAirFix() {
	_airCount--;
}

map<string, StepResource*>::iterator InputReader::GetMasterResMapBegin()
{
	return _masterResourceMap.begin();
}

map<string, StepResource*>::iterator  InputReader::GetMasterResMapEnd()
{
	return _masterResourceMap.end();
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

void InputReader::SetShiftOneStartTime(int starttime)
{
	_shiftOneStartTime = starttime;
}

void InputReader::SetShiftTwoStartTime(int starttime)
{
	_shiftTwoStartTime = starttime;
}

void InputReader::SetShiftThreeStartTime(int starttime)
{
	_shiftThreeStartTime = starttime;
}

double InputReader::GetShiftOneStartTime()
{
	return _shiftOneStartTime;
}

double InputReader::GetShiftTwoStartTime()
{
	return _shiftTwoStartTime;
}

double InputReader::GetShiftThreeStartTime()
{
	return _shiftThreeStartTime;
}

bool InputReader::IsWartime()
{
	if (_wartimeFlag == 1)
	{
		//cout << "war time" << endl;
		return true;
	}
	else
	{
		//cout << "not war time" << endl;
		return false;
	}
}

void InputReader::SetWartimeFlag(int flag)
{
	_wartimeFlag = flag;
}

void InputReader::PrintMasterResMap()
{
	map<string, StepResource*>::const_iterator iter = _masterResourceMap.begin();

	while (iter != _masterResourceMap.end())
	{
		cout << "__________" << endl;
		cout << iter->first << endl;
		iter->second->PrintResProperties();
		iter++;
	}
}

CalConverter* InputReader::GetCalConverter()
{
	return calConvert;
}

//void InputReader::AddSelectedAircraft(string aircraftName)
void InputReader::AddSelectedAircraft(int aircraftNumber)
{
	_IDcount++;
	map<int, string>::const_iterator iter = _addedAircraft.find(aircraftNumber);
	GUISelectedAircraft* newAircraft = new GUISelectedAircraft(iter->second);
	//GUISelectedAircraft* newAircraft = new GUISelectedAircraft(aircraftName);

	if (_GUIListHead == NULL) {
		_GUIListHead = newAircraft;
	}
	else {
		GUISelectedAircraft* iter = _GUIListHead;
		while (iter->_nextAircraft != NULL)
		{
			iter = iter->_nextAircraft;
		}
		iter->_nextAircraft = newAircraft;
	}

}

bool InputReader::FindSelectedAircraft(string aircraftName)
{
	GUISelectedAircraft* iter = _GUIListHead;
	while (iter != NULL)
	{
		if (iter->_aircraftName == aircraftName) {
			return true;
		}
		else
		{
			iter = iter->_nextAircraft;
		}
	}
	return false;
}

int InputReader::GetMapSize()
{
	return _masterMap.size();
}

Aircraft* InputReader::GetAircraft(string aircraftName)
{

	map<string, Aircraft*>::const_iterator iter = _masterMap.find(aircraftName);
	if (iter != _masterMap.end())
	{
		return iter->second;
	}
	else
		return nullptr;
}

void InputReader::SetMasterResNum(string name, int num)
{
	map<string, StepResource*>::const_iterator iter = _masterResourceMap.begin();
	while (iter != _masterResourceMap.end())
	{
		_masterResourceMap.find(name)->second->SetNumResNeeded(num);
		iter++;
	}
}

void InputReader::SetMasterPartsNum(string name, int num)
{
	map<string, Parts*>::const_iterator iter = _masterPartsMap.begin();
	while (iter != _masterPartsMap.end())
	{
		_masterPartsMap.find(name)->second->SetNumPartsNeeded(num);
		iter++;
	}
}

map<string, Aircraft*> InputReader::GetMasterMap()
{
	return _masterMap;
}

map<string, StepResource*> InputReader::GetMasterResourceMap()
{
	return _masterResourceMap;
}

map<string, Parts*> InputReader::GetMasterPartsMap()
{
	return _masterPartsMap;
}

