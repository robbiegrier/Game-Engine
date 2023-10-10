#include "MathEngine.h"
#include "GameObject.h"
#include "Game.h"
#include "Camera.h"

namespace Azul
{
	extern Camera* pHackCamera;

	GameObject::GameObject(GraphicsObject* pInGraphicsObject)
		: pPos{ new Vec3(0.f, 0.f, 0.f) },
		pScale{ new Vec3(1.f, 1.f, 1.f) },
		pRotation{ new Rot(Rot1::X, 0.f) },
		pWorld{ new Mat4(Special::Identity) },
		pGraphicsObject(pInGraphicsObject)
	{
		assert(pPos);
		assert(pScale);
		assert(pRotation);
		assert(pWorld);
		assert(pGraphicsObject);
	}

	GameObject::~GameObject()
	{
		delete pPos;
		delete pScale;
		delete pRotation;
		delete pGraphicsObject;
		delete pWorld;
	}

	void GameObject::SetRelativeLocation(const Vec3& v)
	{
		pPos->set(v);
	}

	void GameObject::SetRelativeScale(const Vec3& v)
	{
		pScale->set(v);
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

	void GameObject::Update(float deltaTime)
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

		Tick(deltaTime);
	}

	void GameObject::Draw()
	{
		pGraphicsObject->Render();
	}
}