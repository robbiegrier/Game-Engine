#include "EditActionRotate.h"
#include "EditorObjectReference.h"
#include "GameObject.h"

namespace Azul
{
	void EditActionRotate::Execute()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->GetGameObject()->AddWorldRotationOffset(rotation);
		}
	}

	void EditActionRotate::Undo()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->GetGameObject()->AddWorldRotationOffset(rotation.getInv());
		}
	}

	void EditActionRotate::Dump()
	{
		Trace::out("EditActionRotate: [%.3f %.3f %.3f %.3f]\n", rotation[x], rotation[y], rotation[z], rotation[w]);
	}

	void EditActionRotate::SetRotation(const Quat& inRot)
	{
		rotation = inRot;
	}

	const Quat& EditActionRotate::GetRotation() const
	{
		return rotation;
	}
}