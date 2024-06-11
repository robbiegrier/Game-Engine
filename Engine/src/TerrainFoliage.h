#ifndef _TerrainFoliage
#define _TerrainFoliage

#include "DLinkedList.h"
#include "terrain.pb.h"
#include "FoliageType.h"
#include <d3d11.h>

namespace Azul
{
	class GOFoliage;
	class Terrain;

	// A component of the terrain mesh that encapsulates foliage management.
	class TerrainFoliage
	{
	public:
		// Big four.
		TerrainFoliage();
		TerrainFoliage(const TerrainFoliage&) = delete;
		TerrainFoliage& operator=(const TerrainFoliage&) = delete;
		~TerrainFoliage();

		// Draw all foliage.
		void Draw();

		// Bind to a terrain asset.
		void Bind(Terrain* pInTerrain);

		// Get the foliage graphics object list
		DLinkedList& GetGraphicsObjects() { return graphicsObjects; }

		// Serialize and deserialize the foliage data from the terrain file.
		void Serialize(terrain_proto& out) const;
		void Deserialize(const terrain_proto& in);

		// Add and remove new foliage types from this terrain instance.
		GOFoliage* AddFoliageType(FoliageType::Name inName);
		void RemoveFoliageType(FoliageType::Name inName);

	private:
		void AddGraphicsObject(GOFoliage* pInGo);

		DLinkedList graphicsObjects;
		ID3D11BlendState* pBlendStateFoliage;
		static constexpr float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr UINT sampleMask = 0xffffffff;
	};
}

#endif
