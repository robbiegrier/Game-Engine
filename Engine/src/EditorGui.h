#ifndef _EditorGui
#define _EditorGui

#include "MathEngine.h"
#include <d3d11.h>
#include "imgui.h"
#include "EditorInputObserver.h"
#include "Selection.h"
#include "EditorController.h"
#include "Clipboard.h"
#include "ConsoleWindow.h"
#include "HierarchyWindow.h"
#include "SettingsWindow.h"
#include "InspectorWindow.h"
#include "WorldWindow.h"
#include "AssetsWindow.h"

namespace Azul
{
	class Viewport;
	class GameObject;
	class EditorVisual;

	// Central Editor Class
	class EditorGui : public EditorInputObserver
	{
	public:
		static constexpr float WIDGET_TOLERANCE = 0.08f;
		static constexpr float SCROLL_SPEED = 3.f;

		// Realtime events
		static void Initialize();
		static void NewFrame();
		static void Draw();
		static void Cleanup();
		static void OnGameObjectRemoved(GameObject* pObject);
		static void UpdateSelectionOnClick(InputModifier mod, GameObject* pObjectClicked);
		static void Console(const char* msg);

		// Get components of the editor
		static Viewport* GetWorldViewport() { return GetInstance().pWorldViewport; }
		static Selection& GetSelection();
		static EditorController& Controller();
		static EditorVisual* GetDraggingVisual();

		// As Input Observer
		virtual void OnLeftClickPressed() override;
		virtual void OnLeftClickReleased() override;
		virtual void OnLeftClickDragInProgress(const MousePosition& mouseStartDragPos, const MousePosition& mouseCurrDragPos) override;
		virtual void OnLeftClickDragEnd(const MousePosition& mouseStartDragPos, const MousePosition& mouseCurrDragPos) override;
		virtual void OnScroll(int deltaMove) override;
		virtual void OnTranslateModePressed() override;
		virtual void OnScaleModePressed() override;
		virtual void OnRotateModePressed() override;
		virtual void OnDeletePressed() override;
		virtual void OnCutPressed() override;
		virtual void OnSavePressed() override;
		virtual void OnLoadPressed() override;
		virtual void OnCopyPressed() override;
		virtual void OnPastePressed() override;

		// State Queries
		static bool IsMouseInWorld();
		static bool IsDraggingSomething();
		static bool IsWorldInFocus();
		static bool IsGoToRequested();

		// Math Queries
		static Vec3 GetPointInFrontOfView(float dist = 40.f);
		static ImVec2 GetNormalizedMousePosition(const ImVec2& inPos);
		static void SortMousePointsClockwise(MousePosition* arr);

	private:
		// Private helpers
		void Update();
		void UpdateDockspace();
		bool HandleEditorVisualInteraction();
		bool HandleObjectSelection();
		void ReleaseDrag();

		// Overlap detection helpers
		GameObject* GetClosestObjectIntersecting(const Vec3& rayOrigin, const Vec3& rayDir);
		EditorVisual* GetEditorVisualIntersecting();

		// Meyers singleton
		static EditorGui& GetInstance();

		// Editor components
		Viewport* pWorldViewport;
		Selection selection;
		Clipboard clipboard;
		EditorController controller;

		// Changing state
		Vec3 clickPoint;
		EditorVisual* pVizDragging;
		bool fullscreen = true;
		bool padding = false;

		// Sub-windows
		ConsoleWindow console;
		HierarchyWindow hierarchy;
		SettingsWindow settings;
		InspectorWindow inspector;
		WorldWindow world;
		AssetsWindow assets;
	};

	void Log(const char* msg, ...);
}

#endif