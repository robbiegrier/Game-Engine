#ifndef _TerrainSystem
#define _TerrainSystem

#include "ProtoAzul.h"

namespace Azul
{
	class Terrain;
	class GameObject;
	class CSTerrainBrush;

	// Singleton system for holding the current terrain. Can change the current terrain
	// by loading from a file or creating a new terrain asset.
	class TerrainSystem
	{
	public:
		// System events
		static void Create();
		static void Destroy();
		static void OnSceneSaved();
		static void OnSceneLoaded();

		// Get the current terrain.
		static Terrain* GetCurrentTerrain();

		// Set the current terrain
		static void SetCurrentTerrain(Terrain* pInTerrain);

		// Save and load from a terrain file
		static void SaveCurrentTerrainToDisk();
		static GameObject* LoadTerrainFromDisk(const char* pTerrainName);

		// Start a new terrain from an empty heightfield.
		static void NewTerrainFreshStart(const char* pInTerrainName, unsigned int inVertexResolution, float inWorldScale);

		// Start a new terrain from a heightmap image.
		static void NewTerrainHeightmap(const char* pInTerrainName, const char* pInHeightmapFile, float inWorldScale, float inHeightScale);

		// Change the current terrain to an existing asset.
		static void ChangeTerrain(const char* pInTerrainName);

	private:
		TerrainSystem();
		TerrainSystem(const TerrainSystem&) = delete;
		TerrainSystem& operator=(const TerrainSystem&) = delete;
		~TerrainSystem();

		// Singleton
		static TerrainSystem& GetInstance();

		// The current terrain asset.
		Terrain* pCurrentTerrain;
	};
}

#endif