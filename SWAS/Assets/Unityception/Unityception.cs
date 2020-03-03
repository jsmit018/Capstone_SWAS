using DynamicPanels;
using RuntimeInspectorNamespace;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class Unityception : SkinnedWindow
{
	private const string RUNTIME_INSPECTOR_URL = "https://assetstore.unity.com/packages/tools/gui/runtime-inspector-hierarchy-111349";
	private const string DYNAMIC_PANELS_URL = "https://assetstore.unity.com/packages/tools/gui/dynamic-panels-114126";
	private const string DEBUG_CONSOLE_URL = "https://assetstore.unity.com/packages/tools/gui/in-game-debug-console-68068";
	private const string SCENE_GIZMO_URL = "https://assetstore.unity.com/packages/tools/gui/runtime-scene-gizmo-123040";

	private Camera mainCam;
	private Transform mainCamParent;
	private RenderTexture mainCamRT;

	[SerializeField]
	private RawImage sceneViewDrawer;

	[SerializeField]
	private RectTransform sceneView;
	private PanelTab sceneViewTab;

	[SerializeField]
	private RectTransform creditsView;

	[SerializeField]
	private PointerEventListener sceneViewPointerListener;
	private PointerEventData sceneViewPointer;

	[SerializeField]
	private RuntimeHierarchy hierarchy;

	[SerializeField]
	private DynamicPanelsCanvas panelsCanvas;

	[SerializeField]
	private Toggle playToggle;
	[SerializeField]
	private Toggle pauseToggle;
	[SerializeField]
	private Button stepButton;

	private bool IsPlaying { get { return playToggle.isOn; } }
	private bool IsPaused { get { return pauseToggle.isOn; } }

	//SWAS: flags for toolbox selections
	private bool toolboxSelected;
	private bool sceneSelected;
	private bool itemDrag;

	private Transform dragObj;
	private Vector3 dragOffset;

	//SWAS: array of buttons within toolbox
	private Button[] toolBoxButtons;

	//SWAS: Toolbox Object
	public Texture2D toolBoxCursorImage;
	public Transform toolBoxObjectA;
	public Transform toolBoxObjectB;

	//SWAS: Toolbox Panel Assigment
	[SerializeField]
	private RectTransform toolBox;

	[SerializeField]
	private Button creditsRuntimeInspector;
	[SerializeField]
	private Button creditsDynamicPanels;
	[SerializeField]
	private Button creditsDebugConsole;
	[SerializeField]
	private Button creditsSceneGizmo;

	private SkinnedWindow[] skinnedWindows;

	[SerializeField]
	private Button[] skinnedButtons;

	[SerializeField]
	private LayoutElement[] skinnedLayoutElements;

	[SerializeField]
	private Image[] menubarButtonBackgrounds;

	[SerializeField]
	private Graphic[] menubarButtonIcons;

	[SerializeField]
	private Image menubarBackground;

	[SerializeField]
	private Image creditsBackground;

	[SerializeField]
	private Text creditsTitle;

	[SerializeField]
	private Dropdown layoutDropdown;

	[SerializeField]
	private Image dropdownBackground;

	[SerializeField]
	private Image dropdownArrow;

	[SerializeField]
	private Image dropdownTemplateBackground;

	[SerializeField]
	private Image dropdownTemplateCheckmark;

	[SerializeField]
	private Text dropdownTemplateText;

	[SerializeField]
	private TextAsset[] panelLayouts;

	private Vector2 sceneViewPrevSize;
	private Vector3 createPosition;

	protected override void Awake()
	{
		base.Awake();

		//SWAS: set initial flag
		toolboxSelected = false;
		//itemDrag = false;

		//SWAS: size ToolBoxButtons to # in toolBox
		toolBoxButtons = toolBox.GetComponentsInChildren<Button>();

		//SWAS: Create onClick listeners for each toolBox button
		for (int i = 0; i < toolBoxButtons.Length; i++)
		{
			Button button = toolBoxButtons[i];
			button.onClick.AddListener(() => CreateItem(button));
		}

		mainCam = GameObject.FindWithTag( "MainCamera" ).GetComponent<Camera>();
		mainCamParent = mainCam.transform.parent;
		mainCamRT = new RenderTexture( 2, 2, 0 );
		mainCam.targetTexture = mainCamRT;

		sceneViewDrawer.texture = mainCamRT;
		sceneViewDrawer.color = Color.white;

		skinnedWindows = FindObjectsOfType<SkinnedWindow>();

		sceneViewPointerListener.PointerDown += ( eventData ) =>
		{
			if(eventData.button == PointerEventData.InputButton.Left)
			{
				Vector2 position;
				RectTransformUtility.ScreenPointToLocalPointInRectangle(sceneView, eventData.position, null, out position);

				Vector2 viewport = new Vector2(position.x / sceneViewPrevSize.x + 0.5f, position.y / sceneViewPrevSize.y + 0.5f);
				if (Physics.Raycast(mainCam.ViewportPointToRay(viewport), out RaycastHit hit))
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

		sceneViewPointerListener.PointerUp += ( eventData ) =>
		{
			if( eventData.button == PointerEventData.InputButton.Left )
			{
				if (itemDrag)
					itemDrag = false;
			}
			else
			{
				sceneViewPointer = null;
			}
		};

		sceneViewPointerListener.PointerClick += ( eventData ) =>
		{
			if( eventData.button == PointerEventData.InputButton.Left )
			{
				Vector2 position;
				RectTransformUtility.ScreenPointToLocalPointInRectangle( sceneView, eventData.position, null, out position );

				Vector2 viewport = new Vector2( position.x / sceneViewPrevSize.x + 0.5f, position.y / sceneViewPrevSize.y + 0.5f );
				if( Physics.Raycast( mainCam.ViewportPointToRay( viewport ), out RaycastHit hit ) )
				{
					hierarchy.Select(hit.collider.transform);
					if(toolboxSelected)
					{
						Debug.Log("I clicked at... " + hit.point);
						createPosition = hit.point;
						toolboxSelected = false;
						sceneSelected = true;
					}
				}
			}
		};

		playToggle.onValueChanged.AddListener( ( value ) =>
		{
			if( !value )
			{
				pauseToggle.isOn = false;
				Time.timeScale = 0f;
			}
			else
			{
				if( !IsPaused )
					Time.timeScale = 1f;
			}
		} );

		pauseToggle.onValueChanged.AddListener( ( value ) =>
		{
			if( IsPlaying )
			{
				if( value )
					Time.timeScale = 0f;
				else
					Time.timeScale = 1f;
			}
		} );

		stepButton.onClick.AddListener( () =>
		{
			if( !IsPlaying )
				return;

			if( !IsPaused )
				pauseToggle.isOn = true;

			StartCoroutine( StepCoroutine() );
		} );

		creditsRuntimeInspector.onClick.AddListener( () => OpenLink( RUNTIME_INSPECTOR_URL ) );
		creditsDynamicPanels.onClick.AddListener( () => OpenLink( DYNAMIC_PANELS_URL ) );
		creditsDebugConsole.onClick.AddListener( () => OpenLink( DEBUG_CONSOLE_URL ) );
		creditsSceneGizmo.onClick.AddListener( () => OpenLink( SCENE_GIZMO_URL ) );

		layoutDropdown.options = new List<Dropdown.OptionData>( 5 )
		{
			new Dropdown.OptionData( "Tall" ),
			new Dropdown.OptionData( "Wide" ),
			new Dropdown.OptionData( "Default" ),
			new Dropdown.OptionData( "Save Layout..." ),
			new Dropdown.OptionData( "Load Layout..." ),
			new Dropdown.OptionData( "- Custom Layout -" )
		};

		layoutDropdown.value = 0;
		layoutDropdown.RefreshShownValue();

		layoutDropdown.onValueChanged.AddListener( ( option ) =>
		{
			if( option < 3 )
				PanelSerialization.DeserializeCanvasFromArray( panelsCanvas, panelLayouts[option].bytes );
			else if( option == 3 )
			{
				panelsCanvas.SaveLayout();
				layoutDropdown.value = 5;

				Debug.Log( "Custom layout saved!" );
			}
			else if( option == 4 )
			{
				panelsCanvas.LoadLayout();
				layoutDropdown.value = 5;
			}
		} );
	}

	private void Start()
	{
		sceneViewTab = PanelUtils.GetAssociatedTab( sceneView );
		sceneViewTab.Panel.FloatingSize = new Vector2( 250f, 300f );
		PanelUtils.GetAssociatedTab( creditsView ).Panel.FloatingSize = new Vector2( 250f, 300f );
	}

	private void Update()
	{
		if(itemDrag)
		{
			Vector2 position;
			RectTransformUtility.ScreenPointToLocalPointInRectangle(sceneView, Input.mousePosition, null, out position);

			Vector2 viewport = new Vector2(position.x / sceneViewPrevSize.x + 0.5f, position.y / sceneViewPrevSize.y + 0.5f);
			
			if (Physics.Raycast(mainCam.ViewportPointToRay(viewport), out RaycastHit hit))
			{
					dragObj.position = new Vector3(hit.point.x + dragOffset.x, hit.point.y + dragOffset.y, 0f);
			}
		}
	}

	private void LateUpdate()
	{
		if( sceneViewTab == null )
			return;

		Vector2 sceneViewSize = sceneViewTab.Panel.Size;
		if( sceneViewSize != sceneViewPrevSize )
		{
			mainCamRT.Release();
			mainCamRT.width = (int) ( sceneViewSize.x * 1.5f );
			mainCamRT.height = (int) ( sceneViewSize.y * 1.5f );

			mainCam.aspect = sceneViewSize.x / sceneViewSize.y;

			sceneViewPrevSize = sceneViewSize;
		}

		if( sceneViewPointer != null )
		{
			Vector3 rot = mainCamParent.localEulerAngles;
			while( rot.x > 180f )
				rot.x -= 360f;
			while( rot.x < -180f )
				rot.x += 360f;

			Vector2 delta = sceneViewPointer.delta * 0.5f;
			rot.x = Mathf.Clamp( rot.x - delta.y, -89.8f, 89.8f );
			rot.y += delta.x;
			rot.z = 0f;

			mainCamParent.localEulerAngles = rot;
		}
	}

	protected override void RefreshSkin()
	{
		for( int i = 0; i < skinnedWindows.Length; i++ )
			skinnedWindows[i].Skin = Skin;

		for( int i = 0; i < skinnedButtons.Length; i++ )
			skinnedButtons[i].SetSkinButton( Skin );

		for( int i = 0; i < skinnedLayoutElements.Length; i++ )
			skinnedLayoutElements[i].SetHeight( Skin.LineHeight * 1.5f );

		for( int i = 0; i < menubarButtonBackgrounds.Length; i++ )
			menubarButtonBackgrounds[i].color = Skin.ButtonBackgroundColor;

		for( int i = 0; i < menubarButtonIcons.Length; i++ )
			menubarButtonIcons[i].color = Skin.ButtonTextColor;

		menubarBackground.color = Skin.BackgroundColor.Tint( -0.025f );
		creditsBackground.color = Skin.BackgroundColor;
		creditsTitle.SetSkinText( Skin );

		dropdownBackground.color = Skin.InputFieldNormalBackgroundColor.Tint( 0.1f );
		dropdownArrow.color = Skin.TextColor.Tint( 0.1f );
		layoutDropdown.captionText.SetSkinInputFieldText( Skin );
		dropdownTemplateText.SetSkinInputFieldText( Skin );
		dropdownTemplateBackground.color = Skin.InputFieldNormalBackgroundColor.Tint( 0.075f );
		dropdownTemplateCheckmark.color = Skin.ToggleCheckmarkColor;
	}

	//SWAS: create item given button press
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
		//Wait for scene selection
		while (!sceneSelected)
		{
			//Had to put this here for now b/c the panel resize will change cursor back to default...
			Cursor.SetCursor(toolBoxCursorImage, Vector2.zero, CursorMode.Auto);
			yield return null;
		}
		if (name == "ThingA")
			Instantiate(toolBoxObjectA, createPosition, Quaternion.identity);
		else
			Instantiate(toolBoxObjectB, createPosition, Quaternion.identity);
		Cursor.SetCursor(null, Vector2.zero, CursorMode.Auto);
	}

	private IEnumerator StepCoroutine()
	{
		Time.timeScale = 1f;
		yield return null;
		Time.timeScale = 0f;
	}

	private void OpenLink( string url )
	{
#if !UNITY_EDITOR && UNITY_WEBGL
		Application.ExternalEval( "window.open(\"" + url + "\",\"_blank\")" );
#else
		Application.OpenURL( url );
#endif
	}
}