#ifndef _EditorInputObserver
#define _EditorInputObserver

#include "DLink.h"

namespace Azul
{
	// Represents a 2D vector for mouse position
	struct MousePosition
	{
		MousePosition() = default;
		MousePosition(int _x, int _y) : x(_x), y(_y) {}
		MousePosition(const MousePosition&) = default;
		MousePosition& operator=(const MousePosition&) = default;
		~MousePosition() = default;

		bool operator==(const MousePosition o) { return x == o.x && y == o.y; }
		bool operator!=(const MousePosition o) { return !(*this == o); }

		int x = 0;
		int y = 0;
	};

	enum class InputModifier
	{
		Shift,
		Control,
		Alt,
		None
	};

	// Observer for EditorInput events
	class EditorInputObserver : public DLink
	{
	public:
		virtual void OnLeftClickPressed() {  }
		virtual void OnRightClickPressed() {  }
		virtual void OnLeftClickReleased() {  }
		virtual void OnRightClickReleased() { }

		virtual void OnTranslateModePressed() {  }
		virtual void OnScaleModePressed() {  }
		virtual void OnRotateModePressed() {  }

		virtual void OnUndoPressed() {}
		virtual void OnRedoPressed() {}
		virtual void OnDeletePressed() {}
		virtual void OnCutPressed() {}
		virtual void OnSavePressed() {}
		virtual void OnLoadPressed() {}
		virtual void OnCopyPressed() {}
		virtual void OnPastePressed() {}

		virtual void OnScroll(int delta)
		{
			static_cast<void>(delta);
		}

		virtual void OnLeftClickDragStart(const MousePosition& start)
		{
			static_cast<void>(start);
		}

		virtual void OnLeftClickDragInProgress(const MousePosition& start, const MousePosition& curr)
		{
			static_cast<void>(start);
			static_cast<void>(curr);
		}

		virtual void OnLeftClickDragEnd(const MousePosition& start, const MousePosition& end)
		{
			static_cast<void>(start);
			static_cast<void>(end);
		}

		virtual ~EditorInputObserver() = default;

		// Inherited via DLink
		void Wash() override;
		bool Compare(DLink* pTargetNode) override;
	};
}

#endif
