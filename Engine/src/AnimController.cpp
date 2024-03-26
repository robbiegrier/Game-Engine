#include "AnimController.h"
#include "ClipManager.h"
#include "SkeletonManager.h"
#include "Skeleton.h"
#include "MeshManager.h"
#include "TextureObjectManager.h"
#include "ShaderObjectManager.h"
#include "MeshManager.h"
#include "GameObjectAnimSkin.h"
#include "Skeleton.h"
#include "GOSkinLightTexture.h"
#include "GameObjectManager.h"

namespace Azul
{
	AnimController::AnimController(AnimTime delta, Clip::Name pInClipName, Skeleton::Name pInSkeletonName, Mesh::Name inMeshName, TextureObject::Name inTextureName)
		: pClip(nullptr)
	{
		pClip = ClipManager::Find(pInClipName);
		pTimerControl = new TimerController(delta, AnimTime(AnimTime::Duration::ZERO), pClip->GetTotalTime());
		pSkeleton = SkeletonManager::Find(pInSkeletonName);

		Vec3 spawnPos(0, 0, 0);

		GOSkinLightTexture* pGraphicsSkin = new GOSkinLightTexture(MeshManager::Find(inMeshName), ShaderObjectManager::Find(ShaderObject::Name::SkinLightTexture), TextureObjectManager::Find(inTextureName));
		GameObjectAnimSkin* pGameObjectSkin = new GameObjectAnimSkin(pGraphicsSkin, this);
		pGraphicsSkin->pBoneWorld = pGameObjectSkin->poBoneWorld;
		pGameObject = GameObjectManager::SpawnObject("Skinned Game Object", pGameObjectSkin, spawnPos);
	}

	AnimController::~AnimController()
	{
		delete pTimerControl;
	}

	void AnimController::Update()
	{
		pTimerControl->Update();
	}

	GameObject* AnimController::GetGameObject() const
	{
		return pGameObject;
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