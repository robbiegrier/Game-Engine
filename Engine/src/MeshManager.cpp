#include "MeshManager.h"
#include "DLinkedList.h"
#include "ListNode.h"
#include "TerrainMesh.h"

namespace Azul
{
	MeshManager* MeshManager::pInstance = nullptr;

	MeshManager::MeshManager(int initialReserved, int reserveGrow)
		: ManagerBase(new DLinkedList(), new DLinkedList(), initialReserved, reserveGrow)
	{
	}

	MeshManager::~MeshManager()
	{
	}

	void MeshManager::Create(int initialReserved, int reserveGrow)
	{
		assert(!pInstance);
		pInstance = new MeshManager(initialReserved, reserveGrow);
	}

	void MeshManager::Destroy()
	{
		assert(pInstance);
		delete pInstance;
		pInstance = nullptr;
	}

	Mesh* MeshManager::Add(Mesh::Name name, Mesh* pModel)
	{
		MeshManager& self = GetInstance();
		ListNode* pNode = (ListNode*)self.Push();
		pNode->Set(pModel);
		pModel->SetName(name);
		return pModel;
	}

	Mesh* MeshManager::Find(Mesh::Name name)
	{
		MeshManager& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			Mesh* pAsset = (Mesh*)pNode->Get();

			if (pAsset->GetName() == name)
			{
				return pAsset;
			}
		}

		return nullptr;
	}

	DLink* MeshManager::CreateNode()
	{
		return new OwningListNode();
	}

	MeshManager& MeshManager::GetInstance()
	{
		assert(pInstance);
		return *pInstance;
	}

	void MeshManager::Dump()
	{
		MeshManager& self = GetInstance();
		Trace::out("%s\n", STRING_ME(MeshManager));
		self.ManagerBase::Dump();
	}

	TerrainMesh* MeshManager::FindTerrainMesh(const char* pTerrainName)
	{
		MeshManager& self = GetInstance();

		Iterator& it = *self.GetActiveIterator();

		for (it.First(); !it.IsDone(); it.Next())
		{
			ListNode* pNode = (ListNode*)it.Curr();
			Mesh* pAsset = (Mesh*)pNode->Get();

			if (pAsset->GetName() == Mesh::Name::Terrain)
			{
				TerrainMesh* pAssetTerrain = (TerrainMesh*)pAsset;
				if (strcmp(pAssetTerrain->GetTerrainMeshName(), pTerrainName) == 0)
				{
					return pAssetTerrain;
				}
			}
		}

		return nullptr;
	}
}