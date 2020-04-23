using System.Collections;
using System.Collections.Generic;
using UnityEngine.SceneManagement;
using UnityEngine;
using UnityEngine.UI;

public class NextCommand : MonoBehaviour
{
    [SerializeField]
    private InputField lengthInput;
    [SerializeField]
    private InputField widthInput;
    [SerializeField]
    private Toggle unitSetter;

    private static float warehouseLength;
    private static float warehouseWidth;
    private static string unitMeasure;
    private bool metric;

    // Start is called before the first frame update
    void Start()
    {
        metric = true;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void OnUnitClick()
    {
        metric = !metric;
    }

    //Switch to Drag-And-Drop Scene
    public void Transition()
    {
        warehouseLength = float.Parse(lengthInput.text);
        warehouseWidth = float.Parse(widthInput.text);
        if (metric == true)
        {
            unitMeasure = "Meters";
        }
        else if (metric == false)
        {
            unitMeasure = "Feet";
        }
        SceneManager.LoadScene(1);
    }
}
