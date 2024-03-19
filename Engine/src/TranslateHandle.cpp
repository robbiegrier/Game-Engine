#include "TranslateHandle.h"
#include "GOFlatTexture.h"
#include "MeshManager.h"
#include "TextureObjectManager.h"
#include "ShaderObjectManager.h"
#include "EditorGui.h"
#include "Selection.h"
#include "GameObject.h"
#include "EditorObjectReference.h"
#include "CameraManager.h"
#include "EditActionTranslate.h"
#include "EditorMath.h"

namespace Azul
{
	TranslateHandle::TranslateHandle()
	{
		GraphicsObject* pGO = new GOFlatTexture(
			MeshManager::Find(Mesh::Name::TranslateHandle),
			ShaderObjectManager::Find(ShaderObject::Name::EditorVisual),
			TextureObjectManager::Find(TextureObject::Name::Red)
		);

		SetGraphicsObject(pGO);
	}

	TranslateHandle::~TranslateHandle()
	{
	}

	void TranslateHandle::OnDragEnter()
	{
		SetOriginVarsFromAxisOffset();
	}

	void TranslateHandle::OnDragUpdate()
	{
		const Vec3 worldSpaceTotalDelta = GetClosestApproachFromMouseRay() - (dragStartPos + originalDragOffset);
		const Vec3 worldSpaceFramewiseDelta = worldSpaceTotalDelta - prevVal;
		const float sign = (worldSpaceTotalDelta - prevVal).dot(GetMutationAxis()) > 0 ? 1.f : -1.f;
		const float framewiseScalar = worldSpaceFramewiseDelta.len() * sign;
		const Vec3 framewiseLocalAxisMutation = GetMutationAxis() * framewiseScalar;

		for (Iterator& it = *pParentSelection->GetSelectedObjectReferences().GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->GetGameObject()->AddWorldLocationOffset(framewiseLocalAxisMutation);
		}

		prevVal = worldSpaceTotalDelta;
	}

	void TranslateHandle::OnDragExit()
	{
		prevVal = Vec3();
		EditActionTranslate* pAction = new EditActionTranslate();
		pAction->SetObjects(pParentSelection->GetSelectedObjectReferences());
		pAction->SetTranslation(GetLocation() - dragStartPos);
		pAction->Commit();
	}

	float TranslateHandle::GetMouseOverDistance() const
	{
		return GetAxisDistanceToMouseRay();
	}

	bool TranslateHandle::IsMouseOver(float tolerance) const
	{
		return IsMouseOverAxisHandle(tolerance);
	}
}