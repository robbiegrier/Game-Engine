#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOFoliage.h"
#include "CameraManager.h"
#include "Engine.h"
#include "TerrainSystem.h"
#include "Terrain.h"
#include "TerrainMesh.h"
#include "QTNode.h"
#include "QuadTree.h"
#include "SOFoliage.h"
#include "ShaderObjectManager.h"
#include "FoliageLibrary.h"
#include "ListNode.h"

namespace Azul
{
	GOFoliage::GOFoliage(FoliageType::Name foliageTypeName)
		: GOFoliage(FoliageLibrary::Find(foliageTypeName))
	{
	}

	GOFoliage::GOFoliage(FoliageType* pInFoliageType)
		: GraphicsObject(nullptr, ShaderObjectManager::Find(ShaderObject::Name::Foliage)),
		pTex(nullptr), pFoliageType(pInFoliageType)
	{
		name = Name::GraphicsObjectFoliage;
	}

	GOFoliage::~GOFoliage()
	{
		delete[] pInstanceData;
		SafeRelease(pVertexBufferInstanceData);
		delete[] vectors;
	}

	void GOFoliage::ActivateGraphics()
	{
	}

	void GOFoliage::Draw()
	{
		if (count > 0)
		{
			Iterator& itTex = *pFoliageType->textureList.GetIterator();
			for (Iterator& it = *pFoliageType->meshList.GetIterator(); !it.IsDone(); it.Next(), itTex.Next())
			{
				TextureObject* pCurrTexture = (TextureObject*)((ListNode*)itTex.Curr())->Get();
				Mesh* pCurrMesh = (Mesh*)((ListNode*)it.Curr())->Get();

				pCurrTexture->ActivateTexture();
				pCurrMesh->ActivateModel();
				Engine::GetContext()->IASetVertexBuffers(4u, 1u, &pVertexBufferInstanceData, &strideInst, &offset);
				Engine::GetContext()->DrawIndexedInstanced(pCurrMesh->GetNumIndicies(), count, 0u, 0u, 0u);
			}
		}
	}

	GraphicsObject* GOFoliage::Clone()
	{
		return new GOFoliage(pFoliageType);
	}

	TextureObject* GOFoliage::GetTexture()
	{
		return pTex;
	}

	void GOFoliage::BindToTerrain(Terrain* pInTerrian)
	{
		if (pInTerrian->GetTerrainMesh() == pTerrainMesh)
		{
			pTerrain = pInTerrian;
			return;
		}

		if (pVertexBufferInstanceData)
		{
			SafeRelease(pVertexBufferInstanceData);
			delete[] pInstanceData;
			delete[] vectors;
		}

		pTerrain = pInTerrian;
		pTerrainMesh = pInTerrian->GetTerrainMesh();
		QuadTree* pQuadTree = pTerrain->GetQuadTree();
		const uint targetDepth = pQuadTree->GetDepth() - LEVELS_UP;
		numVectors = pQuadTree->GetNumNodesOnLevel(targetDepth);
		vectors = new std::vector<FoliageInstance>[numVectors];

		pInstanceData = new FoliageInstance[MAX_INSTANCES_PER_DRAW];
		memset(pInstanceData, 0, MAX_INSTANCES_PER_DRAW * sizeof(FoliageInstance));

		D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
		D3D11_SUBRESOURCE_DATA resourceData{ 0 };

		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = MAX_INSTANCES_PER_DRAW * sizeof(FoliageInstance);
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;

		resourceData.pSysMem = pInstanceData;

		HRESULT hr = Engine::GetDevice()->CreateBuffer(&vertexBufferDesc, &resourceData, &pVertexBufferInstanceData);
		assert(SUCCEEDED(hr));
	}

	void GOFoliage::PrepareVisibleInstanceBuffer(const std::vector<QTNode*>& pVisibleNodes)
	{
		QuadTree* pQuadTree = pTerrain->GetQuadTree();
		uint firstOnLevel = pQuadTree->GetFirstIndexOnLevel(pQuadTree->GetDepth() - LEVELS_UP);
		uint renderCount = 0u;

		for (QTNode* pNode : pVisibleNodes)
		{
			const uint relativeIndex = pNode->nodeIndex - firstOnLevel;
			const std::vector<FoliageInstance>& instances = vectors[relativeIndex];

			const uint potentialCount = instances.size();
			const uint remainingSlots = MAX_INSTANCES_PER_DRAW - renderCount;
			const uint copyCount = std::min(potentialCount, remainingSlots);

			memcpy(&pInstanceData[renderCount], instances.data(), copyCount * sizeof(FoliageInstance));
			renderCount += copyCount;

			if (renderCount >= MAX_INSTANCES_PER_DRAW)
			{
				break;
			}
		}

		count = renderCount;

		D3D11_MAPPED_SUBRESOURCE resource;
		ZeroMemory(&resource, sizeof(resource));
		Engine::GetContext()->Map(pVertexBufferInstanceData, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &pInstanceData[0], count * sizeof(FoliageInstance));
		Engine::GetContext()->Unmap(pVertexBufferInstanceData, 0);
	}

	void GOFoliage::AddInstance(const FoliageInstance& instance)
	{
		QuadTree* pQuadTree = pTerrain->GetQuadTree();
		Vec3 location = Vec3(instance.x, 0.f, instance.z);
		QTNode* pNode = TerrainSystem::GetCurrentTerrain()->GetTreeNodeAt2D(location);

		if (pNode)
		{
			uint indexTarget = pNode->nodeIndex;
			for (uint i = 0u; i < LEVELS_UP; i++)
			{
				indexTarget = QTNode::GetParentIndex(indexTarget);
			}

			uint firstOnLevel = pQuadTree->GetFirstIndexOnLevel(pQuadTree->GetDepth() - LEVELS_UP);
			uint relativeIndex = indexTarget - firstOnLevel;
			vectors[relativeIndex].push_back(instance);

			totalInstances++;
		}
	}

	void GOFoliage::RemoveInstanceNear(const Vec3& removeFocusLocation, const Vec3& pinLocation, const float radius)
	{
		QuadTree* pQuadTree = pTerrain->GetQuadTree();
		QTNode* pNode = TerrainSystem::GetCurrentTerrain()->GetTreeNodeAt2D(removeFocusLocation);

		if (pNode)
		{
			uint indexTarget = pNode->nodeIndex;
			for (uint i = 0u; i < LEVELS_UP; i++)
			{
				indexTarget = QTNode::GetParentIndex(indexTarget);
			}

			uint firstOnLevel = pQuadTree->GetFirstIndexOnLevel(pQuadTree->GetDepth() - LEVELS_UP);
			uint relativeIndex = indexTarget - firstOnLevel;

			auto eraseMe = vectors[relativeIndex].end();
			for (auto it = vectors[relativeIndex].begin(); it != vectors[relativeIndex].end(); it++)
			{
				Vec3 locationOfInstance(it->x, 0.f, it->z);
				Vec3 pin2d(pinLocation.x(), 0.f, pinLocation.z());

				float dist = (locationOfInstance - pin2d).len();

				if (dist < radius)
				{
					eraseMe = it;
					break;
				}
			}

			if (eraseMe != vectors[relativeIndex].end())
			{
				vectors[relativeIndex].erase(eraseMe);
				totalInstances--;
			}
		}
	}
}