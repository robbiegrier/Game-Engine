#include "EditorInput.h"
#include "EditorInputObserver.h"
#include "DLinkIterator.h"
#include "ListNode.h"
#include "imgui.h"
#include "Engine.h"

namespace Azul
{
	EditorInput::EditorInput()
	{
	}

	EditorInput::~EditorInput()
	{
	}

	void EditorInput::Update()
	{
		EditorInput& self = GetInstance();

		if (!Engine::IsWindowFocused())
		{
			return;
		}

		ImVec2 mousePos = ImGui::GetMousePos();
		MousePosition currMousePos((int)mousePos.x, (int)mousePos.y);

		self.shiftDown = (GetKeyState(VK_SHIFT) & 0x8000);
		const bool controlDown = (GetKeyState(VK_CONTROL) & 0x8000);
		const bool altDown = (GetKeyState(18) & 0x8000);

		const bool deleteDown = (GetKeyState(VK_DELETE) & 0x8000);
		const bool deleteDownThisFrame = deleteDown && !self.wasDeleteDown;

		const bool zDown = (GetKeyState('Z') & 0x8000);
		const bool ctrlZDown = zDown && controlDown;
		const bool ctrlZDownThisFrame = ctrlZDown && !self.wasCtrlZDown;

		const bool yDown = (GetKeyState('Y') & 0x8000);
		const bool ctrlYDown = yDown && controlDown;
		const bool ctrlYDownThisFrame = ctrlYDown && !self.wasCtrlYDown;

		const bool xDown = (GetKeyState('X') & 0x8000);
		const bool ctrlXDown = xDown && controlDown;
		const bool ctrlXDownThisFrame = ctrlXDown && !self.wasCtrlXDown;

		const bool cDown = (GetKeyState('C') & 0x8000);
		const bool ctrlCDown = cDown && controlDown;
		const bool ctrlCDownThisFrame = ctrlCDown && !self.wasCtrlCDown;

		const bool vDown = (GetKeyState('V') & 0x8000);
		const bool ctrlVDown = vDown && controlDown;
		const bool ctrlVDownThisFrame = ctrlVDown && !self.wasCtrlVDown;

		const bool sDown = (GetKeyState('S') & 0x8000);
		const bool ctrlSDown = sDown && controlDown;
		const bool ctrlSDownThisFrame = ctrlSDown && !self.wasCtrlSDown;

		const bool translateDown = (GetKeyState('W') & 0x8000) && controlDown;
		const bool translateDownThisFrame = translateDown && !self.wasTranslateModeDown;

		const bool scaleDown = (GetKeyState('R') & 0x8000) && controlDown;
		const bool scaleDownThisFrame = scaleDown && !self.wasScaleModeDown;

		const bool rotateDown = (GetKeyState('E') & 0x8000) && controlDown;
		const bool rotateDownThisFrame = rotateDown && !self.wasRotateModeDown;

		self.leftClickDown = (GetKeyState(VK_LBUTTON) & 0x8000);
		self.leftClickPressedThisFrame = self.leftClickDown && !self.wasLeftClickDown;
		self.leftClickReleasedThisFrame = !self.leftClickDown && self.wasLeftClickDown;

		self.rightClickDown = (GetKeyState(VK_RBUTTON) & 0x8000);
		self.rightClickPressedThisFrame = self.rightClickDown && !self.wasRightClickDown;
		self.rightClickReleasedThisFrame = !self.rightClickDown && self.wasRightClickDown;

		const bool leftMouseDragging = self.leftClickDown && ((currMousePos != self.prevMousePos) || self.wasLeftDragging);
		const bool leftMouseStartDraggingThisFrame = leftMouseDragging && !self.wasLeftDragging;
		const bool leftMouseStopDraggingThisFrame = !leftMouseDragging && self.wasLeftDragging;

		self.wasLeftClickDown = self.leftClickDown;
		self.wasRightClickDown = self.rightClickDown;
		self.prevMousePos = currMousePos;
		self.wasLeftDragging = leftMouseDragging;
		self.wasTranslateModeDown = translateDown;
		self.wasScaleModeDown = scaleDown;
		self.wasRotateModeDown = rotateDown;
		self.wasCtrlZDown = ctrlZDown;
		self.wasCtrlYDown = ctrlYDown;
		self.wasCtrlXDown = ctrlXDown;
		self.wasCtrlCDown = ctrlCDown;
		self.wasCtrlVDown = ctrlVDown;
		self.wasDeleteDown = deleteDown;
		self.wasCtrlSDown = ctrlSDown;

		self.mod = InputModifier::None;
		if (self.shiftDown) self.mod = InputModifier::Shift;
		if (controlDown) self.mod = InputModifier::Control;
		if (altDown) self.mod = InputModifier::Alt;

		ImGui::Begin("Command Bar");

		if (ctrlSDownThisFrame || ImGui::Button("Save", { 50, 50 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnSavePressed();
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Load", { 50, 50 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnLoadPressed();
			}
		}

		if (deleteDownThisFrame)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnDeletePressed();
			}
		}

		if (ctrlXDownThisFrame)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnCutPressed();
			}
		}

		ImGui::SameLine();
		if (ctrlCDownThisFrame || ImGui::Button("Copy", { 50, 50 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnCopyPressed();
			}
		}

		ImGui::SameLine();
		if (ctrlVDownThisFrame || ImGui::Button("Paste", { 50, 50 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnPastePressed();
			}
		}

		ImGui::SameLine();
		if (ctrlZDownThisFrame || ImGui::Button("Undo", { 50, 50 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnUndoPressed();
			}
		}

		ImGui::SameLine();
		if (ctrlYDownThisFrame || ImGui::Button("Redo", { 50, 50 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnRedoPressed();
			}
		}

		ImGui::End();

		ImGui::Begin("Transform");

		if (translateDownThisFrame || ImGui::Button("Translate (ctrl + W)", { 150, 25 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnTranslateModePressed();
			}
		}

		ImGui::SameLine();
		if (rotateDownThisFrame || ImGui::Button("Rotate (ctrl + E)", { 150, 25 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnRotateModePressed();
			}
		}

		ImGui::SameLine();
		if (scaleDownThisFrame || ImGui::Button("Scale (ctrl + R)", { 150, 25 }))
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnScaleModePressed();
			}
		}

		ImGui::End();

		if (self.leftClickPressedThisFrame)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnLeftClickPressed();
			}
		}

		if (self.leftClickReleasedThisFrame)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnLeftClickReleased();
			}
		}

		if (self.rightClickPressedThisFrame)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnRightClickPressed();
			}
		}

		if (self.rightClickReleasedThisFrame)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnRightClickReleased();
			}
		}

		if (leftMouseStartDraggingThisFrame)
		{
			self.leftDragStart = currMousePos;

			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnLeftClickDragStart(self.leftDragStart);
			}
		}

		if (leftMouseDragging)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnLeftClickDragInProgress(self.leftDragStart, currMousePos);
			}
		}

		if (leftMouseStopDraggingThisFrame)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnLeftClickDragEnd(self.leftDragStart, currMousePos);
			}
		}

		if (self.mouseWheelDelta != 0)
		{
			for (Iterator& it = *self.observers.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorInputObserver* pObserver = (EditorInputObserver*)((ListNode*)it.Curr())->Get();
				pObserver->OnScroll(self.mouseWheelDelta);
			}
		}

		self.mouseWheelDelta = 0;
	}

	void EditorInput::AddObserver(EditorInputObserver* pInObserver)
	{
		EditorInput& self = GetInstance();
		self.observers.Add(new ListNode(pInObserver));
	}

	void EditorInput::OnMouseWheelEvent(int delta)
	{
		EditorInput& self = GetInstance();
		self.mouseWheelDelta = delta;
	}

	InputModifier EditorInput::GetModState()
	{
		return GetInstance().mod;
	}

	EditorInput& EditorInput::GetInstance()
	{
		static EditorInput instance;
		return instance;
	}
}