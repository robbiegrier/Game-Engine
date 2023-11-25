#include "ClipManager.h"
#include "ListNode.h"
#include "DLinkedList.h"

namespace Azul
{
	ClipManager* ClipManager::pInstance = nullptr;

	ClipManager::ClipManager(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	ClipManager::~ClipManager()
	{
	}

	void ClipManager::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new ClipManager(initialReserved, reserveGrow);
	}

	void ClipManager::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	Clip* ClipManager::Add(Clip::Name name, Clip* pShader)
	{
		ClipManager& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pShader);
		pShader->SetName(name);
		return pShader;
	}

	Clip* ClipManager::Find(Clip::Name name)
	{
		ClipManager& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			Clip* pAsset = (Clip*)pNode->Get();

			if (pAsset->GetName() == name)
			{
				return pAsset;
			}
		}

		return nullptr;
	}

	DLink* ClipManager::CreateNode()
	{
		return new OwningListNode();
	}

	ClipManager& ClipManager::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void ClipManager::Dump()
	{
		ClipManager& self = GetInstance();
		Trace::out("%s\n", STRING_ME(ClipManager));
		self.ManagerBase::Dump();
	}
}