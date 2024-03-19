#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderObjectManager.h"
#include "GOConstColor.h"
#include "Engine.h"

namespace Azul
{
	bool GameObject::globalRenderShell = true;

	GameObject::GameObject(GraphicsObject* pInGraphicsObject)
		: pPos{ new Vec3(0.f, 0.f, 0.f) },
		pScale{ new Vec3(1.f, 1.f, 1.f) },
		pRotation{ new Quat(Rot1::X, 0.f) },
		pWorld{ new Mat4(Special::Identity) },
		pGraphicsObject(pInGraphicsObject),
		renderShell(true),
		typeName(Name::GameObject)
	{
		assert(pPos);
		assert(pScale);
		assert(pRotation);
		assert(pWorld);
		assert(pGraphicsObject);

		pShell = new GOConstColor(MeshManager::Find(Mesh::Name::Sphere), ShaderObjectManager::Find(ShaderObject::Name::ConstColor), Vec3(0.5f, 0.5f, 0.5f));
	}

	GameObject::~GameObject()
	{
		delete pPos;
		delete pScale;
		delete pRotation;
		delete pGraphicsObject;
		delete pWorld;
		delete pShell;
	}

	void GameObject::SetRelativeLocation(const Vec3& v)
	{
		pPos->set(v);
	}

	void GameObject::SetRelativeScale(const Vec3& v)
	{
		pScale->set(v);
	}

	void GameObject::SetRelativeScale(float s)
	{
		pScale->set(s, s, s);
	}

	void GameObject::SetRelativeRotation(const Rot& m)
	{
		*pRotation = m;
	}

	void GameObject::SetRelativeRotation(const Quat& q)
	{
		*pRotation = q;
	}

	void GameObject::AddRelativeLocationOffset(const Vec3& v)
	{
		SetRelativeLocation(GetRelativeLocation() + v);
	}

	void GameObject::AddRelativeScaleOffset(const Vec3& v)
	{
		SetRelativeScale(GetRelativeScale() + v);
	}

	void GameObject::AddRelativeRotationOffset(const Quat& q)
	{
		SetRelativeRotation((GetRelativeRotation() * q.getNorm()).getNorm());
	}

	void GameObject::AddWorldLocationOffset(const Vec3& v)
	{
		Vec3 recipScale = GetParentGameObject()->GetWorldScale();
		recipScale.set(1.f / recipScale.x(), 1.f / recipScale.y(), 1.f / recipScale.z());
		Vec3 relativeLocation(v.x() * recipScale.x(), v.y() * recipScale.y(), v.z() * recipScale.z());
		relativeLocation *= GetParentGameObject()->GetWorldRotation().getInv();
		AddRelativeLocationOffset(relativeLocation);
	}

	void GameObject::SetWorldLocation(const Vec3& v)
	{
		SetRelativeLocation(Vec3(Vec4(v, 1.f) * GetParentGameObject()->GetWorld().getInv()));
	}

	void GameObject::AddWorldRotationOffset(const Quat& q)
	{
		const Quat newWorldRotation = (GetWorldRotation() * q.getNorm()).getNorm();
		SetWorldRotation(newWorldRotation);
	}

	void GameObject::SetWorldRotation(const Quat& q)
	{
		const Quat newRelativeRotation = (q.getNorm() * GetParentGameObject()->GetWorldRotation().getInv()).getNorm();
		SetRelativeRotation(newRelativeRotation);
	}

	Vec3 GameObject::GetWorldLocation() const
	{
		return Vec3(pWorld->get(Row4::i3));
	}

	Vec3 GameObject::GetWorldScale() const
	{
		return Vec3(pWorld->get(Row4::i0).len(), pWorld->get(Row4::i1).len(), pWorld->get(Row4::i2).len());
	}

	Quat GameObject::GetWorldRotation() const
	{
		Quat totalRot = Quat(Special::Identity);

		for (const GameObject* pCurr = this; pCurr != nullptr; pCurr = pCurr->GetParentGameObject())
		{
			totalRot *= pCurr->GetRelativeRotation();
		}

		return totalRot.getNorm();
	}

	const Vec3& GameObject::GetRelativeLocation() const
	{
		return *pPos;
	}

	const Vec3& GameObject::GetRelativeScale() const
	{
		return *pScale;
	}

	const Quat& GameObject::GetRelativeRotation() const
	{
		return *pRotation;
	}

	Vec3& GameObject::RelativeLocation()
	{
		return *pPos;
	}

	Quat& GameObject::RelativeRotation()
	{
		return *pRotation;
	}

	Vec3& GameObject::RelativeScale()
	{
		return *pScale;
	}

	const Mat4& GameObject::GetWorld() const
	{
		return *pWorld;
	}

	GameObject* GameObject::GetParentGameObject() const
	{
		return (GameObject*)GetParent();
	}

	GraphicsObject* GameObject::GetGraphicsObject() const
	{
		return pGraphicsObject;
	}

	void GameObject::RenderShell()
	{
		pShell->Render();
	}

	void GameObject::SetShellColor(const Vec4& inColor)
	{
		static_cast<GOConstColor*>(pShell)->poLightColor->set(inColor);
	}

	void GameObject::SetRenderShell(bool render)
	{
		renderShell = render;
	}

	bool GameObject::GetRenderShell() const
	{
		return renderShell;
	}

	void GameObject::SetRenderShellGlobal(bool render)
	{
		globalRenderShell = render;
	}

	float GameObject::toSeconds(const AnimTime& abstractTime)
	{
		int timeInSeconds_ms = AnimTime::Quotient(abstractTime, AnimTime(AnimTime::Duration::ONE_MILLISECOND));
		AnimTime timeInMs_remainder = AnimTime::Remainder(abstractTime, AnimTime(AnimTime::Duration::ONE_MILLISECOND));
		int timeInSeconds_us_remainder = AnimTime::Quotient(timeInMs_remainder, AnimTime(AnimTime::Duration::ONE_MICROSECOND));
		return (timeInSeconds_ms * 0.001f) + (timeInSeconds_us_remainder * 0.000001f);
	}

	Vec3 GameObject::GetShellCenter() const
	{
		return Vec3(GetShellWorld().get(Row4::i3));
	}

	float GameObject::GetShellRadius() const
	{
		return Vec3(GetShellWorld().get(Row4::i0)).len();
	}

	void GameObject::SetIsSelectable(bool inIsSelectable)
	{
		isSelectable = inIsSelectable;
	}

	bool GameObject::IsSelectable() const
	{
		return isSelectable;
	}

	GameObject* GameObject::Clone()
	{
		GameObject* pClone = new GameObject(pGraphicsObject->Clone());

		pClone->SetRelativeLocation(GetRelativeLocation());
		pClone->SetRelativeRotation(Rot(GetRelativeRotation()));
		pClone->SetRelativeScale(GetRelativeScale());

		return pClone;
	}

	GameObject::Name GameObject::GetTypeName() const
	{
		return typeName;
	}

	bool GameObject::IsParentOf(GameObject* pOther)
	{
		for (GameObject* pCurr = pOther; pCurr->GetParentGameObject() != nullptr; pCurr = pCurr->GetParentGameObject())
		{
			if (pCurr->GetParentGameObject() == this)
			{
				return true;
			}
		}

		return false;
	}

	Mat4 GameObject::GetShellWorld() const
	{
		Trans shellTrans = Trans();
		float shellRadius = 0.5f;

		if (pGraphicsObject->GetModel())
		{
			shellTrans = Trans(pGraphicsObject->GetModel()->GetBoundSphereCenter());
			shellRadius = pGraphicsObject->GetModel()->GetBoundingSphereRadius();
		}

		Scale shellScale(shellRadius, shellRadius, shellRadius);

		Mat4 shellWorld = shellScale * shellTrans * *pWorld;

		float swScaleX = shellWorld.get(Row4::i0).len();
		float swScaleY = shellWorld.get(Row4::i1).len();
		float swScaleZ = shellWorld.get(Row4::i2).len();

		float maxSwScale = std::max(std::max(swScaleX, swScaleY), swScaleZ);

		shellWorld.set(Row4::i0, Vec4(1.f, 0.f, 0.f, 0.f) * maxSwScale);
		shellWorld.set(Row4::i1, Vec4(0.f, 1.f, 0.f, 0.f) * maxSwScale);
		shellWorld.set(Row4::i2, Vec4(0.f, 0.f, 1.f, 0.f) * maxSwScale);

		return shellWorld;
	}

	void GameObject::Update(AnimTime deltaTime)
	{
		static_cast<void>(deltaTime);

		Trans t(pPos->x(), pPos->y(), pPos->z());
		Scale s(*pScale);
		Rot r(*pRotation);

		*pWorld = s;
		*pWorld *= r;
		*pWorld *= t;

		if (GetParentGameObject())
		{
			*pWorld *= GetParentGameObject()->GetWorld();
		}

		pGraphicsObject->SetWorld(*pWorld);

		pShell->SetWorld(GetShellWorld());
	}

	void GameObject::Draw()
	{
		pGraphicsObject->Render();

		if (renderShell && globalRenderShell || (Engine::GetEditorMode() && alwaysRenderShell))
		{
			pShell->Render();
		}
	}
}