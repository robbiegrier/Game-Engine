#include "EditActionSpawn.h"
#include "GameObjectManager.h"
#include "EditorGui.h"
#include "EditorObjectReference.h"
#include "EditActionDelete.h"

namespace Azul
{
	EditActionSpawn::~EditActionSpawn()
	{
		if (ownSpawnedObject)
		{
			for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
			{
				EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
				EditActionDelete::DeleteObjectAsSubtree(pCurr->GetGameObject());
			}
		}
	}

	void EditActionSpawn::Execute()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			char protoName[GameObject::NAME_SIZE];
			pCurr->GetGameObject()->GetName(protoName, GameObject::NAME_SIZE);
			GameObjectManager::SpawnObject(protoName, pCurr->GetGameObject(), pCurr->GetGameObject()->GetRelativeLocation(), pTargetParent);

			if (pCurr->GetPCSCache().pParent)
			{
				GameObjectManager::ReparentObject(pCurr->GetGameObject(), pCurr->GetPCSCache().pParent, pCurr->GetPCSCache().pPrev, false);
			}
		}

		ownSpawnedObject = false;
	}

	void EditActionSpawn::Undo()
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

	void EditActionSpawn::Dump()
	{
		Trace::out("EditActionSpawn\n");
	}

	void EditActionSpawn::SetLocation(const Vec3& inLocation)
	{
		location = inLocation;
	}

	void EditActionSpawn::SetTargetParent(GameObject* pParent)
	{
		pTargetParent = pParent;
	}
}