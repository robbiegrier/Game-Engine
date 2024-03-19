#include "EditorVisual.h"
#include "GOFlatTexture.h"
#include "GOConstColor.h"
#include "MeshManager.h"
#include "TextureObjectManager.h"
#include "ShaderObjectManager.h"
#include "CameraManager.h"
#include "EditorGui.h"
#include "Selection.h"
#include "GameObject.h"
#include "EditorObjectReference.h"
#include "EditorMath.h"

namespace Azul
{
	EditorVisual::EditorVisual()
		: translation{ Vec3() },
		scale{ Vec3(1.f, 1.f, 1.f) },
		rotation{ Quat(Rot1::X, 0.f) },
		pGraphicsObject(nullptr),
		pParentSelection(nullptr),
		scaleMod(Vec3(1.f, 1.f, 1.f))
	{
		pShell = new GOConstColor(MeshManager::Find(Mesh::Name::Sphere), ShaderObjectManager::Find(ShaderObject::Name::ConstColor), Vec3(0.5f, 0.5f, 0.5f));
	}

	EditorVisual::~EditorVisual()
	{
		delete pGraphicsObject;
		delete pShell;
	}

	void EditorVisual::Update()
	{
		if ((IsMouseOver(EditorGui::WIDGET_TOLERANCE) && !EditorGui::IsDraggingSomething()) || EditorGui::GetDraggingVisual() == this)
		{
			AddTempTexture(TextureObject::Name::Hovered);
		}
		else if (EditorGui::IsDraggingSomething())
		{
			AddTempTexture(TextureObject::Name::Disabled);
		}
		else
		{
			RevertTexture();
		}

		const Vec3& camPos = CameraManager::GetCurrentCamera()->GetLocation();
		const float dist = (camPos - GetShellCenter()).len();
		const float scaleVal = dist * 0.01f;

		SetScale(Vec3(scaleVal * scaleMod.x(), scaleVal * scaleMod.y(), scaleVal * scaleMod.z()));
		SetRotation(Rot(Orient::LocalToWorld, GetMutationAxis(), GetMutationUp()));
		SetLocationFromObject(pParentSelection->GetFocusObject()->GetGameObject());

		Trans tMod(translationMod);

		Trans t(translation);
		Scale s(scale);
		Rot r(rotation);

		Mat4 world = tMod * s * r * t;
		worldCache = world;

		pGraphicsObject->SetWorld(world);
		pShell->SetWorld(GetShellWorld());
	}

	void EditorVisual::Draw()
	{
		pGraphicsObject->Render();
	}

	void EditorVisual::OnDragExit()
	{
		GOFlatTexture* pGOTex = (GOFlatTexture*)pGraphicsObject;
		pGOTex->pTex = pOriginalTexture;
	}

	void EditorVisual::SetLocation(const Vec3& v)
	{
		translation.set(v);
	}

	void EditorVisual::SetScale(const Vec3& v)
	{
		scale.set(v);
	}

	void EditorVisual::SetRotation(const Rot& m)
	{
		rotation.set(m);
	}

	void EditorVisual::SetLocationFromObject(GameObject* pObject)
	{
		SetLocation(pObject->GetWorldLocation());
	}

	const Vec3& EditorVisual::GetLocation() const
	{
		return translation;
	}

	const Mat4& EditorVisual::GetWorld() const
	{
		return worldCache;
	}

	GraphicsObject* EditorVisual::GetGraphicsObject() const
	{
		return pGraphicsObject;
	}

	void EditorVisual::SetTexture(TextureObject::Name inTexture)
	{
		pOriginalTexture = TextureObjectManager::Find(inTexture);
		GOFlatTexture* pGOTex = (GOFlatTexture*)pGraphicsObject;
		pGOTex->pTex = pOriginalTexture;
	}

	void EditorVisual::SetMutationAxis(MutationAxis inAxis)
	{
		mutationAxisType = inAxis;
	}

	void EditorVisual::Wash()
	{
		delete pGraphicsObject;
		pGraphicsObject = nullptr;
		pOriginalTexture = nullptr;
	}

	bool EditorVisual::Compare(DLink* pTargetNode)
	{
		return this == pTargetNode;
	}

	Vec3 EditorVisual::GetShellCenter() const
	{
		return Vec3(GetShellWorld().get(Row4::i3));
	}

	float EditorVisual::GetShellRadius() const
	{
		return Vec3(GetShellWorld().get(Row4::i0)).len();
	}

	void EditorVisual::SetParentSelection(Selection* pInSelection)
	{
		pParentSelection = pInSelection;
	}

	Mat4 EditorVisual::GetShellWorld() const
	{
		Trans shellTrans(pGraphicsObject->GetModel()->GetBoundSphereCenter());
		const float shellRadius = pGraphicsObject->GetModel()->GetBoundingSphereRadius();
		Scale shellScale(shellRadius, shellRadius, shellRadius);

		Trans t(translation);
		Scale s(scale);
		Rot r = Rot(rotation);

		Mat4 world = s * r * t;

		Mat4 shellWorld = shellScale * shellTrans * world;

		float swScaleX = shellWorld.get(Row4::i0).len();
		float swScaleY = shellWorld.get(Row4::i1).len();
		float swScaleZ = shellWorld.get(Row4::i2).len();

		float maxSwScale = std::max(std::max(swScaleX, swScaleY), swScaleZ);

		shellWorld.set(Row4::i0, Vec4(1.f, 0.f, 0.f, 0.f) * maxSwScale);
		shellWorld.set(Row4::i1, Vec4(0.f, 1.f, 0.f, 0.f) * maxSwScale);
		shellWorld.set(Row4::i2, Vec4(0.f, 0.f, 1.f, 0.f) * maxSwScale);

		return shellWorld;
	}

	Vec3 EditorVisual::GetMutationAxis() const
	{
		EditorObjectReference* pRef = (EditorObjectReference*)pParentSelection->GetSelectedObjectReferences().First();
		Rot worldRot = Rot(pRef->GetGameObject()->GetWorldRotation());

		switch (mutationAxisType)
		{
		case MutationAxis::X:
			return Vec3(worldRot.get(Row4::i0)).getNorm();
		case MutationAxis::Y:
			return Vec3(worldRot.get(Row4::i1)).getNorm();
		case MutationAxis::Z:
			return Vec3(worldRot.get(Row4::i2)).getNorm();
		}

		return Vec3();
	}

	Vec3 EditorVisual::GetLocalAxis() const
	{
		switch (mutationAxisType)
		{
		case MutationAxis::X: return Vec3(1.f, 0.f, 0.f);
		case MutationAxis::Y: return Vec3(0.f, 1.f, 0.f);
		case MutationAxis::Z: return Vec3(0.f, 0.f, 1.f);
		}

		return Vec3();
	}

	Vec3 EditorVisual::GetMutationUp() const
	{
		EditorObjectReference* pRef = (EditorObjectReference*)pParentSelection->GetSelectedObjectReferences().First();
		const Mat4& mat = pRef->GetGameObject()->GetWorld();

		switch (mutationAxisType)
		{
		case MutationAxis::X: return Vec3(mat.get(Row4::i1).getNorm());
		case MutationAxis::Y: return Vec3(mat.get(Row4::i2).getNorm());
		case MutationAxis::Z: return Vec3(mat.get(Row4::i0).getNorm());
		}

		return Vec3();
	}

	void EditorVisual::SetGraphicsObject(GraphicsObject* pInGraphicsObject)
	{
		if (pGraphicsObject)
		{
			delete pGraphicsObject;
		}

		pGraphicsObject = pInGraphicsObject;

		GOFlatTexture* pGOTex = (GOFlatTexture*)pGraphicsObject;
		pOriginalTexture = pGOTex->pTex;
	}

	Vec3 EditorVisual::GetClosestApproachFromMouseRay() const
	{
		const Vec3 mutationAxis = GetMutationAxis();

		const ImVec2 vec2 = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
		const Vec3 rayDir = CameraManager::GetCurrentCamera()->GetRay(vec2.x, vec2.y);

		const Vec3 a0 = GetLocation() + (mutationAxis * -10000.f);
		const Vec3 b0 = CameraManager::GetCurrentCamera()->GetLocation();

		return EditorMath::ClosestPointBetweenLineAndRay(a0, mutationAxis, b0, rayDir);
	}

	float EditorVisual::GetAxisDistanceToMouseRay() const
	{
		const Vec3 mutationAxis = GetMutationAxis();

		const ImVec2 vec2 = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
		const Vec3 rayDir = CameraManager::GetCurrentCamera()->GetRay(vec2.x, vec2.y);

		const Vec3 a0 = GetLocation() + (mutationAxis * -10000.f);
		const Vec3 b0 = CameraManager::GetCurrentCamera()->GetLocation();

		const Vec3 ca = EditorMath::ClosestPointBetweenLineAndRay(a0, mutationAxis, b0, rayDir);
		const Vec3 caPrime = EditorMath::ClosestPointBetweenLineAndRay(b0, rayDir, a0, mutationAxis);

		return (ca - caPrime).len();
	}

	bool EditorVisual::IsMouseOverAxisHandle(float tolerance) const
	{
		const Vec3 mutationAxis = GetMutationAxis();

		const ImVec2 vec2 = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
		const Vec3 rayDir = CameraManager::GetCurrentCamera()->GetRay(vec2.x, vec2.y);

		const Vec3 a0 = GetLocation() + (mutationAxis * -10000.f);
		const Vec3 b0 = CameraManager::GetCurrentCamera()->GetLocation();

		const Vec3 ca = EditorMath::ClosestPointBetweenLineAndRay(a0, mutationAxis, b0, rayDir);
		const Vec3 caPrime = EditorMath::ClosestPointBetweenLineAndRay(b0, rayDir, a0, mutationAxis);

		const float dist = (ca - caPrime).len();
		const Vec3 toCa = (ca - GetLocation());
		const float vizScalar = GetShellRadius() * 2.f;
		const float scaledTolerance = vizScalar * tolerance;

		const bool isWithinTolerance = dist < scaledTolerance;
		const bool isPositive = toCa.dot(mutationAxis) > 0.f;
		const bool isNotTooFarOut = toCa.len() < vizScalar;

		return isWithinTolerance && isPositive && isNotTooFarOut;
	}

	void EditorVisual::SetOriginVarsFromAxisOffset()
	{
		dragStartPos = GetLocation();
		originalDragOffset = GetClosestApproachFromMouseRay() - GetLocation();
	}

	Vec3 EditorVisual::GetTotalDeltaDragOnMutationAxis() const
	{
		return GetClosestApproachFromMouseRay() - (GetLocation() + originalDragOffset);
	}

	void EditorVisual::RevertTexture()
	{
		GOFlatTexture* pGOTex = (GOFlatTexture*)pGraphicsObject;
		pGOTex->pTex = pOriginalTexture;
	}

	void EditorVisual::AddTempTexture(TextureObject::Name textureName)
	{
		GOFlatTexture* pGOTex = (GOFlatTexture*)pGraphicsObject;
		pGOTex->pTex = TextureObjectManager::Find(textureName);
	}
}