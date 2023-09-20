#include "ShaderObjectManager.h"
#include "ListNode.h"
#include "DLinkedList.h"

namespace Azul
{
	ShaderObjectManager* ShaderObjectManager::pInstance = nullptr;

	ShaderObjectManager::ShaderObjectManager(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	ShaderObjectManager::~ShaderObjectManager()
	{
	}

	void ShaderObjectManager::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new ShaderObjectManager(initialReserved, reserveGrow);
	}

	void ShaderObjectManager::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	ShaderObject* ShaderObjectManager::Add(ShaderObject::Name name, ShaderObject* pShader)
	{
		ShaderObjectManager& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pShader);
		pShader->SetName(name);
		return pShader;
	}

	ShaderObject* ShaderObjectManager::Find(ShaderObject::Name name)
	{
		ShaderObjectManager& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			ShaderObject* pAsset = (ShaderObject*)pNode->Get();

			if (pAsset->GetName() == name)
			{
				return pAsset;
			}
		}

		return nullptr;
	}

	DLink* ShaderObjectManager::CreateNode()
	{
		return new OwningListNode();
	}

	ShaderObjectManager& ShaderObjectManager::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void ShaderObjectManager::Dump()
	{
		ShaderObjectManager& self = GetInstance();
		Trace::out("%s\n", STRING_ME(ShaderObjectManager));
		self.ManagerBase::Dump();
	}
}