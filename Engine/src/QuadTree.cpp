#include "QuadTree.h"
#include "TerrainMesh.h"
#include "GOConstColor.h"
#include "AnimTimer.h"
#include "CameraManager.h"
#include "Camera.h"

#include <queue>

namespace Azul
{
	QuadTree::QuadTree()
	{
		pNodes = nullptr;
		pVolumeGraphicsObject = GOConstColor::MakeWireframeCube(Vec3(.0f, .0f, 1.f));
	}

	QuadTree::~QuadTree()
	{
		delete pVolumeGraphicsObject;
		delete[] pNodes;
		delete[] pOrderedNodes;
	}

	void QuadTree::Create(TerrainMesh* pInTerrainMesh)
	{
		AnimTimer timer;
		timer.Tic();

		pTerrainMesh = pInTerrainMesh;

		depth = 0u;
		for (unsigned int i = LEAF_SIZE; i != pTerrainMesh->GetVertexResolution(); i = i * 2u)
		{
			depth++;
		}

		if (printStats)
		{
			Trace::out("Terrain resolution: %d\n", pTerrainMesh->GetVertexResolution());
			Trace::out("Leaf size: %d\n", LEAF_SIZE);
			Trace::out("Depth: %d\n", depth);
		}

		totalNodes = 0u;
		for (unsigned int i = 0; i <= depth; i++)
		{
			unsigned int nodesAtLevel = GetNumNodesOnLevel(i);
			totalNodes += nodesAtLevel;

			if (printStats) Trace::out("Nodes at level %d: %d\n", i, nodesAtLevel);
		}

		if (printStats)
		{
			Trace::out("Total Nodes: %d\n", totalNodes);
			Trace::out("Memory Usage: %dmb\n", totalNodes * sizeof(QTNode) / 1'048'576u);
			Trace::out("Memory Usage (Order): %dmb\n", sizeof(QTNode*) * pTerrainMesh->GetTotalVertices() / 1'048'576u);
		}

		pNodes = new QTNode[totalNodes];

		pOrderedNodes = new QTNode * [pTerrainMesh->GetTotalVertices()];
		memset(pOrderedNodes, 0, sizeof(QTNode*) * pTerrainMesh->GetTotalVertices());

		BuildFlatTree();
		InsertAllHeightValues();

		if (printStats)
		{
			AnimTime time = timer.Toc();
			Trace::out("Generated Quad Tree in: %ds %dms\n",
				(int)AnimTime::Quotient(time, AnimTime(AnimTime::Duration::ONE_SECOND)),
				(int)AnimTime::Quotient(AnimTime::Remainder(time, AnimTime(AnimTime::Duration::ONE_SECOND)), AnimTime(AnimTime::Duration::ONE_MILLISECOND)));
		}

		//Dump();
	}

	bool QuadTree::Raycast(const Vec3& rayOrigin, const Vec3& rayDir, Vec3& out) const
	{
		std::vector<QTHit> allLeaves = RaycastAllLeaves(rayOrigin, rayDir);
		std::sort(allLeaves.begin(), allLeaves.end(), [](const QTHit& hit1, const QTHit& hit2) { return fabs(hit1.entryDistance) < fabs(hit2.entryDistance); });

		float closestDistanceTriangle = std::numeric_limits<float>().max();
		QTTriangleHit raycastClosest;

		if (!allLeaves.empty())
		{
			for (const QTHit& hit : allLeaves)
			{
				QTNode* pNode = &pNodes[hit.nodeIndex];

				uint leafAreaMinX = static_cast<uint>((pNode->area.min.x + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
				uint leafAreaMinZ = static_cast<uint>((pNode->area.min.z + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
				uint leafAreaMaxX = static_cast<uint>((pNode->area.max.x + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
				uint leafAreaMaxZ = static_cast<uint>((pNode->area.max.z + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());

				int triIndex = 0;

				static constexpr uint numTrianges = (LEAF_SIZE + 1) * (LEAF_SIZE + 1) * 8u;
				Vec3 triangles[numTrianges * 3u];

				for (uint aix = leafAreaMinX; aix <= leafAreaMaxX; aix++)
				{
					for (uint ajx = leafAreaMinZ; ajx <= leafAreaMaxZ; ajx++)
					{
						if (aix > 0u && ajx > 0u && aix < pTerrainMesh->GetVertexResolution() - 1u && ajx < pTerrainMesh->GetVertexResolution() - 1u)
						{
							const bool starbound = (ajx + aix) % 2u == 0u;

							if (starbound)
							{
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx + 1, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx + 1, aix + 1);

								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx + 1, aix + 1);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix + 1);

								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix + 1);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx - 1, aix + 1);

								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx - 1, aix + 1);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx - 1, aix);

								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx - 1, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx - 1, aix - 1);

								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx - 1, aix - 1);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix - 1);

								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix - 1);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx + 1, aix - 1);

								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx, aix);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx + 1, aix - 1);
								triangles[triIndex++] = pTerrainMesh->GetVertex(ajx + 1, aix);

								//Trace::out("%d\n", triIndex);
							}
						}
					}
				}

				for (uint i = 0u; i != numTrianges * 3u; i += 3u)
				{
					QTTriangleHit raycast;
					RayVsTriangle(raycast, rayOrigin, rayDir, triangles[i], triangles[i + 1u], triangles[i + 2u]);

					if (raycast.hit)
					{
						if (raycast.distance < closestDistanceTriangle)
						{
							closestDistanceTriangle = raycast.distance;
							raycastClosest = raycast;
						}
					}
				}

				if (raycastClosest.hit)
				{
					Vec3 point = rayOrigin + (rayDir)*raycastClosest.distance;
					out = point;
					return true;
				}
			}
		}

		return false;
	}

	std::vector<QTHit> QuadTree::RaycastAllLeaves(const Vec3& rayOrigin, const Vec3& rayDir) const
	{
		std::queue<QTNode*> queue;
		std::vector<QTHit> output;

		QTHit intersected;

		queue.push(&pNodes[0]);

		while (!queue.empty())
		{
			QTNode* pNode = queue.front();
			queue.pop();

			pNode->RayIntersection(intersected, rayOrigin, rayDir);

			if (intersected.hit)
			{
				if (IsLeafNode(*pNode))
				{
					if (intersected.entryDistance >= -MATH_TOLERANCE)
					{
						output.push_back(intersected);
					}
				}
				else
				{
					for (int c = pNode->GetFirstChildIndex(), max = c + 4; c < max; c++)
					{
						QTNode* pChild = &pNodes[c];
						queue.push(pChild);
					}
				}
			}
		}

		return output;
	}

	std::vector<QTNode*> QuadTree::GetNodesInFrustum(const Frustum& frustum, uint level) const
	{
		std::queue<QTNode*> queue;
		std::vector<QTNode*> output;

		queue.push(&pNodes[0]);

		while (!queue.empty())
		{
			QTNode* pNode = queue.front();
			queue.pop();

			bool intersected = pNode->FrustumIntersection(frustum);

			if (intersected)
			{
				if (GetNodeLevel(*pNode) == level)
				{
					output.push_back(pNode);
				}
				else
				{
					for (int c = pNode->GetFirstChildIndex(), max = c + 4; c < max; c++)
					{
						QTNode* pChild = &pNodes[c];
						queue.push(pChild);
					}
				}
			}
		}

		return output;
	}

	void QuadTree::BuildFlatTree()
	{
		const float terrainHalfSize = pTerrainMesh->GetHalfWorldScale();

		QTNode* pNode = &pNodes[0u];
		pNode->area.min = QTPoint{ -terrainHalfSize,-terrainHalfSize };
		pNode->area.max = QTPoint{ terrainHalfSize, terrainHalfSize };
		pNode->heightMin = 0.f;
		pNode->heightMax = 100.f;

		for (uint i = 1u; i < totalNodes; i += 4u)
		{
			QTNode* pParent = &pNodes[(i - 1u) / 4u];

			pNode = &pNodes[i];
			pNode->area = GetNorthWestChildArea(pParent->area);
			pNode->heightMin = 0.f;
			pNode->heightMax = 100.f;
			pNode->nodeIndex = i;

			pNode++;
			pNode->area = GetNorthEastChildArea(pParent->area);
			pNode->heightMin = 0.f;
			pNode->heightMax = 100.f;
			pNode->nodeIndex = i + 1u;

			pNode++;
			pNode->area = GetSouthWestChildArea(pParent->area);
			pNode->heightMin = 0.f;
			pNode->heightMax = 100.f;
			pNode->nodeIndex = i + 2u;

			pNode++;
			pNode->area = GetSouthEastChildArea(pParent->area);
			pNode->heightMin = 0.f;
			pNode->heightMax = 100.f;
			pNode->nodeIndex = i + 3u;
		}
	}

	void QuadTree::InsertAllHeightValues()
	{
		const uint nodesAtLeafLevel = GetNumNodesOnLevel(depth);
		const uint leafFirstIndex = totalNodes - nodesAtLeafLevel;

		for (int i = totalNodes - 1u; i >= 0; i--)
		{
			QTNode* pNode = &pNodes[i];

			pNode->heightMin = std::numeric_limits<float>().max();
			pNode->heightMax = -std::numeric_limits<float>().max();

			if ((uint)i >= leafFirstIndex)
			{
				uint leafAreaMinX = static_cast<uint>((pNode->area.min.x + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
				uint leafAreaMinZ = static_cast<uint>((pNode->area.min.z + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
				uint leafAreaMaxX = static_cast<uint>((pNode->area.max.x + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
				uint leafAreaMaxZ = static_cast<uint>((pNode->area.max.z + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());

				for (uint aix = leafAreaMinX; aix <= leafAreaMaxX; aix++)
				{
					for (uint ajx = leafAreaMinZ; ajx <= leafAreaMaxZ; ajx++)
					{
						if (aix < pTerrainMesh->GetVertexResolution() && ajx < pTerrainMesh->GetVertexResolution())
						{
							Vec3 vertex = pTerrainMesh->GetVertex(ajx, aix);

							pNode->heightMin = std::min(pNode->heightMin, vertex.y());
							pNode->heightMax = std::max(pNode->heightMax, vertex.y());

							pOrderedNodes[pTerrainMesh->GetGridIndex(aix, ajx)] = pNode;
						}
					}
				}
			}
			else
			{
				for (int c = (i * 4) + 1, max = c + 4; c < max; c++)
				{
					QTNode* pChild = &pNodes[c];

					pNode->heightMin = std::min(pNode->heightMin, pChild->heightMin);
					pNode->heightMax = std::max(pNode->heightMax, pChild->heightMax);
				}
			}
		}
	}

	QTRect QuadTree::GetNorthWestChildArea(const QTRect& pParentArea) const
	{
		return QTRect(
			(pParentArea.min.x + pParentArea.max.x) / 2.f,
			(pParentArea.min.z + pParentArea.max.z) / 2.f,
			pParentArea.max.x,
			pParentArea.max.z
		);
	}

	QTRect QuadTree::GetNorthEastChildArea(const QTRect& pParentArea) const
	{
		return QTRect(
			pParentArea.min.x,
			(pParentArea.min.z + pParentArea.max.z) / 2.f,
			(pParentArea.min.x + pParentArea.max.x) / 2.f,
			pParentArea.max.z
		);
	}

	QTRect QuadTree::GetSouthWestChildArea(const QTRect& pParentArea) const
	{
		return QTRect(
			(pParentArea.min.x + pParentArea.max.x) / 2.f,
			pParentArea.min.z,
			pParentArea.max.x,
			(pParentArea.min.z + pParentArea.max.z) / 2.f
		);
	}

	QTRect QuadTree::GetSouthEastChildArea(const QTRect& pParentArea) const
	{
		return QTRect(
			pParentArea.min.x,
			pParentArea.min.z,
			(pParentArea.min.x + pParentArea.max.x) / 2.f,
			(pParentArea.min.z + pParentArea.max.z) / 2.f
		);
	}

	bool QuadTree::IsLeafNode(const QTNode& node) const
	{
		uint leafAreaMinX = static_cast<uint>((node.area.min.x + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
		uint leafAreaMaxX = static_cast<uint>((node.area.max.x + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
		return leafAreaMaxX - leafAreaMinX == LEAF_SIZE;
	}

	uint QuadTree::GetNodeLevel(const QTNode& node) const
	{
		QTNode const* pNode = &node;

		uint counter = depth;

		while (pNode->GetFirstChildIndex() < totalNodes)
		{
			counter--;
			pNode = &pNodes[pNode->GetFirstChildIndex()];
		}

		return counter;
	}

	void QuadTree::DrawNode(const QTNode& node) const
	{
		Vec4 relativeMin = Vec4(node.GetAABBMin(), 1.f);
		Vec4 relativeMax = Vec4(node.GetAABBMax(), 1.f);
		Mat4 minMaxMat = Scale(Vec3(relativeMax - relativeMin)) * Trans(Vec3(0.5f * (relativeMax + relativeMin)));

		pVolumeGraphicsObject->SetWorld(minMaxMat);
		pVolumeGraphicsObject->Render();
	}

	void QuadTree::DrawNode(uint nodeIndex) const
	{
		DrawNode(pNodes[nodeIndex]);
	}

	void QuadTree::DrawNodeAndParents(uint nodeIndex) const
	{
		const QTNode* pNode = &pNodes[nodeIndex];

		GOConstColor* pGOColor = (GOConstColor*)pVolumeGraphicsObject;
		Vec3 col(0, 0, 0);
		Vec3 delta(0.06f, 0.06f, 0.15f);
		pGOColor->SetColor(col);
		DrawNode(*pNode);

		uint parentIndex = (pNode->nodeIndex - 1u) / 4u;

		while (parentIndex != 0)
		{
			QTNode* pParent = &pNodes[parentIndex];

			col += delta;
			pGOColor->SetColor(col);
			DrawNode(*pParent);

			parentIndex = (parentIndex - 1u) / 4u;
		}

		QTNode* pRoot = &pNodes[0];

		col += delta;
		pGOColor->SetColor(col);
		DrawNode(*pRoot);
	}

	void QuadTree::OnVertexHeightChanged(const TerrainCoordinate& coordinate, float inHeight)
	{
		if (coordinate.x < pTerrainMesh->GetVertexResolution() && coordinate.y < pTerrainMesh->GetVertexResolution())
		{
			QTNode* pLeafNode = pOrderedNodes[pTerrainMesh->GetGridIndex(coordinate.x, coordinate.y)];

			pLeafNode->heightMin = std::min(pLeafNode->heightMin, inHeight);
			pLeafNode->heightMax = std::max(pLeafNode->heightMax, inHeight);

			pLeafNode->dirty = true;
		}
	}

	void QuadTree::OnVertexHeightChangedPropogate(const TerrainCoordinate& coordinate)
	{
		const uint index = pTerrainMesh->GetGridIndex(coordinate.x, coordinate.y);

		if (coordinate.x < pTerrainMesh->GetVertexResolution() && coordinate.y < pTerrainMesh->GetVertexResolution())
		{
			QTNode* pLeafNode = pOrderedNodes[index];

			if (pLeafNode->dirty)
			{
				pLeafNode->dirty = false;
				UpdateLeafHeight(pLeafNode);
			}
		}
	}

	void QuadTree::UpdateLeafHeight(QTNode* pLeafNode)
	{
		pLeafNode->heightMin = std::numeric_limits<float>().max();
		pLeafNode->heightMax = -std::numeric_limits<float>().max();

		uint leafAreaMinX = static_cast<uint>((pLeafNode->area.min.x + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
		uint leafAreaMinZ = static_cast<uint>((pLeafNode->area.min.z + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
		uint leafAreaMaxX = static_cast<uint>((pLeafNode->area.max.x + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());
		uint leafAreaMaxZ = static_cast<uint>((pLeafNode->area.max.z + pTerrainMesh->GetHalfWorldScale()) / pTerrainMesh->GetSingleGridCellScale());

		for (uint aix = leafAreaMinX; aix <= leafAreaMaxX; aix++)
		{
			for (uint ajx = leafAreaMinZ; ajx <= leafAreaMaxZ; ajx++)
			{
				if (aix < pTerrainMesh->GetVertexResolution() && ajx < pTerrainMesh->GetVertexResolution())
				{
					Vec3 vertex = pTerrainMesh->GetVertex(ajx, aix);

					pLeafNode->heightMin = std::min(pLeafNode->heightMin, vertex.y());
					pLeafNode->heightMax = std::max(pLeafNode->heightMax, vertex.y());
				}
			}
		}

		QTNode* pChild = pLeafNode;

		uint parentIndex = (pLeafNode->nodeIndex - 1u) / 4u;

		while (parentIndex != 0)
		{
			QTNode* pParent = &pNodes[parentIndex];

			pParent->heightMin = std::numeric_limits<float>().max();
			pParent->heightMax = -std::numeric_limits<float>().max();

			uint firstChildIndex = pParent->GetFirstChildIndex();
			for (uint i = firstChildIndex; i < firstChildIndex + 4u; i++)
			{
				QTNode* pCurrChild = &pNodes[i];

				pParent->heightMin = std::min(pParent->heightMin, pCurrChild->heightMin);
				pParent->heightMax = std::max(pParent->heightMax, pCurrChild->heightMax);
			}

			pChild = pParent;

			parentIndex = (parentIndex - 1u) / 4u;
		}

		QTNode* pRoot = &pNodes[0];

		pRoot->heightMin = std::numeric_limits<float>().max();
		pRoot->heightMax = -std::numeric_limits<float>().max();

		uint firstChildIndex = pRoot->GetFirstChildIndex();
		for (uint i = firstChildIndex; i < firstChildIndex + 4u; i++)
		{
			QTNode* pCurrChild = &pNodes[i];

			pRoot->heightMin = std::min(pRoot->heightMin, pCurrChild->heightMin);
			pRoot->heightMax = std::max(pRoot->heightMax, pCurrChild->heightMax);
		}
	}

	QTNode* QuadTree::GetLeafNodeForVertex(const TerrainCoordinate& coordinate) const
	{
		QTNode* pLeafNode = pOrderedNodes[pTerrainMesh->GetGridIndex(coordinate.x, coordinate.y)];
		return pLeafNode;
	}

	void QuadTree::Draw()
	{
		uint rendStart = 0u;

		static uint depthToDraw = 0u;
		static bool pressed = false;

		if (GetKeyState('U') & 0x8000)
		{
			if (!pressed)
			{
				depthToDraw = (depthToDraw + 1u) % (depth + 1u);
				pressed = true;
			}
		}
		else
		{
			pressed = false;
		}

		for (uint i = 0; i <= depth; i++)
		{
			const unsigned int nodesAtLevel = GetNumNodesOnLevel(i);
			if (i == depthToDraw)
			{
				for (unsigned int j = rendStart; j < rendStart + nodesAtLevel; j++)
				{
					((GOConstColor*)pVolumeGraphicsObject)->SetColor(Vec3(0.0f, 0.0f, .75f));
					DrawNode(pNodes[j]);
				}
			}
			rendStart = rendStart + nodesAtLevel;
		}
	}

	void QuadTree::Dump()
	{
		for (unsigned int i = 0u; i < totalNodes; i++)
		{
			QTNode* pCurrentNode = &pNodes[i];
			Trace::out("[%d]: min:[%.3f %.3f %.3f]  max:[%.3f %.3f %.3f]\n", i,
				pCurrentNode->GetAABBMin().x(), pCurrentNode->GetAABBMin().y(), pCurrentNode->GetAABBMin().z(),
				pCurrentNode->GetAABBMax().x(), pCurrentNode->GetAABBMax().y(), pCurrentNode->GetAABBMax().z()
			);
		}
	}

	unsigned int QuadTree::GetNumNodesOnLevel(unsigned int level) const
	{
		unsigned int nodesAtLevel = 1u;
		for (unsigned int i = 0; i < level; i++)
		{
			nodesAtLevel = nodesAtLevel * 4u;
		}

		return nodesAtLevel;
	}

	unsigned int QuadTree::GetFirstIndexOnLevel(unsigned int level) const
	{
		unsigned int steps = 0u;
		unsigned int nodesAtLevel = 1u;
		for (unsigned int i = 0; i < level; i++)
		{
			steps += nodesAtLevel;
			nodesAtLevel = nodesAtLevel * 4u;
		}

		return steps;
	}
}