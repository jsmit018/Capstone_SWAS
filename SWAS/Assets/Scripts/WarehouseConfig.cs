using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WarehouseConfig : MonoBehaviour
{
    private WarehouseManager warehouseManager;

    void Awake()
    {
        warehouseManager = GameObject.FindObjectOfType<WarehouseManager>();
    }

    private void OnLevelWasLoaded(int level)
    {
        if (level == 1)
        {
            gameObject.transform.localScale = new Vector3(warehouseManager.getWarehouseX(), 1, warehouseManager.getWarehouseY());
        }
    }
}
