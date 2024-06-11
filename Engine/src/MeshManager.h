#ifndef _ModelManager
#define _ModelManager

#include "ManagerBase.h"
#include "Mesh.h"

namespace Azul
{
	class TerrainMesh;

	class MeshManager : public ManagerBase
	{
	public:
		// Manager
		static void Create(int initialReserved = 0, int reserveGrow = 1);
		static void Destroy();
		static Mesh* Add(Mesh::Name name, Mesh* pModel);
		static Mesh* Find(Mesh::Name name);
		static void Dump();

		static TerrainMesh* FindTerrainMesh(const char* pTerrainName);

	protected:
		// Big four
		MeshManager(int initialReserved, int reserveGrow);
		MeshManager(const MeshManager&) = delete;
		MeshManager& operator =(const MeshManager&) = delete;
		~MeshManager();

		// Manager
		virtual DLink* CreateNode() override;

		// Singleton
		static MeshManager& GetInstance();
		static MeshManager* pInstance;
	};
}

#endif