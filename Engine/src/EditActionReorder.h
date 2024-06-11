#ifndef _EditActionReorder
#define _EditActionReorder

#include "EditAction.h"
#include "GameObject.h"

namespace Azul
{
	// Action to reorder an object in the game object hierarchy.
	class EditActionReorder : public EditAction, public Align16
	{
	public:
		// Big four
		EditActionReorder() = default;
		EditActionReorder(const EditActionReorder&) = delete;
		EditActionReorder& operator=(const EditActionReorder&) = delete;
		virtual ~EditActionReorder() = default;

		// Action contract
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Dump() override;

		void SetReorder(GameObject* pInOldParent, GameObject* pInOldPrev, GameObject* pInNewParent, GameObject* pInNewPrev);

	private:
		GameObject* pOldParent;
		GameObject* pOldPrevSibling;

		GameObject* pNewParent;
		GameObject* pNewPrevSibling;
	};
}

#endif