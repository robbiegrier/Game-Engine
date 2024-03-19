#include "EditActionRename.h"
#include "EditorObjectReference.h"

namespace Azul
{
	void EditActionRename::Execute()
	{
		GameObject* pObject = ((EditorObjectReference*)objects.First())->GetGameObject();
		pObject->SetName(newName);
	}

	void EditActionRename::Undo()
	{
		GameObject* pObject = ((EditorObjectReference*)objects.First())->GetGameObject();
		pObject->SetName(prevName);
	}

	void EditActionRename::Dump()
	{
		Trace::out("Rename object action [%s] => [%s]\n", prevName, newName);
	}

	void EditActionRename::SetPrevName(const char* pIn)
	{
		memcpy_s(prevName, GameObject::NAME_SIZE, pIn, GameObject::NAME_SIZE);
	}

	void EditActionRename::SetNewName(const char* pIn)
	{
		memcpy_s(newName, GameObject::NAME_SIZE, pIn, GameObject::NAME_SIZE);
	}

	const char* EditActionRename::GetNewName() const
	{
		return newName;
	}

	const char* EditActionRename::GetPrevName() const
	{
		return prevName;
	}
}