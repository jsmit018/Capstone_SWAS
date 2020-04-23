using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ToolboxManager : MonoBehaviour
{
    public GameObject aircraftButton;
    public GameObject canvas;

    private SelectionManager selectionManager;
    private ToolboxItemHandler toolboxItemHandler;
    private LinkedList<String> selectionList; 

    void Awake()
    {
        selectionManager = GameObject.FindObjectOfType<SelectionManager>();
        toolboxItemHandler = GameObject.FindObjectOfType<ToolboxItemHandler>();
    }

    private void OnLevelWasLoaded(int level)
    {
        selectionList = selectionManager.GetSelection();

        if (level == 1)
        {
            while (selectionList.Count > 0)
            {
                GameObject newButton = Instantiate(aircraftButton) as GameObject;
                newButton.transform.SetParent(canvas.transform, false);
                newButton.transform.name = selectionList.First.Value;
                newButton.GetComponentInChildren<Text>().text = newButton.transform.name;
                selectionList.RemoveFirst();
            }

            toolboxItemHandler.InitializeToolboxButtons();
        }
    }
}
