#include "GameObjectManager.h"
#include "PCSNode.h"
#include "PCSTreeForwardIterator.h"
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

namespace Azul
{
	void GameObjectManager::Create()
	{
		GameObjectManager& self = GetInstance();

		GraphicsObject* pNull = new GONull();
		GameObject* pRoot = new GameObject(pNull);
		pRoot->SetRenderShell(false);
		pRoot->SetName("Objects");

		self.objects.Insert(pRoot, nullptr);

		Trace::out("Created GameObjectManager.\n");
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

		Trace::out("Destroyed GameObjectManager.\n");
	}

	void GameObjectManager::Update(AnimTime deltaTime)
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
		self.objects.Insert(pGameObject, pParent);

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
}