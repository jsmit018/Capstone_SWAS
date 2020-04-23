using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CandidateButton : MonoBehaviour
{

    [SerializeField]
    private Text mytext;

    [SerializeField]
    private CandidateControl candidateController;

    public void SetText(string typeName)
    {
        mytext.text = typeName;
    }

    public void OnClick()
    {
        candidateController.SetSelection(this);
    }
}
