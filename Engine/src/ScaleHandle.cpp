#include "ScaleHandle.h"
#include "GOFlatTexture.h"
#include "MeshManager.h"
#include "TextureObjectManager.h"
#include "ShaderObjectManager.h"
#include "EditorGui.h"
#include "Selection.h"
#include "GameObject.h"
#include "EditorObjectReference.h"
#include "CameraManager.h"
#include "EditActionScale.h"
#include "EditorMath.h"

namespace Azul
{
	ScaleHandle::ScaleHandle()
	{
		GraphicsObject* pGO = new GOFlatTexture(
			MeshManager::Find(Mesh::Name::ScaleHandle),
			ShaderObjectManager::Find(ShaderObject::Name::EditorVisual),
			TextureObjectManager::Find(TextureObject::Name::Red)
		);

		SetGraphicsObject(pGO);
	}

	ScaleHandle::~ScaleHandle()
	{
	}

	void ScaleHandle::OnDragEnter()
	{
		SetOriginVarsFromAxisOffset();
		objectTrueStartSize = pParentSelection->GetFocusObject()->GetGameObject()->GetShellRadius() / 2.f;
		startScale = pParentSelection->GetFocusObject()->GetGameObject()->GetRelativeScale();
	}

	void ScaleHandle::OnDragUpdate()
	{
		const Vec3 worldSpaceTotalDelta = GetClosestApproachFromMouseRay() - (dragStartPos + originalDragOffset);
		const Vec3 worldSpaceFramewiseDelta = worldSpaceTotalDelta - prevVal;
		const float sign = (worldSpaceTotalDelta - prevVal).dot(GetMutationAxis()) > 0 ? 1.f : -1.f;
		const float framewiseScaling = worldSpaceFramewiseDelta.len() * sign;

		for (Iterator& it = *pParentSelection->GetSelectedObjectReferences().GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			const Vec3 framewiseLocalAxisScaling = GetLocalAxis() * framewiseScaling;

			// Clamp user input scale
			Vec3 clampedScale = pCurr->GetGameObject()->GetRelativeScale() + framewiseLocalAxisScaling;
			clampedScale.set(std::max(clampedScale.x(), MIN_SCALE_CLAMP), std::max(clampedScale.y(), MIN_SCALE_CLAMP), std::max(clampedScale.z(), MIN_SCALE_CLAMP));
			pCurr->GetGameObject()->SetRelativeScale(clampedScale);
		}

		prevVal = worldSpaceTotalDelta;
	}

	void ScaleHandle::OnDragExit()
	{
		prevVal = Vec3();
		scaleMod = Vec3(1.f, 1.f, 1.f);

		endScale = pParentSelection->GetFocusObject()->GetGameObject()->GetRelativeScale();

		EditActionScale* pAction = new EditActionScale();
		pAction->SetObjects(pParentSelection->GetSelectedObjectReferences());
		pAction->SetScale(endScale - startScale);
		pAction->Commit();
	}

	float ScaleHandle::GetMouseOverDistance() const
	{
		return GetAxisDistanceToMouseRay();
	}

	bool ScaleHandle::IsMouseOver(float tolerance) const
	{
		return IsMouseOverAxisHandle(tolerance);
	}
}