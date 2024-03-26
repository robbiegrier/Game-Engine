#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "AnimationSystem.h"
#include "Clip.h"
#include "AnimController.h"

namespace Azul
{
	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject* pGraphicsObject, AnimController* pInController)
		: GameObject(pGraphicsObject), poBoneWorld{ new Mat4[BONE_COUNT_MAX]() }, pController{ pInController }
	{
		for (size_t i = 0; i < BONE_COUNT_MAX; i++)
		{
			poBoneWorld[i].set(Special::Identity);
		}
	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete[] poBoneWorld;
	}

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		GameObject::Update(currentTime);

		UpdateSkinGPU();
	}

	void GameObjectAnimSkin::UpdateSkinGPU()
	{
		CSAnimationMixer& computeShader = AnimationSystem::GetComputeShaderMixer();
		BlendInput bi = pController->GetClip()->GetBlendInput(pController->GetTimerController()->GetCurrTime());
		computeShader.Open(bi.pA, bi.pB, bi.tS);
		computeShader.Run();

		CSAnimationWorld& computeShaderWorld = AnimationSystem::GetComputeShaderWorld();
		Skeleton* pSkeleton = pController->GetSkeleton();
		computeShaderWorld.Open(pSkeleton->GetHierarchyTable(), pSkeleton->GetHierarchyDepth(), GetWorld(), pGraphicsObject->GetModel()->poInvBindArray);
		computeShaderWorld.Run();
		computeShaderWorld.AwaitResults(poBoneWorld);
		computeShaderWorld.Close();
	}
}