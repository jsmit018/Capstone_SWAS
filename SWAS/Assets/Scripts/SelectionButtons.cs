using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SelectionButtons : MonoBehaviour
{

    [SerializeField]
    private Text mytext;

    [SerializeField]
    private SelectionControl selectionController;

    private bool subButton;
    
    //Set Text Within a Button
    public void SetText(string typeName)
    {
        mytext.text = typeName;
    }

    //Set Bool Value to Indicate Sub-Button status 
    public void SetState(bool val)
    {
        subButton = val;
    }

    //Determines Deletion Logic
    public bool GetState()
    {
        return subButton;
    }

   //Funtion to Select a Clicked Button
    public void OnClick()
    {
        selectionController.SelectButton(this);
    }
}
