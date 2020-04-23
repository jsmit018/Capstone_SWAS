using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class CandidateControl : MonoBehaviour
{

    [SerializeField]
    private GameObject buttonTemplate;
    [SerializeField]
    private Dropdown typeSelector;

    private CandidateButton selectedButton;
    private TextAsset datasource;
    private string[] data;
    private int size;

    LinkedList<String> CandidateList = new LinkedList<String>();
    LinkedList<GameObject> buttonList = new LinkedList<GameObject>();


    // Start is called before the first frame update
    void Start()
    {
        //Get Data File
        datasource = Resources.Load<TextAsset>("SWASInputData");

        data = datasource.text.Split(new char[] { '\n' });

        PopulateList();
    }

    public string GetButton()
    {
        //Check That a Button Is Selected
        if(selectedButton == null)
        {
            return null;
        }
        else
        {
            string buttonText = selectedButton.GetComponentInChildren<Text>().text;
            return buttonText;
        }
        
    }

    //Set a Button as Selected
    public void SetSelection(CandidateButton selection)
    {
        selectedButton = selection;
    }

    //Initialize List
    private void PopulateList()
    {
        //Populate List
        for (int i = 5; i < data.Length - 1; i++)
        {
            //Divide rows by commas
            string[] row = data[i].Split(new char[] { ',' });

            //Search for 'Missions'
            if (row[0] == "Mission Table")
            {
                //Skip header line
                i += 2;
                row = data[i].Split(new char[] { ',' });
                //Do Until Empty Line
                do
                {
                    //Add Aircraft Button
                    if (typeSelector.value == 1)
                    {
                        bool exists = false;

                        //Check if Button Already Exists
                        foreach (string str in CandidateList)
                        {
                            if (str == row[0])
                            {
                                exists = true;
                            }
                        }

                        if (exists == false)
                        {
                            CandidateList.AddLast(row[0]);

                            GameObject button = Instantiate(buttonTemplate) as GameObject;
                            button.SetActive(true);

                            button.GetComponent<CandidateButton>().SetText(row[0]);

                            button.transform.SetParent(buttonTemplate.transform.parent, false);

                            buttonList.AddLast(button);
                        }
                    }
                    //Add Mission Button
                    else if (typeSelector.value == 0)
                    {
                        bool exists = false;
                        foreach (string str in CandidateList)
                        {
                            if (str == row[1])
                            {
                                exists = true;
                            }
                        }

                        //Create New Button
                        if (exists == false)
                        {
                            CandidateList.AddLast(row[1]);

                            GameObject button = Instantiate(buttonTemplate) as GameObject;
                            button.SetActive(true);

                            button.GetComponent<CandidateButton>().SetText(row[1]);

                            button.transform.SetParent(buttonTemplate.transform.parent, false);

                            buttonList.AddLast(button);
                        }
                    }
                    i++;
                    row = data[i].Split(new char[] { ',' });
                } while (row[0] != "");
            }


        }
    }

    //Clear List and Repopulate
    public void DestroyList()
    {
        GameObject button;
        while (CandidateList.Count > 0)
        {
            button = buttonList.Last.Value;
            buttonList.RemoveLast();
            CandidateList.RemoveLast();
            Destroy(button);
        }

        PopulateList();
    }
}