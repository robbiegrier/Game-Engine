#include "FontLibrary.h"
#include "ListNode.h"
#include "DLinkedList.h"

namespace Azul
{
	FontLibrary* FontLibrary::pInstance = nullptr;

	FontLibrary::FontLibrary(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	FontLibrary::~FontLibrary()
	{
	}

	void FontLibrary::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new FontLibrary(initialReserved, reserveGrow);
	}

	void FontLibrary::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	Font* FontLibrary::Add(Font::Name name, Font* pAsset)
	{
		FontLibrary& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pAsset);
		pAsset->SetName(name);
		return pAsset;
	}

	Font* FontLibrary::Find(Font::Name name)
	{
		FontLibrary& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			Font* pAsset = (Font*)pNode->Get();

			if (pAsset->GetName() == name)
			{
				return pAsset;
			}
		}

		return nullptr;
	}

	DLink* FontLibrary::CreateNode()
	{
		return new OwningListNode();
	}

	FontLibrary& FontLibrary::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void FontLibrary::Dump()
	{
		FontLibrary& self = GetInstance();
		Trace::out("%s\n", STRING_ME(FontLibrary));
		self.ManagerBase::Dump();
	}
}