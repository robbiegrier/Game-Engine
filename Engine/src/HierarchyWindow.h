#ifndef _HierarchyWindow
#define _HierarchyWindow

namespace Azul
{
	class GameObject;

	// Shows the game object hierachy. This should become more interactive and functional.
	class HierarchyWindow
	{
	public:
		// Big four
		HierarchyWindow() = default;
		HierarchyWindow(const HierarchyWindow&) = delete;
		HierarchyWindow& operator=(const HierarchyWindow&) = delete;
		~HierarchyWindow() = default;

		void Update();

	private:
		void TreeBuildHelper(class PCSNode* pNode, bool& selection);

		void GoTo(GameObject* pObject, bool& selectionBool);
		void Selection(GameObject* pObject, bool selectionBool);
		void DragDrop(GameObject* pObject);
		void ReorderSpacer(GameObject* pParent, GameObject* pPrev);

		void PerformObjectReorder(GameObject* pObject, GameObject* pParent, GameObject* pPrev);

		bool open;
	};
}

#endif
