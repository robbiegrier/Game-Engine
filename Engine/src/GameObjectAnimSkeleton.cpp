#include "Game.h"
#include "GameObjectAnimSkeleton.h"
#include "MathEngine.h"
#include "GraphicsObject.h"
#include "Mesh.h"

namespace Azul
{
	GameObjectAnimSkeleton::GameObjectAnimSkeleton(GraphicsObject* pGraphicsObject, BoneTransform* _pBoneResult)
		: GameObjectAnim(pGraphicsObject)
	{
		poBoneOrientation = new Mat4(Special::Identity);
		pBoneResult = _pBoneResult;
	}

	GameObjectAnimSkeleton::~GameObjectAnimSkeleton()
	{
		delete poBoneOrientation;
	}

	void GameObjectAnimSkeleton::Update(AnimTime currentTime)
	{
		SetRelativeLocation(pBoneResult[index].T);
		SetRelativeScale(pBoneResult[index].S);
		SetRelativeRotation(Rot(pBoneResult[index].Q));

		GameObject::Update(currentTime);
	}

	void GameObjectAnimSkeleton::SetIndex(int i)
	{
		index = i;
	}

	void GameObjectAnimSkeleton::SetBoneOrientation(const Mat4& tmp)
	{
		*poBoneOrientation = tmp;
	}

	Mat4 GameObjectAnimSkeleton::GetBoneOrientation(void) const
	{
		return Mat4(*poBoneOrientation);
	}
}