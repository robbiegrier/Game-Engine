#include "MathEngine.h"
#include "GameObject.h"
#include "Game.h"
#include "Camera.h"

// HACK FEST --- will eventually be in managers/singleton
extern ID3D11Device* pHackDevice;
extern ID3D11DeviceContext* pHackDeviceContext;

namespace Azul
{
	extern Camera* pHackCamera;

	GameObject::GameObject(GraphicsObject* pInGraphicsObject)
		: pPos{ new Vec3(0.f, 0.f, 0.f) },
		pScale{ new Vec3(1.f, 1.f, 1.f) },
		pRotation{ new Mat4(Mat4::Rot1::X, 0.f) },
		pWorld{ new Mat4(Mat4::Special::Identity) },
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

	void GameObject::SetRelativeRotation(const Mat4& m)
	{
		pRotation->set(m);
	}

	Vec3 GameObject::GetLocation() const
	{
		return Vec3(pWorld->get(Mat4::Row::i3));
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

		Mat4 t(Mat4::Trans::XYZ, pPos->x(), pPos->y(), pPos->z());
		Mat4 s(Mat4::Scale::XYZ, *pScale);
		Mat4& r = *pRotation;

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