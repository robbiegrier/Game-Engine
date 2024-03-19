#include "Game.h"
#include "MathEngine.h"
#include "GameObjectBasic.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "Anim.h"
#include "Engine.h"

namespace Azul
{
	GameObjectBasic::GameObjectBasic(GraphicsObject* pGraphicsObject)
		: GameObjectRigid(pGraphicsObject)
	{
		poTrans = new Vec3(0.0f, 0.0f, 0.0f);
		poScale = new Vec3(1.0f, 1.0f, 1.0f);
		poQuat = new Quat(0.0f, 0.0f, 0.0f, 1.0f);
		delta_x = 0.0f;
		delta_y = 0.0f;
		delta_z = 0.0f;
		cur_rot_x = 0.0f;
		cur_rot_y = 0.0f;
		cur_rot_z = 0.0f;
	}

	GameObjectBasic::~GameObjectBasic()
	{
		delete poTrans;
		delete poScale;
		delete poQuat;
	}

	void GameObjectBasic::privUpdate(AnimTime currentTime)
	{
		static_cast<void>(currentTime);

		Trans T(*poTrans);
		Scale S(*poScale);
		Quat  Q(*poQuat);

		cur_rot_x += delta_x * Engine::GetDeltaTime();
		cur_rot_y += delta_y * Engine::GetDeltaTime();
		cur_rot_z += delta_z * Engine::GetDeltaTime();

		Rot Rx(Rot1::X, cur_rot_x);
		Rot Ry(Rot1::Y, cur_rot_y);
		Rot Rz(Rot1::Z, cur_rot_z);

		*pWorld = S * Q * Rx * Ry * Rz * T;
	}

	void GameObjectBasic::Update(AnimTime currentTime)
	{
		//privUpdate(currentTime);

		cur_rot_x += delta_x * Engine::GetDeltaTime();
		cur_rot_y += delta_y * Engine::GetDeltaTime();
		cur_rot_z += delta_z * Engine::GetDeltaTime();

		Rot Rx(Rot1::X, cur_rot_x);
		Rot Ry(Rot1::Y, cur_rot_y);
		Rot Rz(Rot1::Z, cur_rot_z);

		SetRelativeLocation(*poTrans);
		SetRelativeScale(*poScale);
		SetRelativeRotation(*poQuat * Rx * Ry * Rz);

		GameObject::Update(currentTime);
		//pGraphicsObject->SetWorld(*pWorld);
	}

	void GameObjectBasic::SetQuat(float qx, float qy, float qz, float qw)
	{
		poQuat->set(qx, qy, qz, qw);
	}

	void GameObjectBasic::SetScale(float sx, float sy, float sz)
	{
		poScale->set(sx, sy, sz);
	}

	void GameObjectBasic::SetTrans(float x, float y, float z)
	{
		poTrans->set(x, y, z);
	}

	void GameObjectBasic::SetScale(const Vec3& r)
	{
		*poScale = r;
	}

	void GameObjectBasic::SetQuat(const Quat& r)
	{
		*poQuat = r;
	}

	void GameObjectBasic::SetTrans(const Vec3& r)
	{
		*poTrans = r;
	}
}