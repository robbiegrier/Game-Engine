#ifndef _EditorInput
#define _EditorInput

#include "DLinkedList.h"
#include "EditorInputObserver.h"

namespace Azul
{
	// Centralizes input detection for important editor inputs. Observable in observer pattern.
	class EditorInput
	{
	public:
		// Update when editor active
		static void Update();

		// Add observer which will recieve all input events
		static void AddObserver(EditorInputObserver* pInObserver);

		// Mouse wheel cannot be detected directly, so it is called externally by a Windows event
		static void OnMouseWheelEvent(int delta);

		// Get the key modifier state this frame (shft, ctrl, alt, etc)
		static InputModifier GetModState();

	private:
		// Big four
		EditorInput();
		EditorInput(EditorInput&) = delete;
		EditorInput& operator=(EditorInput&) = delete;
		~EditorInput();

		// Meyers singleton
		static EditorInput& GetInstance();

		DLinkedList observers;
		MousePosition prevMousePos;

		bool wasLeftClickDown = false;
		bool wasRightClickDown = false;
		bool wasTranslateModeDown = false;
		bool wasScaleModeDown = false;
		bool wasRotateModeDown = false;
		bool wasLeftDragging = false;
		bool wasCtrlXDown = false;
		bool wasCtrlZDown = false;
		bool wasCtrlYDown = false;
		bool wasCtrlCDown = false;
		bool wasCtrlVDown = false;
		bool wasDeleteDown = false;
		bool wasCtrlSDown = false;

		int mouseWheelDelta = 0;

		MousePosition leftDragStart;

		InputModifier mod = InputModifier::None;
	};
}

#endif