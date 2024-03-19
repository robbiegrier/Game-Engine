#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "Anim.h"

namespace Azul
{
	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject* pGraphicsObject, Bone* _pBoneResult)
		: GameObjectAnim(pGraphicsObject)
	{
		poTrans = new Vec3(0.0f, 0.0f, 0.0f);
		poScale = new Vec3(1.0f, 1.0f, 1.0f);
		poQuat = new Quat(0.0f, 0.0f, 0.0f, 1.0f);
		poLocal = new Mat4(Special::Identity);
		poBoneOrientation = new Mat4(Special::Identity);
		pBoneResult = _pBoneResult;
	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete poTrans;
		delete poScale;
		delete poQuat;
		delete poLocal;
		delete poBoneOrientation;
	}

	void GameObjectAnimSkin::privUpdate(AnimTime currentTime)
	{
		//static_cast<void>(currentTime);

		//Trans T;
		//Scale S;
		//Quat  Q;

		//GameObjectAnimSkin* pParentObject = (GameObjectAnimSkin*)GetParent();
		//Mat4 matParent(Special::Identity);
		//if (pParentObject)
		//{
		//	matParent = *pParentObject->pWorld;

		//	// REMEMBER this is for Animation and hierachy, you need to handle models differently
		//	// Get the result bone array, from there make the matrix
		//	T.set(pBoneResult[index].T);
		//	S.set(pBoneResult[index].S);
		//	Q = pBoneResult[index].Q;
		//}
		//else
		//{
		//	T.set(0, 0, 0);
		//	S.set(1, 1, 1);
		//	Q.set(Special::Identity);
		//}

		//// world matrix
		//*poLocal = S * Q * T;
		//*pWorld = *poLocal * matParent;
	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		//privUpdate(currentTime);
		//pGraphicsObject->SetWorld(*pWorld);

		SetRelativeLocation(pBoneResult[index].T);
		SetRelativeScale(pBoneResult[index].S);
		SetRelativeRotation(Rot(pBoneResult[index].Q));

		GameObject::Update(currentTime);
	}

	void GameObjectAnimSkin::SetQuat(float qx, float qy, float qz, float qw)
	{
		poQuat->set(qx, qy, qz, qw);
	}

	void GameObjectAnimSkin::SetScale(float sx, float sy, float sz)
	{
		poScale->set(sx, sy, sz);
	}

	void GameObjectAnimSkin::SetTrans(float x, float y, float z)
	{
		poTrans->set(x, y, z);
	}

	void GameObjectAnimSkin::SetIndex(int i)
	{
		index = i;
	}

	void GameObjectAnimSkin::SetScale(const Vec3& r)
	{
		*poScale = r;
	}

	void GameObjectAnimSkin::SetQuat(const Quat& r)
	{
		*poQuat = r;
	}

	void GameObjectAnimSkin::SetTrans(const Vec3& r)
	{
		*poTrans = r;
	}

	void GameObjectAnimSkin::SetBoneOrientation(const Mat4& tmp)
	{
		*poBoneOrientation = tmp;
	}

	Mat4 GameObjectAnimSkin::GetBoneOrientation(void) const
	{
		// return a copy...
		return Mat4(*poBoneOrientation);
	}
}