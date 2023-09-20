#include "CameraManager.h"
#include "DLinkedList.h"
#include "ListNode.h"

namespace Azul
{
	CameraManager* CameraManager::pInstance = nullptr;

	CameraManager::CameraManager(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	CameraManager::~CameraManager()
	{
	}

	void CameraManager::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new CameraManager(initialReserved, reserveGrow);
	}

	void CameraManager::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	Camera* CameraManager::Add(Camera::Name name, Camera* pItem)
	{
		CameraManager& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pItem);
		pItem->SetName(name);

		if (!self.pCurrentCamera)
		{
			self.pCurrentCamera = pItem;
		}

		return pItem;
	}

	Camera* CameraManager::Find(Camera::Name name)
	{
		CameraManager& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			Camera* pItem = (Camera*)pNode->Get();

			if (pItem->GetName() == name)
			{
				return pItem;
			}
		}

		return nullptr;
	}

	void CameraManager::SetCurrentCamera(Camera::Name name)
	{
		CameraManager& self = GetInstance();
		Camera* pItem = Find(name);
		assert(pItem);

		self.pCurrentCamera = pItem;
	}

	Camera* CameraManager::GetCurrentCamera()
	{
		CameraManager& self = GetInstance();
		assert(self.pCurrentCamera);
		return self.pCurrentCamera;
	}

	void CameraManager::Update(float deltaTime)
	{
		static_cast<void>(deltaTime);

		CameraManager& self = GetInstance();

		if (self.pCurrentCamera)
		{
			self.pCurrentCamera->UpdateCamera();
		}
	}

	void CameraManager::Dump()
	{
		CameraManager& self = GetInstance();
		Trace::out("%s\n", STRING_ME(CameraManager));
		self.ManagerBase::Dump();
	}

	DLink* CameraManager::CreateNode()
	{
		return new OwningListNode();
	}

	CameraManager& CameraManager::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}
}