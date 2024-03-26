#include "AnimationSystem.h"
#include "ListNode.h"
#include "DLinkedList.h"
#include "MathApp.h"
#include "BoneTransform.h"

namespace Azul
{
	AnimationSystem* AnimationSystem::pInstance = nullptr;

	AnimationSystem::AnimationSystem()
	{
		computeShaderWorld.AttachTo(computeShaderMixer);
	}

	AnimationSystem::~AnimationSystem()
	{
	}

	AnimationSystem& AnimationSystem::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void AnimationSystem::Create()
	{
		assert(!pInstance);
		pInstance = new AnimationSystem();
	}

	void AnimationSystem::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	void AnimationSystem::Dump()
	{
		Trace::out("%s\n", STRING_ME(AnimationSystem));
	}

	CSAnimationMixer& AnimationSystem::GetComputeShaderMixer()
	{
		return GetInstance().computeShaderMixer;
	}

	CSAnimationWorld& AnimationSystem::GetComputeShaderWorld()
	{
		return GetInstance().computeShaderWorld;
	}
}