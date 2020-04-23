using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class NextButtonManager : MonoBehaviour
{
    public Button nextButton;

    private bool widthSet;
    private bool lengthSet;
    private bool selection;
    private SelectionControl selectionControl;

    void Awake()
    {
        widthSet = false;
        lengthSet = false;
        selection = false;

        selectionControl = GameObject.FindObjectOfType<SelectionControl>();
    }

    private void Update()
    {
        if(selectionControl.GetSelectionList().Count > 0)
        {
            selection = true;
        }

        if(widthSet && lengthSet && selection)
        {
            nextButton.interactable = true;
        }
    }

    public void WidthSet()
    {
        widthSet = true;
    }

    public void LengthSet()
    {
        lengthSet = true;
    }

}
