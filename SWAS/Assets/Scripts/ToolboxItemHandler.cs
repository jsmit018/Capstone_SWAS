using RuntimeInspectorNamespace;
using System.Collections;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class ToolboxItemHandler : MonoBehaviour
{
    public GameObject toolBoxObject;
    public Texture2D toolBoxCursorImage;

    [SerializeField]
    private RectTransform toolBox;

    [SerializeField]
    private PointerEventListener sceneViewPointerListener;
    private PointerEventData sceneViewPointer;

    [SerializeField]
    private RectTransform sceneView;

    [SerializeField]
    private RuntimeHierarchy hierarchy;
    private Unityception unityception;
    
    // Array of buttons within toolbox
    private Button[] toolBoxButtons;
    
    // Flags for toolbox selections
    private bool toolboxSelected;
    private bool sceneSelected;
    private bool itemDrag;

    private Transform dragObj;
    private Vector3 dragOffset;
    private Vector3 createPosition;

    void Awake()
    {
        unityception = GameObject.FindObjectOfType<Unityception>();

        // Set initial flag
        toolboxSelected = false;

        sceneViewPointerListener.PointerDown += (eventData) =>
        {
            if (eventData.button == PointerEventData.InputButton.Left)
            {
                Vector2 position;
                RectTransformUtility.ScreenPointToLocalPointInRectangle(sceneView, eventData.position, null, out position);

                Vector2 viewport = new Vector2(position.x / unityception.GetSceneViewPrevSize().x + 0.5f, position.y / unityception.GetSceneViewPrevSize().y + 0.5f);
                if (Physics.Raycast(unityception.GetMainCam().ViewportPointToRay(viewport), out RaycastHit hit))
                {
                    if (hit.transform.CompareTag("ToolboxItem"))
                    {
                        itemDrag = true;
                        dragObj = hit.transform;
                        dragOffset = new Vector3(dragObj.position.x - hit.point.x, dragObj.position.y - hit.point.y, 0f);
                    }
                }
            }
            else
            {
                sceneViewPointer = eventData;
            }
        };

        sceneViewPointerListener.PointerUp += (eventData) =>
        {
            if (eventData.button == PointerEventData.InputButton.Left)
            {
                if (itemDrag)
                    itemDrag = false;
            }
            else
            {
                sceneViewPointer = null;
            }
        };

        sceneViewPointerListener.PointerClick += (eventData) =>
        {
            if (eventData.button == PointerEventData.InputButton.Left)
            {
                Vector2 position;
                RectTransformUtility.ScreenPointToLocalPointInRectangle(sceneView, eventData.position, null, out position);

                Vector2 viewport = new Vector2(position.x / unityception.GetSceneViewPrevSize().x + 0.5f, position.y / unityception.GetSceneViewPrevSize().y + 0.5f);
                if (Physics.Raycast(unityception.GetMainCam().ViewportPointToRay(viewport), out RaycastHit hit))
                {
                    hierarchy.Select(hit.collider.transform);
                    if (toolboxSelected)
                    {
                        Debug.Log("I clicked at... " + hit.point);
                        createPosition = hit.point;
                        toolboxSelected = false;
                        sceneSelected = true;
                    }
                }
            }
        };
    }

    public void InitializeToolboxButtons()
    { 
        // Size ToolBoxButtons to # in toolBox
        toolBoxButtons = toolBox.GetComponentsInChildren<Button>();

        for (int i = 0; i < toolBoxButtons.Length; i++)
        {
            Button button = toolBoxButtons[i];
            button.onClick.AddListener(() => CreateItem(button));
        }
    }

    void Update()
    {
        if (itemDrag)
        {
            Vector2 position;
            RectTransformUtility.ScreenPointToLocalPointInRectangle(sceneView, Input.mousePosition, null, out position);

            Vector2 viewport = new Vector2(position.x / unityception.GetSceneViewPrevSize().x + 0.5f, position.y / unityception.GetSceneViewPrevSize().y + 0.5f);

            if (Physics.Raycast(unityception.GetMainCam().ViewportPointToRay(viewport), out RaycastHit hit))
            {
                dragObj.position = new Vector3(hit.point.x + dragOffset.x, hit.point.y + dragOffset.y, 0f);
            }
        }
    }

    // Create item given button press
    private void CreateItem(Button button)
    {
        Debug.Log("I clicked on " + button.transform.name);
        //Hold until click in scene
        toolboxSelected = true;
        sceneSelected = false;

        StartCoroutine(PlaceInScene(button.transform.name));
    }

    IEnumerator PlaceInScene(string name)
    {
        // Wait for scene selection
        while (!sceneSelected)
        {
            // Had to put this here for now b/c the panel resize will change cursor back to default...
            Cursor.SetCursor(toolBoxCursorImage, Vector2.zero, CursorMode.Auto);
            yield return null;
        }

        GameObject newToolBoxObject = Instantiate(toolBoxObject, createPosition, Quaternion.identity) as GameObject;
        newToolBoxObject.transform.name = name;

        Cursor.SetCursor(null, Vector2.zero, CursorMode.Auto);
    }
}
