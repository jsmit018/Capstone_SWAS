using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraBehavior : MonoBehaviour
{
    public GameObject cameraTarget;
    private float targetX;
    private float targetY;

    // Initialize Camera Size Based on Warehouse Dimensions
    void Start()
    {
        targetX = cameraTarget.transform.localScale.x;
        targetY = cameraTarget.transform.localScale.y;

        Camera camera = GetComponent<Camera>();

        if (targetX > targetY)
        {
            camera.orthographicSize = targetX * 2.7f;
        }
        else
        {
            camera.orthographicSize = targetY * 2.7f;
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
