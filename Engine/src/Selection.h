#ifndef _Selection
#define _Selection

#include "DLinkedList.h"
#include "MathEngine.h"

namespace Azul
{
	class GameObject;
	class EditorObjectReference;
	class EditorVisual;

	// Represents the set of objects that are currently selected. Objects must be unique.
	class Selection
	{
	public:
		// Big four
		Selection();
		Selection(const Selection&) = delete;
		Selection operator=(const Selection&) = delete;
		~Selection();

		// Object set
		void AddObject(GameObject* pInObject);
		void RemoveObject(GameObject* pInObject);
		void DeleteObjects();
		bool IsEmpty() const;
		void Clear();

		// Main loop events
		void Update();
		void Draw();

		// Get components
		const DLinkedList& GetSelectedObjectReferences() const;
		const DLinkedList& GetEditorVisuals() const;
		EditorObjectReference* GetFocusObject() const;

		// Change mode
		void EnterTranslateMode();
		void EnterScaleMode();
		void EnterRotateMode();

	private:
		void RemoveObjectReference(EditorObjectReference* pRef);
		void OnModeChanged();
		void ClearVisuals();

		DLinkedList selectedObjects;
		DLinkedList editorVisuals;
	};
}

#endif
