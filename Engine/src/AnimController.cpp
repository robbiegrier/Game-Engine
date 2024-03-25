#include "AnimController.h"
#include "AnimatedObjectHierarchy.h"
#include "GameObjectBasic.h"
#include "ClipManager.h"
#include "SkeletonManager.h"
#include "Skeleton.h"
#include "MeshManager.h"
#include "TextureObjectManager.h"

namespace Azul
{
	AnimController::AnimController(AnimTime delta, Clip::Name pInClipName, Skeleton::Name pInSkeletonName, Mesh::Name inMeshName, TextureObject::Name inTextureName, AnimMode inMode)
		: pClip(nullptr), pBoneResult(nullptr), pAnimatedObjectHierarchy(nullptr)
	{
		pClip = ClipManager::Find(pInClipName);
		pTimerControl = new TimerController(delta, AnimTime(AnimTime::Duration::ZERO), pClip->GetTotalTime());
		Skeleton* pSkeleton = SkeletonManager::Find(pInSkeletonName);
		pBoneResult = new BoneTransform[BONE_COUNT_MAX]();
		pAnimatedObjectHierarchy = new AnimatedObjectHierarchy(pBoneResult, pSkeleton, MeshManager::Find(inMeshName), TextureObjectManager::Find(inTextureName), this, inMode);
	}

	AnimController::~AnimController()
	{
		delete pTimerControl;
		delete[] pBoneResult;
		delete pAnimatedObjectHierarchy;
	}

	void AnimController::Update()
	{
		pTimerControl->Update();
	}

	GameObjectBasic* AnimController::GetObjectPivot() const
	{
		return pAnimatedObjectHierarchy->GetPivotGameObject();
	}

	void AnimController::SetClip(Clip::Name inClipName)
	{
		pClip = ClipManager::Find(inClipName);
		pTimerControl->SetMaxTime(pClip->GetTotalTime());
	}

	Clip* AnimController::GetClip() const
	{
		return pClip;
	}

	void AnimController::Wash()
	{
	}

	bool AnimController::Compare(DLink* pTargetNode)
	{
		return this == (AnimController*)pTargetNode;
	}
}