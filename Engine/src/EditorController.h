#ifndef _EditorController
#define _EditorController

#include "MathEngine.h"

namespace Azul
{
	// Controls the camera and perspective when in editor mode
	class EditorController
	{
	public:
		// Big four
		EditorController() = default;
		EditorController(const EditorController&) = delete;
		EditorController& operator=(const EditorController&) = delete;
		~EditorController() = default;

		// Update when in editor mode
		void Update();

	private:
		void CheckSwitchCameraInput();
		void CheckMovement(float deltaTime);
		void CheckLookAt(float deltaTime);
		void ToggleLookAtMode();
		void ToggleFramerateMode();
		void ProcessGravity(float deltaTime);

		bool useMouseLook = false;
		POINT prevPoint;
		bool jumping = false;
		bool sprinting = false;
		float jumpImpulse = 0.f;
		bool walkMode = false;
	};
}

#endif