#include "GameObjectManager.h"
#include "PCSNode.h"
#include "PCSTreeForwardIterator.h"
#include "PCSTreeReverseIterator.h"
#include "PyramidMesh.h"
#include "CubeMesh.h"
#include "GOColorByVertex.h"
#include "GONull.h"
#include "GOWireframe.h"
#include "SOColorByVertex.h"
#include "GameObject.h"
#include "MeshManager.h"
#include "ShaderObjectManager.h"
#include "GOFlatTexture.h"
#include "GOLightTexture.h"
#include "TextureObjectManager.h"
#include "EditorGui.h"
#include "Player.h"
#include "GOFoliage.h"

namespace Azul
{
	extern GOFoliage* pGoFoliage;
	extern GOFoliage* pGoFoliage2;

	void GameObjectManager::Create()
	{
		GameObjectManager& self = GetInstance();

		GraphicsObject* pNull = new GONull();
		GameObject* pRoot = new GameObject(pNull);
		pRoot->SetRenderShell(false);
		pRoot->SetIsSelectable(false);
		pRoot->SetName("Objects");

		self.objects.Insert(pRoot, nullptr);

		//Trace::out("Created GameObjectManager.\n");
	}

	void GameObjectManager::Destroy()
	{
		GameObjectManager& self = GetInstance();

		PCSTreeForwardIterator it(self.objects.GetRoot());

		for (it.First(); !it.IsDone();)
		{
			PCSNode* pNode = it.Current();
			it.Next();
			delete pNode;
		}

		//Trace::out("Destroyed GameObjectManager.\n");
	}

	void GameObjectManager::Start()
	{
		GameObjectManager& self = GetInstance();

		for (PCSTreeForwardIterator it(self.objects.GetRoot()); !it.IsDone(); it.Next())
		{
			GameObject* pObject = (GameObject*)it.Current();
			pObject->Start();
		}
	}

	void GameObjectManager::Tick(float deltaTime)
	{
		GameObjectManager& self = GetInstance();

		for (PCSTreeForwardIterator it(self.objects.GetRoot()); !it.IsDone(); it.Next())
		{
			GameObject* pObject = (GameObject*)it.Current();
			pObject->Tick(deltaTime);
		}
	}

	void GameObjectManager::Update(float deltaTime)
	{
		GameObjectManager& self = GetInstance();
		PCSTreeForwardIterator it(self.objects.GetRoot());

		for (it.First(); !it.IsDone(); it.Next())
		{
			GameObject* pObject = (GameObject*)it.Current();
			pObject->Update(deltaTime);
		}
	}

	void GameObjectManager::Draw()
	{
		GameObjectManager& self = GetInstance();
		PCSTreeForwardIterator it(self.objects.GetRoot());

		for (it.First(); !it.IsDone(); it.Next())
		{
			GameObject* pObject = (GameObject*)it.Current();
			pObject->Draw();
		}
	}

	GameObject* GameObjectManager::SpawnObject(const char* const pName, Mesh::Name model, ShaderObject::Name shader, const Vec3& location, GameObject* pParent)
	{
		return SpawnObject(pName, MeshManager::Find(model), ShaderObjectManager::Find(shader), location, pParent);
	}

	GameObject* GameObjectManager::SpawnObject(const char* const pName, Mesh::Name model, TextureObject::Name texture, const Vec3& location, GameObject* pParent)
	{
		return SpawnObject(pName, MeshManager::Find(model), ShaderObjectManager::Find(ShaderObject::Name::Default),
			TextureObjectManager::Find(texture), location, pParent);
	}

	GameObject* GameObjectManager::ReparentObject(GameObject* pObject, GameObject* pParent, GameObject* pPrev, bool preserveWorld)
	{
		if (pObject->IsParentOf(pParent) || pObject == pParent || pObject == pPrev || pPrev == pParent)
		{
			return nullptr;
		}

		Quat newRelativeRotation = Quat(Rot(Rot(pObject->GetWorldRotation()) * Rot(pParent->GetWorldRotation().getInv()))).getNorm();
		Vec3 newRelativeLocation = Vec3(Vec4(pObject->GetWorldLocation(), 1.f) * pParent->GetWorld().getInv());
		Vec3 newRelativeScale = Vec3(Vec4(pObject->GetWorldScale(), 0.f) * Scale(pParent->GetWorldScale()).getInv());

		GetInstance().objects.RelocateSubtree(pObject, pParent, pPrev);

		if (preserveWorld)
		{
			pObject->SetRelativeLocation(newRelativeLocation);
			pObject->SetRelativeRotation(newRelativeRotation);
			pObject->SetRelativeScale(newRelativeScale);
		}

		return pObject;
	}

	GameObject* GameObjectManager::FindObject(const char* const pName)
	{
		GameObjectManager& self = GetInstance();

		char* pCompare = new char[PCSNode::NAME_SIZE];
		GameObject* pOutput = nullptr;

		PCSTreeForwardIterator it(self.objects.GetRoot());
		for (it.First(); !it.IsDone(); it.Next())
		{
			GameObject* pObject = (GameObject*)it.Current();
			pObject->GetName(pCompare, PCSNode::NAME_SIZE);

			if (strcmp(pCompare, pName) == 0)
			{
				pOutput = pObject;
				break;
			}
		}

		delete pCompare;

		return pOutput;
	}

	void GameObjectManager::DestroyObject(const char* const pName)
	{
		GameObjectManager& self = GetInstance();
		GameObject* pNode = FindObject(pName);
		self.objects.Remove(pNode);
		delete pNode;
	}

	GameObject* GameObjectManager::DespawnObject(const char* const pName)
	{
		GameObject* pNode = FindObject(pName);
		return DespawnObject(pNode);
	}

	GameObject* GameObjectManager::DespawnObject(GameObject* pObject)
	{
		GameObjectManager& self = GetInstance();
		self.objects.DetachSubtree(pObject);
		return pObject;
	}

	GameObject* GameObjectManager::SpawnObject(const char* const pName, Mesh* pModel, ShaderObject* pShader, const Vec3& location, GameObject* pParent)
	{
		GameObjectManager& self = GetInstance();

		if (!pParent)
		{
			pParent = (GameObject*)self.objects.GetRoot();
		}

		GraphicsObject* pGraphicsObject = new GOColorByVertex(pModel, pShader);
		GameObject* pGameObject = new GameObject(pGraphicsObject);
		pGameObject->SetName(pName);
		pGameObject->SetRelativeLocation(location);
		self.objects.Insert(pGameObject, pParent);

		return pGameObject;
	}

	GameObject* GameObjectManager::SpawnObject(const char* const pName, Mesh* pModel, ShaderObject* pShader, TextureObject* pTexture, const Vec3& location, GameObject* pParent)
	{
		GameObjectManager& self = GetInstance();

		if (!pParent)
		{
			pParent = (GameObject*)self.objects.GetRoot();
		}

		GraphicsObject* pGraphicsObject = new GOLightTexture(pModel, pShader, pTexture);
		GameObject* pGameObject = new GameObject(pGraphicsObject);
		pGameObject->SetName(pName);
		pGameObject->SetRelativeLocation(location);
		self.objects.Insert(pGameObject, pParent);

		return pGameObject;
	}

	GameObject* GameObjectManager::SpawnObject(const char* const pName, GraphicsObject* pGraphicsObject, const Vec3& location, GameObject* pParent)
	{
		GameObjectManager& self = GetInstance();

		if (!pParent)
		{
			pParent = (GameObject*)self.objects.GetRoot();
		}

		GameObject* pGameObject = new GameObject(pGraphicsObject);
		pGameObject->SetName(pName);
		pGameObject->SetRelativeLocation(location);
		self.objects.Insert(pGameObject, pParent);

		return pGameObject;
	}

	GameObject* GameObjectManager::SpawnObject(const char* const pName, GameObject* pGameObject, const Vec3& location, GameObject* pParent)
	{
		GameObjectManager& self = GetInstance();

		if (!pParent)
		{
			pParent = (GameObject*)self.objects.GetRoot();
		}

		pGameObject->SetName(pName);
		pGameObject->SetRelativeLocation(location);
		self.objects.InsertBack(pGameObject, pParent);

		return pGameObject;
	}

	void GameObjectManager::Dump()
	{
		GameObjectManager& self = GetInstance();
		self.objects.Print();
	}

	GameObjectManager& GameObjectManager::GetInstance()
	{
		static GameObjectManager instance;
		return instance;
	}

	void GameObjectManager::ClearObjects()
	{
		GameObjectManager& self = GetInstance();

		PCSTreeForwardIterator it(self.objects.GetRoot());

		for (it.First(); !it.IsDone();)
		{
			PCSNode* pNode = it.Current();
			EditorGui::OnGameObjectRemoved((GameObject*)pNode);
			it.Next();
			delete pNode;
		}

		self.objects = PCSTree();

		GraphicsObject* pNull = new GONull();
		GameObject* pRoot = new GameObject(pNull);
		pRoot->SetRenderShell(false);
		pRoot->SetIsSelectable(false);
		pRoot->SetName("Objects");

		self.objects.Insert(pRoot, nullptr);
	}
}