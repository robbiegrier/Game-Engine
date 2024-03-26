#include "Animator.h"
#include "TimerController.h"
#include "ClipManager.h"
#include "SkeletonManager.h"
#include "AnimationSystem.h"
#include "GOSkinLightTexture.h"

namespace Azul
{
	Animator::Animator()
		: pTimerController{ new TimerController(animationSpeed * ANIM_SPEED_CONST * AnimTime(AnimTime::Duration::FILM_24_FRAME), AnimTime(AnimTime::Duration::ZERO), AnimTime()) },
		pActiveClip(nullptr),
		pSkeleton(nullptr)
	{
	}

	Animator::~Animator()
	{
		delete pTimerController;
	}

	void Animator::Update(float deltaTime)
	{
		pTimerController->SetDeltaTime(deltaTime * animationSpeed * ANIM_SPEED_CONST * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		pTimerController->Update();

		CSAnimationMixer& computeShader = AnimationSystem::GetComputeShaderMixer();
		BlendInput bi = pActiveClip->GetBlendInput(pTimerController->GetCurrTime());
		computeShader.Open(bi.pA, bi.pB, bi.tS);
		computeShader.Run();

		CSAnimationWorld& computeShaderWorld = AnimationSystem::GetComputeShaderWorld();
		GOSkinLightTexture* pGoSkin = (GOSkinLightTexture*)GetGameObject()->GetGraphicsObject();
		computeShaderWorld.Open(pSkeleton->GetHierarchyTable(), pSkeleton->GetHierarchyDepth(), GetGameObject()->GetWorld(), pGoSkin->GetModel()->poInvBindArray);
		computeShaderWorld.Run();
		computeShaderWorld.AwaitResults(pGoSkin->pBoneWorld);
		computeShaderWorld.Close();
	}

	void Animator::SetActiveClip(Clip::Name inClipName)
	{
		pActiveClip = ClipManager::Find(inClipName);
		pTimerController->SetMaxTime(pActiveClip->GetTotalTime());
	}

	void Animator::SetSkeleton(Skeleton::Name inSkeletonName)
	{
		pSkeleton = SkeletonManager::Find(inSkeletonName);
	}

	void Animator::Serialize(json& out)
	{
		out["Type"] = "Animator";
		out["Active Clip"] = GetActiveClip()->GetName();
		out["Skeleton"] = GetSkeleton()->GetName();
		out["Animation Speed"] = animationSpeed;
	}

	void Animator::Deserialize(const json& in)
	{
		SetActiveClip(in["Active Clip"]);
		SetSkeleton(in["Skeleton"]);
		SetAnimationSpeed(in["Animation Speed"]);
	}
}