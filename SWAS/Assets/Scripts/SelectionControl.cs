using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SelectionControl : MonoBehaviour
{

    [SerializeField]
    private GameObject buttonTemplate;
    [SerializeField]
    private GameObject subButtonTemplate;
    [SerializeField]
    private CandidateControl CandidateControler;
    [SerializeField]
    private SelectionControl SelectionControler;
    [SerializeField]
    private Dropdown typeSelector;
    

    private SelectionButtons selectedButton;
    private LinkedList<String> selectedList = new LinkedList<String>();
    static LinkedList<GameObject> setList = new LinkedList<GameObject>();

    private TextAsset datasource;
    private string[] data;


    void Start()
    {
        //Read In Data File
        datasource = Resources.Load<TextAsset>("SWASInputData");
        selectedButton = null;
        data = datasource.text.Split(new char[] { '\n' });
    }

    //Add Selected Entry to Selection List
    public void Add()
    {
        bool exists = false;
       
        //Check that a button is selected
        if(CandidateControler.GetButton() != null)
        {
            //Check that button is not already in list
            foreach (string str in selectedList)
            {
                if (str == CandidateControler.GetButton())
                {
                    exists = true;
                }
            }

            if(exists == false)
            {
                //Add Button To List
                selectedList.AddLast(CandidateControler.GetButton());

                //Create Button UI
                GameObject button = Instantiate(buttonTemplate) as GameObject;
                button.SetActive(true);

                button.GetComponent<SelectionButtons>().SetText(CandidateControler.GetButton());

                button.transform.SetParent(buttonTemplate.transform.parent, false);
                button.GetComponent<SelectionButtons>().SetState(false);

                setList.AddLast(button);

                //Search Input
                for (int i = 5; i < data.Length - 1; i++)
                {
                    //Divide lines into entries based on commas
                    string[] row = data[i].Split(new char[] { ',' });

                    //Search document for start of "Missions"
                    if(row[0] == "Missions")
                    {
                        //Skip Headings
                        i += 2;
                        row = data[i].Split(new char[] { ',' });
                        //Run Through Data Until a Blank Line is Encountered
                        do
                        {
                            //Search for Matching Aircraft
                            if(typeSelector.value == 0)
                            {
                                //Set up New Sub-Button
                                if (row[1] == CandidateControler.GetButton())
                                {
                                    GameObject subButton = Instantiate(subButtonTemplate) as GameObject;
                                    subButton.SetActive(true);
                                    subButton.GetComponent<SelectionButtons>().SetText(row[0]);
                                    subButton.transform.SetParent(button.transform, false);
                                    subButton.GetComponent<SelectionButtons>().SetState(true);
                                    setList.AddLast(subButton);
                                }
                            }

                            //Search for Matching Mission
                            else if(typeSelector.value == 1)
                            {
                                //Set up New Sub-Button
                                if (row[0] == CandidateControler.GetButton())
                                {
                                    GameObject subButton = Instantiate(subButtonTemplate) as GameObject;
                                    subButton.SetActive(true);
                                    subButton.GetComponent<SelectionButtons>().SetText(row[1]);
                                    subButton.transform.SetParent(button.transform, false);
                                    subButton.GetComponent<SelectionButtons>().SetState(true);
                                    setList.AddLast(subButton);
                                }
                            }

                            i++;
                            row = data[i].Split(new char[] { ',' });
                        } while (row[0] != "");
                    }
                }

            }
            
        }
        
    }

    //Remove Selected Entry from Selection List
    public void Remove()
    {
        if(selectedButton.GetState())
        {
            setList.Remove(selectedButton.gameObject);
            Destroy(selectedButton.gameObject);
            selectedButton = null;
        }
        else
        {
            setList.Remove(selectedButton.gameObject);
            selectedList.Remove(selectedButton.GetComponentInChildren<Text>().text);
            Destroy(selectedButton.gameObject);
            selectedButton = null;
        }
        
    }

    //Update which button is selected from list
    public void SelectButton(SelectionButtons selection)
    {
        selectedButton = selection;
    }

    //Delete Current Selection and List
    public void Delete()
    {
        GameObject button;
        while (selectedList.Count > 0)
        {
            button = setList.Last.Value;
            setList.RemoveLast();
            selectedList.RemoveLast();
            Destroy(button);
        }
    }

    public LinkedList<String> GetSelectionList()
    {
        return selectedList;
    }
}
