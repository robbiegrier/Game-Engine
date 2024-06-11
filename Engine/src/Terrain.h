#ifndef _Terrain
#define _Terrain

#include "GameObject.h"
#include "TerrainUtils.h"

namespace Azul
{
	class TerrainMesh;
	class QuadTree;
	class QTNode;

	// The Terrain Game Object class that holds the terrain mesh and the quad tree. Provides game queries for the terrain.
	class Terrain : public GameObject
	{
	public:
		// Big four
		Terrain(GraphicsObject* pInGraphicsObject);
		Terrain() = delete;
		Terrain(const Terrain&) = delete;
		Terrain& operator=(Terrain&) = delete;
		virtual ~Terrain();

		// Core loop methods
		virtual void Update(float deltaTime) override;
		virtual void Draw() override;

		// Get the quad tree node which contains this location
		QTNode* GetTreeNodeAt2D(const Vec3& location) const;

		// Get the terrain cell in 2d space located nearest to this location
		bool GetCellAt2D(const Vec3& location, TerrainCoordinate& outCoord) const;

		// Get the terrain cell in 2d space located nearest to this location (rounding up)
		bool GetCeilingCellAt2D(const Vec3& location, TerrainCoordinate& outCoord) const;

		// Get the world location of the terrain vertex at this coordinate
		Vec3 GetTerrainVertexWorldLocation(const TerrainCoordinate& coordinate) const;

		// Get the general barycentric interpolation of a value
		bool GetBarycentricData(float& outBeta, float& outGamma, Vec3& outA, Vec3& outB, Vec3& outC, const Vec3& P) const;

		// Get the interpolated terrain height at this exact position
		float ComputeBarycentricHeight(const Vec3& P) const;

		// Accessors
		TerrainMesh* GetTerrainMesh() const { return pTerrainMesh; }
		QuadTree* GetQuadTree() const { return pQuadTree; }

		// Debug methods
		virtual void DebugDraw() override;
		bool* GetRenderFoliagePtr() { return &renderFoliage; }

	private:
		TerrainMesh* pTerrainMesh;
		QuadTree* pQuadTree;
		bool renderFoliage = true;
	};
}

#endif