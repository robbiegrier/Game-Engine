#include "FoliageLibrary.h"
#include "ListNode.h"
#include "DLinkedList.h"

namespace Azul
{
	FoliageLibrary* FoliageLibrary::pInstance = nullptr;

	FoliageLibrary::FoliageLibrary(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	FoliageLibrary::~FoliageLibrary()
	{
	}

	void FoliageLibrary::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new FoliageLibrary(initialReserved, reserveGrow);
	}

	void FoliageLibrary::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	FoliageType* FoliageLibrary::Add(FoliageType::Name name, FoliageType* pAsset)
	{
		FoliageLibrary& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pAsset);
		pAsset->SetName(name);
		return pAsset;
	}

	FoliageType* FoliageLibrary::Find(FoliageType::Name name)
	{
		FoliageLibrary& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			FoliageType* pAsset = (FoliageType*)pNode->Get();

			if (pAsset->GetName() == name)
			{
				return pAsset;
			}
		}

		return nullptr;
	}

	DLink* FoliageLibrary::CreateNode()
	{
		return new OwningListNode();
	}

	FoliageLibrary& FoliageLibrary::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void FoliageLibrary::Dump()
	{
		FoliageLibrary& self = GetInstance();
		Trace::out("%s\n", STRING_ME(FontLibrary));
		self.ManagerBase::Dump();
	}
}