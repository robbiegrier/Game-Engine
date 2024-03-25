#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "AnimatedObjectHierarchy.h"
#include "AnimationSystem.h"
#include "Clip.h"
#include "GameObjectBasic.h"

namespace Azul
{
	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject* pGraphicsObject, AnimatedObjectHierarchy* _pAnimatedObjectHierarchy, BoneTransform* pInBoneResult)
		: GameObjectAnim(pGraphicsObject),
		pBoneResult(pInBoneResult),
		poBoneWorld{ new Mat4[BONE_COUNT_MAX]() },
		pAnimationHierarchy{ _pAnimatedObjectHierarchy }
	{
		for (size_t i = 0; i < BONE_COUNT_MAX; i++)
		{
			poBoneWorld[i].set(Special::Identity);
		}
	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete[] this->poBoneWorld;
	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		GameObject::Update(currentTime);

		UpdateSkinGPU();
	}

	void GameObjectAnimSkin::SetIndex(int i)
	{
		index = i;
	}

	void GameObjectAnimSkin::UpdateSkinGPU()
	{
		CSAnimationMixer& computeShader = AnimationSystem::GetComputeShaderMixer();

		AnimController* pController = pAnimationHierarchy->pController;
		BlendInput bi = pController->GetClip()->GetBlendInput(pController->GetTimerController()->GetCurrTime());
		computeShader.Open(bi.pA, bi.pB, bi.tS);
		computeShader.Run();

		Skeleton* pSkeleton = pAnimationHierarchy->pSkeletonSource;
		CSAnimationWorld& computeShaderWorld = AnimationSystem::GetComputeShaderWorld();

		computeShaderWorld.Open(pSkeleton->GetHierarchyTable(), pSkeleton->GetHierarchyDepth(), pAnimationHierarchy->GetPivotGameObject()->GetWorld(), pGraphicsObject->GetModel()->poInvBindArray);
		computeShaderWorld.Run();
		computeShaderWorld.AwaitResults(poBoneWorld);
		computeShaderWorld.Close();
	}
}