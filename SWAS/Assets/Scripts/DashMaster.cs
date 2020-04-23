using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DashMaster : MonoBehaviour
{
    [SerializeField]
    private string inputFileName;

    //OutputFields
    [SerializeField]
    private Text titleText;
    [SerializeField]
    private Text field1;
    [SerializeField]
    private Text field2;
    [SerializeField]
    private Text field3;
    [SerializeField]
    private Text field4;
    //[SerializeField]
    private TextAsset inputFile;

    private string[] fileLines;

    //set of indicies used to read over sections
    private int aircraftStart;
    private int missionStart;
    private int resourceStart;
    private int resourceFailureStart;
    private int resourceWaitStart;
    private int aircraftWaitStart;
    private int repairJobStart;
    private int reworkStart;
    private int partsStart;
    private int restockStart;

    //Universal Metrics
    private int runNumber;

    // Start is called before the first frame update
    void Start()
    {
        //read in file and divide into lines
        
        inputFile = Resources.Load<TextAsset>("DashboardParamProposal");

        //Delete above line and use 
        //SetInputFile(inputFileName);

        fileLines = inputFile.text.Split(new char[] { '\n' });

       

        //index fields
        for(int i = 0; i < (fileLines.Length - 1); i++)
        {
            //split lines into individual string elements
            string[] row = fileLines[i].Split(new char[] { ',' });

            //Obtain Simulation Runs
            if(row[0] == "Number of Runs")
            {
                runNumber = int.Parse(row[1]);
            }


            //Set Appropriate index (beginning not indexed: 0)
            if (row[0] == "Aircraft")
            {
                aircraftStart = i;
            }

            if(row[0] == "Missions")
            {
                missionStart = i;
            }

            if(row[0] == "Resources")
            {
                resourceStart = i;
            }

            if(row[0] == "Resource Failure")
            {
                resourceFailureStart = i;
            }

            if(row[0] == "Resource Waits")
            {
                resourceWaitStart = i;
            }

            if(row[0] == "Aircraft Wait")
            {
                aircraftWaitStart = i;
            }

            if(row[0] == "Repair Jobs")
            {
                repairJobStart = i;
            }

            if(row[0] == "Reworks")
            {
                reworkStart = i;
            }

            if(row[0] == "Parts Requests")
            {
                partsStart = i;
            }

            if(row[0] == "Restocking")
            {
                restockStart = i;
            }

        }

        Overview();
    }

    //set of statistics for "Overview"
    public void Overview()
    {
        //Overview Metrics
        string warehouseWidth = "";
        string warehouseLength = "";
        string runTime = "";
       
        int aircraftTotal = 0;
        int aircraftAverage;

        int aircraftTypeSize = ((missionStart - 2) - (aircraftStart + 3));
        int missionSize = ((resourceStart - 1) - (missionStart + 2));


        //C# dynamic arrays
        string[,] aircraftTypes = new string[runNumber, aircraftTypeSize];
        int[,] aircraftNumber = new int[runNumber, aircraftTypeSize];
        string[,] missions = new string[runNumber, missionSize];
        
        titleText.text = "  Overview";

        //Read data into local variables
        for (int i = 0; i < (aircraftStart - 1); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });

            //Simulation Information
            if (row[0] == "Warehouse Length")
            {
                warehouseLength = row[1];
            }

            if (row[0] == "Warehouse Width")
            {
                warehouseWidth = row[1];
            }

            if (row[0] == "Runtime Duration")
            {
                runTime = row[1];
            }

        }
            
        //Populate Aircraft Array
        for (int i = (aircraftStart+3); i < (missionStart-2); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for (int j = 0; j < runNumber; j++)
            {
                aircraftTypes[j, i - (aircraftStart + 3)] = row[j * 2];
                aircraftNumber[j, i - (aircraftStart + 3)] = int.Parse(row[j * 2 + 1]);
                aircraftNumber[j, aircraftTypeSize-1] += aircraftNumber[j, i - (aircraftStart + 3)]; //update total
            }
        }

        //Populate Mission Array
        for(int i = (missionStart+2); i <(resourceStart-1); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j < runNumber; j++)
            {
                missions[j, (i - (missionStart + 2))] = row[j];
            }
        }

        //minor math
        for(int j = 0; j < runNumber; j++)
        {
            aircraftTotal += aircraftNumber[j, aircraftTypeSize-1];
        }

        aircraftAverage = aircraftTotal / runNumber;

        //Quadrant1 Text
        field1.text = "Warehouse Information:\n" +
                      "  Length: " + warehouseLength + "\n" +
                      "  Width: " + warehouseWidth + "\n" +
                      "  Runs: " + runNumber + "\n" +
                      "  Runtime: " + runTime + "\n" +
                      "  Throughput: " + aircraftTotal+"\n" +
                      "     Average: " + aircraftAverage;

        //Quadrant2 Text

        //Set Headings
        field2.text = "Aircraft:\n" +
                      "  Type";
        for(int j = 0; j < runNumber; j++)
        {
            field2.text = field2.text + "\tRun " + (j+1);
        }
        field2.text = field2.text + "\n";

        //create rows
        for(int i = 0; i < missionSize; i++)
        {
            field2.text += aircraftTypes[1, i];
            for(int j = 0; j < runNumber; j++)
            {
                field2.text = field2.text + "\t" + aircraftNumber[j, i];
            }
            field2.text += "\n";
        }

        //Quadrant3 Text
        field3.text = "Missions:\n";
        for(int j = 0; j < runNumber; j++)
        {
            field3.text = field3.text + "Run " + (j+1) + "\t";
        }
        field3.text += "\n";
        for(int i = 0; i < ((resourceStart - 1) - (missionStart + 2)); i++)
        {
            for(int j = 0; j < runNumber; j++)
            {
                field3.text = field3.text + missions[j, i] + "\t";
            }
            field3.text += "\n";
        }


        //Quadrant4 Text
        field4.text = "";
    }

    //Set of Statistics for "Availability"
    public void Availability()
    {
        //Availability Metrics
        float resourceWaitAverage = 0;
        float aircraftWaitAverage = 0;

        int resourcesSize = ((resourceFailureStart - 1) - (resourceStart + 3));
        int resourceWaitSize = ((aircraftWaitStart - 1) - (resourceWaitStart + 3));
        int aircraftWaitSize = ((repairJobStart - 1) - (aircraftWaitStart + 3));

        //Availability Arrays
        var resources = new string[runNumber, resourcesSize];
        var resourceUsage = new float[(runNumber + 1), 3, resourcesSize];
        var resourceUtilization = new float[(runNumber + 1), 2, resourcesSize];
        var resourceWaits = new string[runNumber, 2 , resourceWaitSize];
        var resourceWaitTimes = new float[runNumber, resourceWaitSize];
        var aircraftWaitEvents = new string[runNumber, aircraftWaitSize];
        var aircraftWaitTimes = new float[runNumber, aircraftWaitSize];
        var aircraftWaitAverages = new float[(runNumber)];

        titleText.text = "  Availability";

        //Read Information

        //Read in Resources
        for(int i = resourceStart+3; i < (resourceFailureStart - 1); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j < runNumber; j++)
            {
                resources[j, (i - ( resourceStart + 3 ))] = row[j*6];
                resourceUsage[j, 0, (i - (resourceStart + 3 ) )] = float.Parse(row[(j * 6 +1)]);
                resourceUsage[runNumber, 0, (i - (resourceStart + 3))] += (resourceUsage[j, 0, (i - (resourceStart + 3))] / runNumber);
                resourceUsage[j, 1, (i - (resourceStart + 3))] = float.Parse(row[(j * 6 + 4)]);
                resourceUsage[runNumber, 1, (i - (resourceStart + 3))] += (resourceUsage[j, 1, (i - (resourceStart + 3))] / runNumber);
                resourceUsage[j, 2, (i - (resourceStart + 3))] = float.Parse(row[(j * 6 + 5)]);
                resourceUsage[runNumber, 2, (i - (resourceStart + 3))] += (resourceUsage[j, 2, (i - (resourceStart + 3))] / runNumber);
                resourceUtilization[j, 0, (i - (resourceStart + 3 ))] = float.Parse(row[(j * 6 + 2)]);
                resourceUtilization[runNumber,0, (i - (resourceStart + 3))] += (resourceUtilization[j, 0, (i - (resourceStart + 3))]/runNumber);
                resourceUtilization[j, 1, (i - (resourceStart + 3 ))] = float.Parse(row[(j * 6 + 3)]);
                resourceUtilization[runNumber, 1, (i - (resourceStart + 3))] += (resourceUtilization[j, 1, (i - (resourceStart + 3))] / runNumber);
            }
        }

        //Read in Resource Waits
        for(int i = resourceWaitStart +3; i < (aircraftWaitStart-1); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j < runNumber; j++)
            {
                resourceWaits[j, 0, (i- (resourceWaitStart + 3 ))] = row[(j * 4)];
                resourceWaits[j, 1, (i - (resourceWaitStart + 3 ))] = row[(j * 4 + 2)];
                resourceWaitTimes[j, (i - (resourceWaitStart + 3 ))] = float.Parse(row[j * 4 + 3]);
                //increment run's average wait time
                resourceWaitTimes[j, (resourceWaitSize - 1)] += (resourceWaitTimes[j, (i - (resourceWaitStart + 3 ))] / (resourceWaitSize));
            }
        }

        //Read in Aircraft Waits
        for(int i = (aircraftWaitStart+3); i < (repairJobStart-1);i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j < runNumber; j++)
            {
                aircraftWaitEvents[j, (i - (aircraftWaitStart + 3))] = row[j * 6 + 1];
                aircraftWaitTimes[j, (i - (aircraftWaitStart + 3))] = float.Parse(row[j * 6 + 5]);
                //increase run's average wait times
                aircraftWaitAverages[j] += (aircraftWaitTimes[j, (i - (aircraftWaitStart + 3))] / (aircraftWaitSize));
            }
        }


        //Minor math

        //Overall average for wait time due to resources
        for(int j = 0; j < runNumber; j++)
        {
            resourceWaitAverage += (resourceWaitTimes[j, (resourceWaitSize - 1)]/runNumber);
        }

        //Overall average for aircraft wait times
        for(int j = 0; j < runNumber; j++)
        {
            aircraftWaitAverage += (aircraftWaitAverages[j] / runNumber);
        }

        //Output Information

        //Quadrant 1
        field1.text = "Resources:\n" +
                      "  Resource";
        for(int j = 0; j < runNumber; j++)
        {
            field1.text += ("\tRun " + (j + 1));
        }

        field1.text += "\tAverage\n";

        for(int i = 0; i < resourcesSize; i++)
        {
            field1.text += resources[0, i];
            for(int j = 0; j < runNumber; j++)
            {
                field1.text += ( "\t"+resourceUsage[j, 0, i]);
            }
            field1.text += ("\t" + resourceUsage[(runNumber), 0, i] + "\n");
        }

        //Quadrant 2
        field2.text = "Utilization:\n" +
                      "Resource";
        for(int j = 0; j < runNumber; j++)
        {
            field2.text += ("\tRun " + (j + 1) + "\t");
        }
        field2.text += "\tAverages\n";

        for(int i = 0; i < resourcesSize; i++)
        {
            field2.text += resources[0, i];
            for(int j = 0; j < runNumber; j++)
            {
                field2.text += ("\t" + resourceUtilization[j, 0, i] + "\t" + resourceUtilization[j, 1, i]);
            }

            field2.text += ("\t" + resourceUtilization[runNumber, 0, i] + "\t" + resourceUtilization[runNumber, 1, i]+"\n");
        }

        //Quadrant 3
        field3.text = "Requests:\n" +
                      "\t";
        for(int j = 0; j < runNumber; j++)
        {
            field3.text += ("\tRun " + (j + 1)+"\t");
        }

        field3.text += "\tAverages\n";

        for(int i = 0; i < resourcesSize; i++)
        {
            field3.text += resources[0, i];

            for(int j = 0; j<runNumber; j++)
            {
                field3.text += ("\t" + resourceUsage[j, 1, i] + "\t" + resourceUsage[j, 2, i]);
            }

            field3.text += ("\t" + resourceUsage[runNumber, 1, i] + "\t" + resourceUsage[runNumber, 2, i]+"\n");
        }

        //Quadrant 4
        field4.text = "Wait Time Statistics:\n" +
                      "Resources\n";
        for(int j = 0; j < runNumber; j++)
        {
            field4.text += ("\tRun" + (j + 1));
        }
        field4.text += "\tAverage\n";

        for(int j = 0; j < runNumber;j++)
        {
            field4.text += ("\t" + resourceWaitTimes[j, (aircraftWaitSize - 1)]);
        }

        field4.text += ("\t" + resourceWaitAverage + "\n\nAircraft Service\n");

        for (int j = 0; j < runNumber; j++)
        {
            field4.text += ("\tRun" + (j + 1));
        }

        field4.text += "\tAverage\n";

        for (int j = 0; j < runNumber; j++)
        {
            field4.text += ("\t" + aircraftWaitAverages[j]);
        }

        field4.text += ("\t" + aircraftWaitAverage);

    }

    //Set of statistics for "Performance"
    public void Peformance()
    {
        //Performance Metrics
        float aircraftTotal = 0;
        float aircraftAverage;
        int reworkCount = ((partsStart-1)-(reworkStart+3));
        int runTime = 0;

        string plannedMaintenance = "";
        string unplannedMaintenance = "";

        int repairJobSize = ((reworkStart - 1) - (repairJobStart + 3));
        int resourceSize = ((resourceFailureStart - 1) - (resourceStart + 3));
        int aircraftSize = ((missionStart - 2) - (aircraftStart + 3));

        //Performance Arrays
        var repairJobSet = new string[runNumber, 2, repairJobSize];
        var repairJobTimes = new float[runNumber, repairJobSize];
        var resources = new string[runNumber, resourceSize];
        var resourceInit = new int[runNumber, resourceSize];
        var resourceDownTime = new float[runNumber, resourceSize];
        var resourceUptime = new float[runNumber, resourceSize];
        var aircraftNumbers = new float[runNumber, aircraftSize+1];
        var aircraftTypes = new string[runNumber, aircraftSize];
        var aircraftRepairAverage = new float[runNumber, aircraftSize];
        var aircraftAverageTime = new float[aircraftSize];

        titleText.text = "  Performance";

        //Read in Data

        for(int i = 0; i < (aircraftStart-1);i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            if(row[0] == "Number of Planned Repair Jobs")
            {
                plannedMaintenance = row[1];
            }

            else if (row[0] == "Number of Unplanned Repair Jobs")
            {
                unplannedMaintenance = row[1];
            }

            else if (row[0] == "Runtime Duration")
            {
                runTime = int.Parse(row[1]);
            }
        }

        for(int i = (aircraftStart+3); i < (missionStart-2); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            {
                for(int j = 0; j < runNumber; j++)
                {
                    aircraftTypes[j, (i - (aircraftStart + 3))] = row[2 * j];
                    aircraftNumbers[j, (i - (aircraftStart + 3))] = float.Parse(row[(2 * j) + 1]);
                    aircraftNumbers[j, aircraftSize] += float.Parse(row[(2 * j) + 1]);
                }
            }
        }

        for(int i = (resourceStart+3); i < (resourceFailureStart-1);i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j<runNumber; j++)
            {
                resources[j, (i - (resourceStart + 3))] = row[j * 6];
                resourceInit[j, (i - (resourceStart + 3))] = int.Parse(row[(j * 6) + 1]);
                resourceDownTime[j, (i - (resourceStart + 3))] += float.Parse(row[(j * 6) + 2]);
            }
        }

        for(int i = (resourceFailureStart+3); i < (resourceWaitStart-1); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j < runNumber; j++)
            {
                for(int k = 0; k <resourceSize;k++)
                {
                    if(resources[j,k] == row[j*3])
                    {
                        resourceDownTime[j, k] += float.Parse(row[(j * 3) + 2]);
                    }
                }
            }
        }

        for(int i = (repairJobStart+3);i<(reworkStart-1);i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j < runNumber; j++)
            {
                repairJobSet[j, 0, (i - (repairJobStart + 3))] = row[j * 3];
                repairJobSet[j, 1, (i - (repairJobStart + 3))] = row[(j * 3) + 1];
                repairJobTimes[j, (i - (repairJobStart + 3))] = float.Parse(row[(j * 3) + 2]);
            }
        }

        //Minor Math
        for (int i = (repairJobStart + 3); i < (reworkStart - 1); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for (int j = 0; j < runNumber; j++)
            {
                for (int k = 0; k < aircraftSize; k++)
                {
                    if (aircraftTypes[j, k] == row[j * 3])
                    {
                        aircraftRepairAverage[j, k] += (float.Parse(row[(j * 3) + 2]) / aircraftNumbers[j, k]);
                    }
                }
            }
        }

        for(int j = 0; j < runNumber; j++)
        {
            for(int i = 0; i < resourceSize; i++)
            {
                resourceUptime[j, i] = (runTime * resourceInit[j, i]) - resourceDownTime[j, i];
            }
        }

        for(int i = 0; i < aircraftSize; i++)
        {
            for(int j = 0; j < runNumber; j++)
            {
                aircraftAverageTime[i] += (aircraftRepairAverage[j, i]/runNumber);
            }
        }

        for(int j = 0; j < runNumber; j++)
        {
            aircraftTotal += aircraftNumbers[j,aircraftSize];
        }

        aircraftAverage = aircraftTotal / runNumber;

        //Output Data

        //Quadrant1
        field1.text = "Maintenance:\n" +
                      "  Planned: " + plannedMaintenance + "\n" +
                      "  Unplanned: " + unplannedMaintenance + "\n" +
                      "  Reworks: "+ reworkCount+"\n"+
                      "  Total Aircraft: " + aircraftTotal + "\n" +
                      "  Average Aircraft: " + aircraftAverage;

        //Quadrant2
        field2.text = "Aircraft:\n";
        
        for(int j = 0; j < runNumber; j++)
        {
            field2.text += ("Run " + (j + 1) + "\tType\tRepair\tTime\n");
            for(int i = 0; i < repairJobSize; i++)
            {
                field2.text += (repairJobSet[j, 0, i] + "\t" + repairJobSet[j, 1, i] + "\t" + repairJobTimes[j, i]+"\n");
            }
            field2.text += "\n";
        }

        //Quadrant3
        field3.text = "Resources:\n";

        for(int j = 0; j < runNumber; j++)
        {
            field3.text += ("Run " + (j + 1) + "\tResource\tUptime\tDownTime\n");
            for(int i = 0; i < resourceSize; i++)
            {
                field3.text += (resources[j, i] + "\t" + resourceUptime[j, i] + "\t" + resourceDownTime[j, i]+"\n");
            }
            field3.text += "\n";
        }

        //Quadrant4
        field4.text = "Averages:\n" +
                      "Aircraft\tTime\n";
        for(int i = 0; i < aircraftSize; i++)
        {
            field4.text += (aircraftTypes[0, i] + "\t" + aircraftAverageTime[i] + "\n");
        }
    }

    //Set of Statistics for "Parts"
    public void Parts()
    {
        //Parts Metrics
        float restockAverage = 0;
        float restockTimeAverage = 0;

        int partsSize = ((restockStart-1) - (partsStart + 3));
        int restockSize = ((fileLines.Length-1) - (restockStart + 3));

        //Parts Arrays
        var partsSet = new string[runNumber, partsSize];
        var partsUsage = new float[runNumber+1, 2 , partsSize];
        var restockSet = new string[runNumber, restockSize];
        var restockTimes = new float[runNumber, restockSize+1];
        var restockCounts = new int[runNumber];


        titleText.text = "  Parts";

        //Read in Data

        //Populate "Parts" arrays
        for(int i = (partsStart+3); i < (restockStart-1);i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j < runNumber; j++)
            {
                if(row[j*4] != "")
                {
                    //Read in part name
                    partsSet[j, (i - (partsStart + 3))] = row[j * 4];
                    //Read in number of this part used
                    partsUsage[j, 0, (i - (partsStart + 3))] = float.Parse(row[(j * 4) + 1]);
                    //tally average over runs
                    partsUsage[runNumber, 0, (i - (partsStart + 3))] += float.Parse(row[(j * 4) + 1])/runNumber;
                    //Read in number of unsuccessful requests for the part
                    partsUsage[j, 1, (i - (partsStart + 3))] = float.Parse(row[(j * 4) + 3]);
                    //Tally average over runs
                    partsUsage[runNumber, 1, (i - (partsStart + 3))] += float.Parse(row[(j * 4) + 1])/runNumber;
                }
            }
        }

        //Populate "Restock" arrays
        for(int i = (restockStart+3);i < (fileLines.Length-1); i++)
        {
            string[] row = fileLines[i].Split(new char[] { ',' });
            for(int j = 0; j < runNumber; j++)
            {
                //Check for data
                if(row[j*2] != "")
                {
                    //Read in Restocked part
                    restockSet[j, (i - (restockStart + 3))] = row[j * 2];
                    //Increment restock count
                    restockCounts[j]++;
                    //Read in Time for restock event
                    restockTimes[j, (i - (restockStart + 3))] = float.Parse(row[(j * 2) + 1]);
                    //Tally total time
                    restockTimes[j, restockSize] += float.Parse(row[(j * 2) + 1]);
                }
            }
        }

        //Minor Math
        for(int j = 0; j < runNumber; j++)
        {
            restockAverage += (restockCounts[j] / runNumber);
            restockTimes[j, restockSize] = (restockTimes[j, restockSize] / restockCounts[j]);
            restockTimeAverage += (restockTimes[j, restockSize] / runNumber);
        }


        //Output Data

        //Quadrant 1
        field1.text = "Parts:\n";
        for(int j = 0; j < runNumber; j++)
        {
            field1.text += ("Run " + (j + 1) + "\n");
            field1.text += ("Part\tNumber Used\n");
            for(int i = 0; i < partsSize; i++)
            {
                if(partsSet[j,i] != "")
                {
                    field1.text += (partsSet[j, i] + "\t" + partsUsage[j, 0, i] + "\n");
                }
            }
            field1.text += "\n";
        }

        field1.text += "\n" +
            "Averages:\n";
        for(int i = 0; i < partsSize; i++)
        {
            field1.text += (partsSet[0, i] + "\t" + partsUsage[runNumber, 0, i] + "\n");
        }

        //Quadrant 2
        field2.text = "Part Shortages:\n";
        for(int j = 0; j < runNumber; j++)
        {
            field2.text += ("Run " + (j + 1) + "\n");
            field2.text += ("Part\tUnsuccessful Requests\n");
            for (int i = 0; i < partsSize; i++)
            {
                if (partsSet[j, i] != "")
                {
                    field2.text += (partsSet[j, i] + "\t" + partsUsage[j, 1, i] + "\n");
                }
            }
            field2.text += "\n";
        }

        field2.text += "\n" +
            "Averages:\n";
        for (int i = 0; i < partsSize; i++)
        {
            field2.text += (partsSet[0, i] + "\t" + partsUsage[runNumber, 1, i] + "\n");
        }

        //Quadrant 3
        field3.text = "Restocking:\n";
        field3.text = "Run\tRestocks\tAverage Time\n";
        for(int j = 0; j < runNumber; j++)
        {
            field3.text += ("Run " + (j + 1) + "\t" + restockCounts[j] + "\t" + restockTimes[j, restockSize]+ "\n" );
        }

        //Quadrant 4
        field4.text = "Averages:\n" +
                      "  Number of Restocks: " + restockAverage + "\n" +
                      "  Restock Time: " + restockTimeAverage;
    }

    //Used to set results file from main simulation
    public void SetInputFile(string fileName)
    {
        inputFile = Resources.Load<TextAsset>(fileName); 
    }

    public void Erase()
    {
        titleText.text = "";
        field1.text = "";
        field2.text = "";
        field3.text = "";
        field4.text = "";
    }
}
