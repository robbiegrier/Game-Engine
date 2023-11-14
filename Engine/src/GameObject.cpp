#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderObjectManager.h"
#include "GOConstColor.h"

namespace Azul
{
	bool GameObject::globalRenderShell = true;

	GameObject::GameObject(GraphicsObject* pInGraphicsObject)
		: pPos{ new Vec3(0.f, 0.f, 0.f) },
		pScale{ new Vec3(1.f, 1.f, 1.f) },
		pRotation{ new Rot(Rot1::X, 0.f) },
		pWorld{ new Mat4(Special::Identity) },
		pGraphicsObject(pInGraphicsObject),
		renderShell(true)
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

	Vec3 GameObject::GetLocation() const
	{
		return Vec3(pWorld->get(Row4::i3));
	}

	const Vec3& GameObject::GetRelativeLocation() const
	{
		return *pPos;
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

	void GameObject::SetRenderShell(bool render)
	{
		renderShell = render;
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

	void GameObject::Update(AnimTime deltaTime)
	{
		static_cast<void>(deltaTime);

		Trans t(pPos->x(), pPos->y(), pPos->z());
		Scale s(*pScale);
		Rot& r = *pRotation;

		*pWorld = r * s * t;

		if (GetParentGameObject())
		{
			*pWorld *= GetParentGameObject()->GetWorld();
		}

		pGraphicsObject->SetWorld(*pWorld);

		if (pGraphicsObject->GetModel())
		{
			Trans shellTrans(pGraphicsObject->GetModel()->GetBoundSphereCenter());
			float shellRadius = pGraphicsObject->GetModel()->GetBoundingSphereRadius();
			Scale shellScale(shellRadius, shellRadius, shellRadius);
			pShell->SetWorld(shellScale * shellTrans * *pWorld);
		}
		else
		{
			pShell->SetWorld(*pWorld);
		}

		Tick(deltaTime);
	}

	void GameObject::Draw()
	{
		pGraphicsObject->Render();

		if (renderShell && globalRenderShell)
		{
			pShell->Render();
		}
	}
}