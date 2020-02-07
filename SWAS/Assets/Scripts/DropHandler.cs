using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class NewBehaviourScript : MonoBehaviour, IDropHandler
{
    public GameObject scenePanel;

    public void OnDrop(PointerEventData eventData)
    {
        //RectTransform toolboxPanel = transform as RectTransform;
        RectTransform sceneCull = scenePanel.GetComponent<RectTransform>();

        if(RectTransformUtility.RectangleContainsScreenPoint(sceneCull, Input.mousePosition))
        {
            Debug.Log("Dropped");
        }
    }
}
