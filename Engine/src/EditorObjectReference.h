#ifndef _EditorObjectReference
#define _EditorObjectReference

#include "DLink.h"

namespace Azul
{
	class GameObject;
	class GraphicsObject;

	// Used as a level of indirection between editor systems and game objects
	// such that GameObject does not need functionality that is specific to the
	// editor. Also, used as DLink node for non-owning linked lists of game objects.
	class EditorObjectReference : public DLink
	{
	public:
		// Big four
		EditorObjectReference() = delete;
		explicit EditorObjectReference(GameObject* pInObject);
		EditorObjectReference(const EditorObjectReference&) = delete;
		EditorObjectReference& operator=(const EditorObjectReference&) = delete;
		~EditorObjectReference();

		// Query game object
		void SetGameObject(GameObject* pInObject);
		GameObject* GetGameObject() const;

		// Selection events
		void OnSelected();
		void OnDeselected();

		// Draw the reference indicators
		void Draw();

		struct PCSCache
		{
			GameObject* pParent = nullptr;
			GameObject* pPrev = nullptr;
		};

		void StorePCSCache();
		const PCSCache& GetPCSCache() const;

	private:
		void Wash() override;
		bool Compare(DLink* pTargetNode) override;

		GameObject* pObject;
		GraphicsObject* pSelectionIndicator;
		PCSCache pcsCache;
	};
}

#endif