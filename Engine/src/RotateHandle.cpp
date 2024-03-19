#include "RotateHandle.h"
#include "GOFlatTexture.h"
#include "MeshManager.h"
#include "TextureObjectManager.h"
#include "ShaderObjectManager.h"
#include "EditorGui.h"
#include "Selection.h"
#include "GameObject.h"
#include "EditorObjectReference.h"
#include "CameraManager.h"
#include "EditActionRotate.h"
#include "EditorMath.h"

namespace Azul
{
	RotateHandle::RotateHandle()
	{
		GraphicsObject* pGO = new GOFlatTexture(
			MeshManager::Find(Mesh::Name::RotateHandle),
			ShaderObjectManager::Find(ShaderObject::Name::EditorVisual),
			TextureObjectManager::Find(TextureObject::Name::Red)
		);

		SetGraphicsObject(pGO);
	}

	RotateHandle::~RotateHandle()
	{
	}

	void RotateHandle::OnDragEnter()
	{
		const Vec3 mutationAxis = GetMutationAxis();
		const ImVec2 mousePos = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
		const Vec3 mouseRayDir = CameraManager::GetCurrentCamera()->GetRay(mousePos.x, mousePos.y);
		const Vec3 mouseRayOrigin = CameraManager::GetCurrentCamera()->GetLocation();

		const Vec3 frameDragPoint = EditorMath::GetRayPointOnPlane(GetLocation(), mutationAxis, mouseRayOrigin, mouseRayDir);

		const Vec3 toStart = frameDragPoint - GetLocation();
		const Vec3 tangent = toStart.cross(mutationAxis);

		const Vec3 frameProjPoint = EditorMath::ProjectVector(Vec3(), tangent);

		startDragPoint = frameDragPoint;
		prevTangentPoint = frameProjPoint;
		prevDragPoint = frameDragPoint;
		totalDelta = Quat();
	}

	void RotateHandle::OnDragUpdate()
	{
		const Vec3 mutationAxis = GetMutationAxis();
		const ImVec2 mousePos = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
		const Vec3 mouseRayDir = CameraManager::GetCurrentCamera()->GetRay(mousePos.x, mousePos.y);
		const Vec3 mouseRayOrigin = CameraManager::GetCurrentCamera()->GetLocation();

		const Vec3 frameDragPoint = EditorMath::GetRayPointOnPlane(GetLocation(), mutationAxis, mouseRayOrigin, mouseRayDir);
		const Vec3 toDragPoint = frameDragPoint - startDragPoint;

		const Vec3 toStart = GetLocation() - startDragPoint;
		const Vec3 tangent = toStart.cross(mutationAxis);

		const Vec3 frameProjPoint = EditorMath::ProjectVector(toDragPoint, tangent);
		const Vec3 toFrameProjPoint = frameProjPoint - prevTangentPoint;

		float scalar = (frameProjPoint - prevTangentPoint).len() * DRAG_SCALAR_MODIFIER;

		if (toFrameProjPoint.dot(tangent) < 0.f)
		{
			scalar = -scalar;
		}

		Quat deltaRotationWorldSpace(Axis::AxisAngle, GetMutationAxis(), scalar);

		for (Iterator& it = *pParentSelection->GetSelectedObjectReferences().GetIterator(); !it.IsDone(); it.Next())
		{
			EditorObjectReference* pCurr = (EditorObjectReference*)it.Curr();
			pCurr->GetGameObject()->AddWorldRotationOffset(deltaRotationWorldSpace);
		}

		prevTangentPoint = frameProjPoint;
		prevDragPoint = frameDragPoint;
		totalDelta *= deltaRotationWorldSpace;
	}

	void RotateHandle::OnDragExit()
	{
		EditActionRotate* pAction = new EditActionRotate();
		pAction->SetObjects(pParentSelection->GetSelectedObjectReferences());
		pAction->SetRotation(totalDelta);
		pAction->Commit();
	}

	float RotateHandle::GetMouseOverDistance() const
	{
		const Vec3 mutationAxis = GetMutationAxis();

		const ImVec2 mousePos = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
		const Vec3 mouseRayDir = CameraManager::GetCurrentCamera()->GetRay(mousePos.x, mousePos.y);
		const Vec3 mouseRayOrigin = CameraManager::GetCurrentCamera()->GetLocation();

		if (EditorMath::RayVsPlaneIntersection(GetLocation(), mutationAxis, mouseRayOrigin, mouseRayDir))
		{
			const Vec3 pointOnPlane = EditorMath::GetRayPointOnPlane(GetLocation(), mutationAxis, mouseRayOrigin, mouseRayDir);

			const float distToCamera = (mouseRayOrigin - GetLocation()).len();
			float targetDistance = CAMERA_DIST_MODIFIER * distToCamera;

			const float clickDistanceFromOrigin = (pointOnPlane - GetLocation()).len();
			const float error = fabsf(clickDistanceFromOrigin - targetDistance);

			return error;
		}

		return std::numeric_limits<float>().max();
	}

	bool RotateHandle::IsMouseOver(float tolerance) const
	{
		return GetMouseOverDistance() < tolerance * TOLERANCE_MODIFIER;
	}
}