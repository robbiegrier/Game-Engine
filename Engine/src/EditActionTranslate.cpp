#include "EditActionTranslate.h"
#include "EditorObjectReference.h"
#include "GameObject.h"
#include "EditorMath.h"

namespace Azul
{
	void EditActionTranslate::Execute()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->GetGameObject()->AddWorldLocationOffset(translation);
		}
	}

	void EditActionTranslate::Undo()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->GetGameObject()->AddWorldLocationOffset(-translation);
		}
	}

	void EditActionTranslate::Dump()
	{
		Trace::out("EditActionTranslate: [%.3f %.3f %.3f]\n", translation[x], translation[y], translation[z]);
	}

	void EditActionTranslate::SetTranslation(const Vec3& inTranslation)
	{
		translation = inTranslation;
	}

	const Vec3& EditActionTranslate::GetTranslation() const
	{
		return translation;
	}
}