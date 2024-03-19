#include "EditActionReorder.h"
#include "EditorObjectReference.h"
#include "GameObjectManager.h"

namespace Azul
{
	void EditActionReorder::Execute()
	{
		GameObject* pObject = ((EditorObjectReference*)objects.First())->GetGameObject();
		GameObjectManager::ReparentObject(pObject, pNewParent, pNewPrevSibling);
	}

	void EditActionReorder::Undo()
	{
		GameObject* pObject = ((EditorObjectReference*)objects.First())->GetGameObject();
		GameObjectManager::ReparentObject(pObject, pOldParent, pOldPrevSibling);
	}

	void EditActionReorder::Dump()
	{
		GameObject* pObject = ((EditorObjectReference*)objects.First())->GetGameObject();
		Trace::out("Reorder in tree: %s\n", pObject->GetNamePtr());
	}

	void EditActionReorder::SetReorder(GameObject* pInOldParent, GameObject* pInOldPrev, GameObject* pInNewParent, GameObject* pInNewPrev)
	{
		pOldParent = pInOldParent;
		pOldPrevSibling = pInOldPrev;
		pNewParent = pInNewParent;
		pNewPrevSibling = pInNewPrev;
	}
}