#include "TextureObjectManager.h"
#include "ListNode.h"
#include "DLinkedList.h"

namespace Azul
{
	TextureObjectManager* TextureObjectManager::pInstance = nullptr;

	TextureObjectManager::TextureObjectManager(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	TextureObjectManager::~TextureObjectManager()
	{
	}

	void TextureObjectManager::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new TextureObjectManager(initialReserved, reserveGrow);
	}

	void TextureObjectManager::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	TextureObject* TextureObjectManager::Add(TextureObject::Name name, TextureObject* pItem)
	{
		TextureObjectManager& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pItem);
		pItem->SetName(name);
		return pItem;
	}

	TextureObject* TextureObjectManager::Find(TextureObject::Name name)
	{
		TextureObjectManager& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			TextureObject* pAsset = (TextureObject*)pNode->Get();

			if (pAsset->GetName() == name)
			{
				return pAsset;
			}
		}

		return nullptr;
	}

	DLink* TextureObjectManager::CreateNode()
	{
		return new OwningListNode();
	}

	TextureObjectManager& TextureObjectManager::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void TextureObjectManager::Dump()
	{
		TextureObjectManager& self = GetInstance();
		Trace::out("%s\n", STRING_ME(ShaderObjectManager));
		self.ManagerBase::Dump();
	}
}