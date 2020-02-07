using UnityEngine;

namespace RuntimeSceneGizmo
{
	public class CameraMovement : MonoBehaviour
	{
		public Camera sceneCamera;
		private float sensitivity;
		private float x;
		private float y;

		private void Update()
		{
			if( Input.GetMouseButtonDown( 0 ) )
				sensitivity = 0.5f;
				x = Input.GetAxis("Mouse X");
				y = Input.GetAxis("Mouse Y");
				sceneCamera.transform.Translate(new Vector3(-x, -y, 0) * sensitivity);
		}
	}
}