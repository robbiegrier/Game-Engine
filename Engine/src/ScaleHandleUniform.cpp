#include "ScaleHandleUniform.h"
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
	ScaleHandleUniform::ScaleHandleUniform()
	{
		GraphicsObject* pGO = new GOFlatTexture(
			MeshManager::Find(Mesh::Name::Cube),
			ShaderObjectManager::Find(ShaderObject::Name::EditorVisual),
			TextureObjectManager::Find(TextureObject::Name::Yellow)
		);

		SetGraphicsObject(pGO);

		scaleMod = Vec3(5.f, 5.f, 5.f);
		translationMod = Vec3(1.f, -1.f, -1.f);
	}

	ScaleHandleUniform::~ScaleHandleUniform()
	{
	}

	void ScaleHandleUniform::OnDragEnter()
	{
		SetOriginVarsFromAxisOffset();
		objectTrueStartSize = pParentSelection->GetFocusObject()->GetGameObject()->GetShellRadius() / 2.f;
		startScale = pParentSelection->GetFocusObject()->GetGameObject()->GetRelativeScale();
	}

	void ScaleHandleUniform::OnDragUpdate()
	{
		const Vec3 worldSpaceTotalDelta = GetClosestApproachFromMouseRay() - (dragStartPos + originalDragOffset);
		const Vec3 worldSpaceFramewiseDelta = worldSpaceTotalDelta - prevVal;
		const float sign = (worldSpaceTotalDelta - prevVal).dot(GetMutationAxis()) > 0 ? 1.f : -1.f;
		const float framewiseScaling = worldSpaceFramewiseDelta.len() * sign;

		for (Iterator& it = *pParentSelection->GetSelectedObjectReferences().GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			const Vec3 framewiseLocalAxisScaling = Vec3(1.f, 1.f, 1.f) * framewiseScaling;

			// Clamp user input scale
			Vec3 clampedScale = pCurr->GetGameObject()->GetRelativeScale() + framewiseLocalAxisScaling;
			clampedScale.set(std::max(clampedScale.x(), MIN_SCALE_CLAMP), std::max(clampedScale.y(), MIN_SCALE_CLAMP), std::max(clampedScale.z(), MIN_SCALE_CLAMP));
			pCurr->GetGameObject()->SetRelativeScale(clampedScale);
		}

		prevVal = worldSpaceTotalDelta;
	}

	void ScaleHandleUniform::OnDragExit()
	{
		prevVal = Vec3();
		//scaleMod = Vec3(1.f, 1.f, 1.f);

		endScale = pParentSelection->GetFocusObject()->GetGameObject()->GetRelativeScale();

		EditActionScale* pAction = new EditActionScale();
		pAction->SetObjects(pParentSelection->GetSelectedObjectReferences());
		pAction->SetScale(endScale - startScale);
		pAction->Commit();
	}

	float ScaleHandleUniform::GetMouseOverDistance() const
	{
		return GetAxisDistanceToMouseRay();
	}

	bool ScaleHandleUniform::IsMouseOver(float tolerance) const
	{
		const ImVec2 vec2 = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
		const Vec3 rayDir = CameraManager::GetCurrentCamera()->GetRay(vec2.x, vec2.y);
		const Vec3 rayOrigin = CameraManager::GetCurrentCamera()->GetLocation();
		float distOut;
		float error;

		float scaledTolerance = tolerance * 20.f;
		Scale toleranceMod(Vec3(scaledTolerance, scaledTolerance, scaledTolerance));

		bool isIntersect = EditorMath::RayVsOBBIntersection(rayOrigin, rayDir, Vec3(-.25f, -.25f, -.25f), Vec3(.25f, .25f, .25f), toleranceMod * GetWorld(), distOut, error);

		return isIntersect;
	}
}