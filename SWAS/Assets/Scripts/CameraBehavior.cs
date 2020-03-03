using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraBehavior : MonoBehaviour
{
    public GameObject cameraTarget;
    private float targetY;

    // Start is called before the first frame update
    void Start()
    {
        targetY = cameraTarget.transform.localScale.z;

        Camera camera = GetComponent<Camera>();

        camera.orthographicSize = targetY * 5.6f;
    }
}
