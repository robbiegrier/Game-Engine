#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnimSkeleton.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "Anim.h"

namespace Azul
{
	GameObjectAnimSkeleton::GameObjectAnimSkeleton(GraphicsObject* pGraphicsObject, Bone* _pBoneResult)
		: GameObjectAnim(pGraphicsObject)
	{
		assert(pGraphicsObject);
		this->poTrans = new Vec3(0.0f, 0.0f, 0.0f);
		this->poScale = new Vec3(1.0f, 1.0f, 1.0f);
		this->poQuat = new Quat(0.0f, 0.0f, 0.0f, 1.0f);
		this->poLocal = new Mat4(Special::Identity);
		this->poBoneOrientation = new Mat4(Special::Identity);
		this->pBoneResult = _pBoneResult;
		assert(_pBoneResult);
		assert(poBoneOrientation);
		assert(this->poTrans);
		assert(this->poScale);
		assert(this->poQuat);
		assert(this->poLocal);
	}

	GameObjectAnimSkeleton::~GameObjectAnimSkeleton()
	{
		delete this->poTrans;
		delete this->poScale;
		delete this->poQuat;
		delete this->poLocal;
		delete this->poBoneOrientation;
	}

	void GameObjectAnimSkeleton::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		//Trace::out("index: %d \n", index);

		Trans T;
		Scale S;
		Quat  Q;

		// Get the parent matrix... if root, set to identity
		GameObjectAnimSkeleton* pParentObject = (GameObjectAnimSkeleton*)this->GetParent();
		Mat4 matParent(Special::Identity);
		if (pParentObject)
		{
			matParent = *pParentObject->pWorld;

			// REMEMBER this is for Animation and hierachy,
			//          you need to handle models differently
			// Get the result bone array, from there make the matrix
			T.set(this->pBoneResult[index].T);
			S.set(this->pBoneResult[index].S);
			Q = this->pBoneResult[index].Q;
		}
		else
		{
			T.set(0, 0, 0);
			S.set(1, 1, 1);
			Q.set(Special::Identity);
		}

		// world matrix
		*this->poLocal = S * Q * T;
		*this->pWorld = *poLocal * matParent;
	}

	void GameObjectAnimSkeleton::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// This updates the orientation
		Anim::PoseSkeleton(this);

		// push to graphics object
		Mat4 mTmp = *this->poBoneOrientation;

		// push to graphics object
		pGraphicsObject->SetWorld(mTmp);
	}

	void GameObjectAnimSkeleton::SetQuat(float qx, float qy, float qz, float qw)
	{
		this->poQuat->set(qx, qy, qz, qw);
	}

	void GameObjectAnimSkeleton::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void GameObjectAnimSkeleton::SetTrans(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}

	void GameObjectAnimSkeleton::SetIndex(int i)
	{
		this->index = i;
	}

	void GameObjectAnimSkeleton::SetScale(Vec3& r)
	{
		*this->poScale = r;
	}

	void GameObjectAnimSkeleton::SetQuat(Quat& r)
	{
		*this->poQuat = r;
	}

	void GameObjectAnimSkeleton::SetTrans(Vec3& r)
	{
		*this->poTrans = r;
	}

	void GameObjectAnimSkeleton::SetBoneOrientation(const Mat4& tmp)
	{
		*this->poBoneOrientation = tmp;
	}

	Mat4 GameObjectAnimSkeleton::GetBoneOrientation(void) const
	{
		// return a copy...
		return Mat4(*this->poBoneOrientation);
	}
}