#include "EditAction.h"
#include "ListNode.h"
#include "EditorObjectReference.h"
#include "EditorHistory.h"

namespace Azul
{
	void EditAction::SetName(Name inName)
	{
		name = inName;
	}

	EditAction::Name EditAction::GetName() const
	{
		return name;
	}

	const char* EditAction::NameToString()
	{
		switch (name)
		{
		case Name::Translate: return "Translate";
		case Name::Scale: return "Scale";
		case Name::Rotate: return "Rotate";
		case Name::Spawn: return "Spawn";
		case Name::Delete: return "Delete";
		case Name::Null: return "Null";
		default: return "None";
		}
	}

	void EditAction::SetObjects(const DLinkedList& inObjectRefs)
	{
		ClearObjects();

		for (Iterator& it = *inObjectRefs.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			EditorObjectReference* pNewRef = new EditorObjectReference(pCurr->GetGameObject());
			objects.Add(pNewRef);
		}
	}

	void EditAction::SetSingleObject(GameObject* pSingleObject)
	{
		ClearObjects();

		EditorObjectReference* pRef = new EditorObjectReference(pSingleObject);
		objects.Add(pRef);
	}

	void EditAction::ClearObjects()
	{
		while (!objects.IsEmpty())
		{
			DLink* tmp = objects.Pop();
			delete tmp;
		}
	}

	void EditAction::Commit()
	{
		EditorHistory::CommitAction(this);
	}

	void EditAction::Wash()
	{
		ClearObjects();
	}

	bool EditAction::Compare(DLink* pTargetNode)
	{
		return name == ((EditAction*)pTargetNode)->GetName();
	}
}