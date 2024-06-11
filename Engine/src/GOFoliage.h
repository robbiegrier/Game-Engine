#ifndef _GOFoliage
#define _GOFoliage

#include "GraphicsObject.h"
#include "TextureObject.h"
#include "DLink.h"
#include <vector>
#include "QTNode.h"
#include "FoliageType.h"

namespace Azul
{
	class Terrain;
	class TerrainMesh;

	// Graphics object for foliage. This is a special type of graphics object because it uses
	// hardware instancing and is owned by the terrain foliage system.
	class GOFoliage : public GraphicsObject, public DLink
	{
	public:
		// Big four
		explicit GOFoliage(FoliageType::Name foliageTypeName);
		explicit GOFoliage(FoliageType* pInFoliageType);
		GOFoliage() = delete;
		GOFoliage(const GOFoliage&) = delete;
		GOFoliage& operator = (const GOFoliage&) = delete;
		virtual ~GOFoliage();

		// Render contract
		virtual void ActivateGraphics() override;
		virtual void Draw() override;
		virtual GraphicsObject* Clone() override;
		virtual TextureObject* GetTexture() override;

		// bind to a terrain asset
		void BindToTerrain(Terrain* pTerrian);

		// prepare the instances to be rendered this frame
		void PrepareVisibleInstanceBuffer(const std::vector<QTNode*>& pVisibleNodes);

		// Add and remove instances
		void AddInstance(const FoliageInstance& instance);
		void RemoveInstanceNear(const Vec3& removeFocusLocation, const Vec3& pinLocation, const float radius);

		// DLink methods
		virtual bool Compare(DLink* pTargetNode) override { return this == pTargetNode; }
		virtual void Wash() override {}

		// Accessors
		uint GetTotalInstances() const { return totalInstances; }
		uint GetNumVectors() const { return numVectors; }
		uint GetVisibleInstances() const { return count; }
		void SetSpawnScale(float inScale) { pFoliageType->baseScale = inScale; }
		float GetSpawnScale() const { return pFoliageType->baseScale; }
		float* GetSpawnScalePtr() { return &pFoliageType->baseScale; }
		FoliageType* GetFoliageType() const { return pFoliageType; }
		std::vector<FoliageInstance>* GetInstanceStorage() const { return vectors; }

	private:
		std::vector<FoliageInstance>* vectors = nullptr;
		FoliageInstance* pInstanceData = nullptr;
		static constexpr unsigned int MAX_INSTANCES_PER_DRAW = 100000u;
		unsigned int count = 0u;
		ID3D11Buffer* pVertexBufferInstanceData = nullptr;
		Terrain* pTerrain = nullptr;
		TerrainMesh* pTerrainMesh = nullptr;
		FoliageType* pFoliageType = nullptr;
		uint totalInstances = 0u;
		uint numVectors = 0u;
		TextureObject* pTex;

	public:
		static constexpr unsigned int LEVELS_UP = 3u;
		static constexpr unsigned int strideInst = sizeof(FoliageInstance);
		static constexpr unsigned int offset = 0u;
	};
}

#endif