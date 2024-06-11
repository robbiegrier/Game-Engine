#ifndef _QuadTree
#define _QuadTree

#include "QTNode.h"
#include <vector>

namespace Azul
{
	class TerrainMesh;
	class GraphicsObject;

	// The quad tree provides a partitioning of the terrain for efficent intersection tests.
	// Engine and game code can use the tree to find a specific intersection point, or other things
	// such as the set of visible nodes. The nodes are stored in a sequential array and the heirarchy
	// is implied by the dimensions of the tree.
	class QuadTree
	{
	public:
		// Big four
		QuadTree();
		QuadTree(const QuadTree&) = delete;
		QuadTree& operator=(const QuadTree&) = delete;
		~QuadTree();

		// Generate the tree from a terrain asset
		void Create(TerrainMesh* pInTerrainMesh);

		// Perform a raycast. Returns true if the raycast hit somewhere on the terrain. The hit location is stored in out.
		bool Raycast(const Vec3& rayOrigin, const Vec3& rayDir, Vec3& out) const;

		// Get all leaf nodes hit by the raycast. Used as part of the raycast algorithm.
		std::vector<QTHit> RaycastAllLeaves(const Vec3& rayOrigin, const Vec3& rayDir) const;

		// Get all nodes at a given level within the frustum. Useful for finding visible partitions at a given resolution.
		std::vector<QTNode*> GetNodesInFrustum(const Frustum& frustum, uint level) const;

		// debug & helpers
		void Draw();
		void Dump();
		unsigned int GetNumNodesOnLevel(unsigned int level) const;
		unsigned int GetFirstIndexOnLevel(unsigned int level) const;
		void DrawNodeAndParents(uint nodeIndex) const;
		void OnVertexHeightChanged(const TerrainCoordinate& coordinate, float inHeight);
		void OnVertexHeightChangedPropogate(const TerrainCoordinate& coordinate);
		QTNode* GetLeafNodeForVertex(const TerrainCoordinate& coordinate) const;
		void DrawNode(const QTNode& node) const;
		void DrawNode(uint nodeIndex) const;
		uint GetDepth() const { return depth; }

	private:
		void UpdateLeafHeight(QTNode* pLeaf);
		void BuildFlatTree();
		void InsertAllHeightValues();
		QTRect GetNorthWestChildArea(const QTRect& pParentArea) const;
		QTRect GetNorthEastChildArea(const QTRect& pParentArea) const;
		QTRect GetSouthWestChildArea(const QTRect& pParentArea) const;
		QTRect GetSouthEastChildArea(const QTRect& pParentArea) const;
		bool IsLeafNode(const QTNode& node) const;
		uint GetNodeLevel(const QTNode& node) const;

		QTNode* pNodes;
		QTNode** pOrderedNodes;
		GraphicsObject* pVolumeGraphicsObject;
		TerrainMesh* pTerrainMesh;
		bool printStats = false;

		unsigned int totalNodes = 0u;
		unsigned int depth = 0u;
		static constexpr unsigned int LEAF_SIZE = 8u;
	};
}

#endif
