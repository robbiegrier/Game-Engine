#include "SkeletonManager.h"
#include "ListNode.h"
#include "DLinkedList.h"

namespace Azul
{
	SkeletonManager* SkeletonManager::pInstance = nullptr;

	SkeletonManager::SkeletonManager(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	SkeletonManager::~SkeletonManager()
	{
	}

	void SkeletonManager::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new SkeletonManager(initialReserved, reserveGrow);
	}

	void SkeletonManager::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	Skeleton* SkeletonManager::Add(Skeleton::Name name, Skeleton* pItem)
	{
		SkeletonManager& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pItem);
		pItem->SetName(name);
		return pItem;
	}

	Skeleton* SkeletonManager::Find(Skeleton::Name name)
	{
		SkeletonManager& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			Skeleton* pAsset = (Skeleton*)pNode->Get();

			if (pAsset->GetName() == name)
			{
				return pAsset;
			}
		}

		return nullptr;
	}

	DLink* SkeletonManager::CreateNode()
	{
		return new OwningListNode();
	}

	SkeletonManager& SkeletonManager::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void SkeletonManager::Dump()
	{
		SkeletonManager& self = GetInstance();
		Trace::out("%s\n", STRING_ME(SkeletonManager));
		self.ManagerBase::Dump();
	}
}