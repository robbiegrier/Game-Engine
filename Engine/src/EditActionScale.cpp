#include "EditActionScale.h"
#include "EditorObjectReference.h"
#include "GameObject.h"

namespace Azul
{
	void EditActionScale::Execute()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->GetGameObject()->AddRelativeScaleOffset(scale);
		}
	}

	void EditActionScale::Undo()
	{
		for (Iterator& it = *objects.GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->GetGameObject()->AddRelativeScaleOffset(-scale);
		}
	}

	void EditActionScale::Dump()
	{
		Trace::out("EditActionScale: [%.3f %.3f %.3f]\n", scale[x], scale[y], scale[z]);
	}

	void EditActionScale::SetScale(const Vec3& inScale)
	{
		scale = inScale;
	}

	const Vec3& EditActionScale::GetScale() const
	{
		return scale;
	}
}