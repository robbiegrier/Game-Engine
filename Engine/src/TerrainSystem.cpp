#include "TerrainSystem.h"
#include "Terrain.h"
#include "TerrainMesh.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "PCSTreeForwardIterator.h"
#include "EditorGui.h"
#include "GOTerrain.h"
#include "ShaderObjectManager.h"
#include "TextureObjectManager.h"
#include "MeshManager.h"
#include "EditActionDelete.h"
#include "CSTerrainBrush.h"
#include "TerrainBrushRaise.h"
#include "TerrainBrushFlatten.h"
#include "TerrainBrushConvolution.h"
#include "TerrainBrushNoise.h"
#include "TerrainFoliage.h"

namespace Azul
{
	TerrainSystem::TerrainSystem()
		: pCurrentTerrain(nullptr)
	{
	}

	TerrainSystem::~TerrainSystem()
	{
	}

	void TerrainSystem::Create()
	{
	}

	void TerrainSystem::Destroy()
	{
	}

	Terrain* TerrainSystem::GetCurrentTerrain()
	{
		return GetInstance().pCurrentTerrain;
	}

	void TerrainSystem::SetCurrentTerrain(Terrain* pInTerrain)
	{
		TerrainSystem& self = GetInstance();
		self.pCurrentTerrain = pInTerrain;

		EditorGui::GetTerrainEditor().OnTerrainChanged(self.pCurrentTerrain);
	}

	void TerrainSystem::OnSceneSaved()
	{
		SaveCurrentTerrainToDisk();
	}

	void TerrainSystem::OnSceneLoaded()
	{
		for (PCSTreeForwardIterator it(GameObjectManager::GetAllObjects().GetRoot()); !it.IsDone(); it.Next())
		{
			GameObject* pCurr = (GameObject*)it.Current();

			if (pCurr->GetTypeName() == GameObject::Name::Terrain)
			{
				SetCurrentTerrain((Terrain*)pCurr);
				break;
			}
		}
	}

	void TerrainSystem::SaveCurrentTerrainToDisk()
	{
		Log("Saving Terrain!");

		TerrainSystem& self = GetInstance();

		if (self.pCurrentTerrain)
		{
			TerrainMesh* pMesh = self.pCurrentTerrain->GetTerrainMesh();

			terrain_proto proto;
			pMesh->Serialize(proto);

			std::string filename = std::string("..\\Terrains\\") + std::string(pMesh->GetTerrainMeshName()) + std::string(".terrain.proto.azul");
			std::remove(filename.c_str());
			ProtoAzul::WriteProtoFile(proto, filename.c_str());
		}
	}

	GameObject* TerrainSystem::LoadTerrainFromDisk(const char* pTerrainName)
	{
		TerrainSystem& self = GetInstance();

		Log("Loading Terrain!");

		TerrainMesh* pTerrainMesh = MeshManager::FindTerrainMesh(pTerrainName);

		if (!pTerrainMesh)
		{
			terrain_proto proto;
			std::string filename = std::string("..\\Terrains\\") + std::string(pTerrainName) + std::string(".terrain.proto.azul");
			ProtoAzul::ReadProtoFile(proto, filename.c_str());

			pTerrainMesh = new TerrainMesh();
			pTerrainMesh->Deserialize(proto);
			MeshManager::Add(Mesh::Name::Terrain, pTerrainMesh);

			GraphicsObject* pGoTerrain = new GOTerrain(pTerrainMesh, ShaderObjectManager::Find(ShaderObject::Name::Terrain));
			Terrain* pTerrain = new Terrain(pGoTerrain);

			self.pCurrentTerrain = pTerrain;
			pTerrain->GetTerrainMesh()->GetFoliage()->Deserialize(proto);

			return pTerrain;
		}
		else
		{
			GraphicsObject* pGoTerrain = new GOTerrain(pTerrainMesh, ShaderObjectManager::Find(ShaderObject::Name::Terrain));
			Terrain* pTerrain = new Terrain(pGoTerrain);

			self.pCurrentTerrain = pTerrain;

			return pTerrain;
		}
	}

	void TerrainSystem::NewTerrainFreshStart(const char* pInTerrainName, unsigned int inVertexResolution, float inWorldScale)
	{
		TerrainSystem& self = GetInstance();

		SaveCurrentTerrainToDisk();

		Log("Making New Fresh Terrain: %s!", pInTerrainName);

		EditActionDelete* pAction = new EditActionDelete();
		pAction->SetSingleObject(self.pCurrentTerrain);
		pAction->Execute();
		delete pAction;

		TerrainMesh* pTerrainMesh = new TerrainMesh(pInTerrainName, inWorldScale, inVertexResolution);
		MeshManager::Add(Mesh::Name::Terrain, pTerrainMesh);

		GraphicsObject* pGoTerrain = new GOTerrain(pTerrainMesh, ShaderObjectManager::Find(ShaderObject::Name::Terrain));
		Terrain* pTerrainGameObject = new Terrain(pGoTerrain);

		GameObjectManager::SpawnObject(pInTerrainName, pTerrainGameObject, Vec3());

		SetCurrentTerrain(pTerrainGameObject);
	}

	void TerrainSystem::NewTerrainHeightmap(const char* pInTerrainName, const char* pInHeightmapFile, float inWorldScale, float inHeightScale)
	{
		TerrainSystem& self = GetInstance();

		SaveCurrentTerrainToDisk();

		Log("Making New Heightmap Terrain: %s (%s)!", pInTerrainName, pInHeightmapFile);

		EditActionDelete* pAction = new EditActionDelete();
		pAction->SetSingleObject(self.pCurrentTerrain);
		pAction->Execute();
		delete pAction;

		TerrainMesh* pTerrainMesh = new TerrainMesh(pInTerrainName, inWorldScale, inHeightScale, pInHeightmapFile);
		MeshManager::Add(Mesh::Name::Terrain, pTerrainMesh);

		GraphicsObject* pGoTerrain = new GOTerrain(pTerrainMesh, ShaderObjectManager::Find(ShaderObject::Name::Terrain));
		Terrain* pTerrainGameObject = new Terrain(pGoTerrain);

		GameObjectManager::SpawnObject(pInTerrainName, pTerrainGameObject, Vec3());

		SetCurrentTerrain(pTerrainGameObject);
	}

	void TerrainSystem::ChangeTerrain(const char* pInTerrainName)
	{
		TerrainSystem& self = GetInstance();

		Log("Changing Terrain to %s!", pInTerrainName);

		EditActionDelete* pAction = new EditActionDelete();
		pAction->SetSingleObject(self.pCurrentTerrain);
		pAction->Execute();
		delete pAction;

		Terrain* pTerrainGameObject = (Terrain*)LoadTerrainFromDisk(pInTerrainName);
		GameObjectManager::SpawnObject(pInTerrainName, pTerrainGameObject, Vec3());

		SetCurrentTerrain(pTerrainGameObject);
	}

	TerrainSystem& TerrainSystem::GetInstance()
	{
		static TerrainSystem instance;
		return instance;
	}
}