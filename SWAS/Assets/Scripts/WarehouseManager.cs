using UnityEngine;
using UnityEngine.UI;

public class WarehouseManager : MonoBehaviour
{
    public InputField widthInput;
    public InputField lengthInput;

    private static float warehouseX;
    private static float warehouseY;
    private NextButtonManager nextButtonManager;

    void Awake()
    {
        nextButtonManager = GameObject.FindObjectOfType<NextButtonManager>();
    }

    void Start()
    {
        var widthEvent = new InputField.SubmitEvent();
        var lengthEvent = new InputField.SubmitEvent();
        widthEvent.AddListener(SubmitWidth);
        lengthEvent.AddListener(SubmitLength);
        widthInput.onEndEdit = widthEvent;
        lengthInput.onEndEdit = lengthEvent;
    }

    private void SubmitWidth(string arg0)
    {
        warehouseX = float.Parse(arg0);
        nextButtonManager.WidthSet();
    }

    private void SubmitLength(string arg0)
    {
        warehouseY = float.Parse(arg0);
        nextButtonManager.LengthSet();
    }

    public float getWarehouseX()
    {
        return warehouseX;
    }

    public float getWarehouseY()
    {
        return warehouseY;
    }
}
