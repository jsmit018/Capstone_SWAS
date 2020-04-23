using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;

public class SelectionManager : MonoBehaviour
{
    private static LinkedList<String> selectionList;
    private static LinkedList<String> aircraftList;
    private SelectionControl selectionControl;

    //[DllImport("TestDLL")]
    [DllImport("SWAS_DES")]
    //private static extern int Add(int a, int b);
    //private static extern int AddSelectedAircraft(string aircraft);
    private static extern int TestFunc(int a, int b);
    //[DllImport("SWAS_DES")]
  //  private static extern string SendString(string name);
    //[DllImport("SWAS_DES")]
    //private static extern void DoNothing(string name);

    void Awake()
    {
        selectionControl = GameObject.FindObjectOfType<SelectionControl>();
    }
    public void SetSelection()
    {
        //Debug.Log("Naive Library that adds 2 + 2: Answer: " + Add(2, 2));
        Debug.Log("Naive Function in SWAS_DES that adds 2 + 2: Answer: " + TestFunc(2, 2));
       // Debug.Log(SendString("This message was sent through the SWAS_DES!"));

        selectionList = selectionControl.GetSelectionList();
        aircraftList = selectionControl.GetSelectionList();
        while (aircraftList.Count > 0)
        {
            //AddSelectedAircraft(aircraftList.First.Value);
            aircraftList.RemoveFirst();
        }
    }

    public LinkedList<String> GetSelection()
    {
        return selectionList;
    }
}
