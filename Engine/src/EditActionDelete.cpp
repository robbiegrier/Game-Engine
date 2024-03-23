#include "EditActionDelete.h"
#include "GameObjectManager.h"
#include "EditorGui.h"
#include "EditorObjectReference.h"
#include "PCSTreeForwardIterator.h"

namespace Azul
{
	EditActionDelete::~EditActionDelete()
	{
		if (ownSpawnedObject)
		{
			for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
				DeleteObjectAsSubtree(pCurr->GetGameObject());
			}
		}
	}

	void EditActionDelete::Execute()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();

			pCurr->StorePCSCache();

			GameObjectManager::DespawnObject(pCurr->GetGameObject());
			EditorGui::OnGameObjectRemoved(pCurr->GetGameObject());
		}

		ownSpawnedObject = true;
	}

	void EditActionDelete::Undo()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			char protoName[GameObject::NAME_SIZE];
			pCurr->GetGameObject()->GetName(protoName, GameObject::NAME_SIZE);
			GameObjectManager::SpawnObject(protoName, pCurr->GetGameObject(), pCurr->GetGameObject()->GetRelativeLocation());

			GameObjectManager::ReparentObject(pCurr->GetGameObject(), pCurr->GetPCSCache().pParent, pCurr->GetPCSCache().pPrev, false);
		}

		ownSpawnedObject = false;
	}

	void EditActionDelete::Dump()
	{
		Trace::out("EditActionDelete\n");
	}

	void EditActionDelete::DeleteObjectAsSubtree(GameObject* pObject)
	{
		Trace::out("DeleteObjectAsSubtree: %s\n", pObject->GetNamePtr());
		for (PCSTreeForwardIterator it(pObject); !it.IsDone();)
		{
			GameObject* pCurr = (GameObject*)it.Current();
			it.Next();
			Trace::out("deleting %s\n", pCurr->GetNamePtr());
			delete pCurr;
		}
	}
}