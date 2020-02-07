using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RayCast : MonoBehaviour
{
    public Camera sceneCamera;

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            RaycastHit hit;
            //Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            // do we hit our scene plane?
            if (Physics.Raycast(ray, out hit))
            {
                Debug.Log(hit.collider.gameObject);

                var localPoint = hit.textureCoord;
                // convert the hit texture coordinates into camera coordinates
                //Ray portalRay = sceneCamera.ScreenPointToRay(new Vector2(localPoint.x * sceneCamera.pixelWidth, localPoint.y * sceneCamera.pixelHeight));
                Ray sceneRay = sceneCamera.ViewportPointToRay(hit.textureCoord);
                RaycastHit sceneHit;
                // test these camera coordinates in another raycast test
                if (Physics.Raycast(sceneRay, out sceneHit))
                {
                    Debug.Log(sceneHit.collider.gameObject);
                }
            }
        }
    }
}
