#include "AnimationSystem.h"
#include "ListNode.h"
#include "DLinkedList.h"

namespace Azul
{
	AnimationSystem* AnimationSystem::pInstance = nullptr;

	AnimationSystem::AnimationSystem(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
		computeShaderWorld.AttachTo(computeShaderMixer);
	}

	AnimationSystem::~AnimationSystem()
	{
	}

	DLink* AnimationSystem::CreateNode()
	{
		return new OwningListNode();
	}

	AnimationSystem& AnimationSystem::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void AnimationSystem::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new AnimationSystem(initialReserved, reserveGrow);
	}

	void AnimationSystem::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	AnimController* AnimationSystem::Add(AnimController* pItem)
	{
		AnimationSystem& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pItem);
		return pItem;
	}

	void AnimationSystem::Dump()
	{
		AnimationSystem& self = GetInstance();
		Trace::out("%s\n", STRING_ME(AnimationSystem));
		self.ManagerBase::Dump();
	}

	void AnimationSystem::Update()
	{
		AnimationSystem& self = GetInstance();

		for (Iterator& it = *self.GetActiveIterator(); !it.IsDone(); it.Next())
		{
			AnimController* pCurr = (AnimController*)((ListNode*)it.Curr())->Get();
			pCurr->Update();
		}
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