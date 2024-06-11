#include "TerrainFoliage.h"
#include "ListNode.h"
#include "GOFoliage.h"
#include "TerrainSystem.h"
#include "Terrain.h"
#include "QuadTree.h"
#include "CameraManager.h"
#include "ShaderObjectManager.h"
#include "SOFoliage.h"
#include "Engine.h"
#include "TerrainMesh.h"

namespace Azul
{
	TerrainFoliage::TerrainFoliage()
		: pBlendStateFoliage(nullptr)
	{
		HRESULT hr;

		CD3D11_BLEND_DESC bsd;
		ZeroMemory(&bsd, sizeof(CD3D11_BLEND_DESC));
		bsd.RenderTarget[0].BlendEnable = TRUE;
		bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		bsd.AlphaToCoverageEnable = true;

		hr = Engine::GetDevice()->CreateBlendState(&bsd, &pBlendStateFoliage);
		assert(SUCCEEDED(hr));
	}

	TerrainFoliage::~TerrainFoliage()
	{
		SafeRelease(pBlendStateFoliage);
	}

	void TerrainFoliage::Draw()
	{
		Engine::GetContext()->OMSetBlendState(pBlendStateFoliage, blendFactor, sampleMask);

		TerrainMesh* pTerrainMesh = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh();
		ID3D11ShaderResourceView* const pHeightfieldView = pTerrainMesh->GetView();
		Engine::GetContext()->VSSetShaderResources(1u, 1, &pHeightfieldView);

		QuadTree* pQuadTree = TerrainSystem::GetCurrentTerrain()->GetQuadTree();
		SOFoliage* pShader = (SOFoliage*)ShaderObjectManager::Find(ShaderObject::Name::Foliage);

		pShader->UnifiedOpen();

		std::vector<QTNode*> visible = pQuadTree->GetNodesInFrustum(CameraManager::GetCurrentCamera()->GetFrustum(pShader->GetFoliageParams().renderDistance), pQuadTree->GetDepth() - GOFoliage::LEVELS_UP);

		for (Iterator& it = *graphicsObjects.GetIterator(); !it.IsDone(); it.Next())
		{
			GOFoliage* pGo = (GOFoliage*)((ListNode*)it.Curr())->Get();
			pShader->FoliageTypeOpen(pGo->GetFoliageType());
			pGo->PrepareVisibleInstanceBuffer(visible);
			pGo->Render();
		}

		Engine::GetContext()->OMSetBlendState(nullptr, blendFactor, sampleMask);

		static ID3D11ShaderResourceView* pSRVNull = nullptr;
		Engine::GetContext()->VSSetShaderResources(1u, 1, &pSRVNull);
	}

	void TerrainFoliage::Bind(Terrain* pInTerrain)
	{
		for (Iterator& it = *graphicsObjects.GetIterator(); !it.IsDone(); it.Next())
		{
			GOFoliage* pGo = (GOFoliage*)((ListNode*)it.Curr())->Get();
			pGo->BindToTerrain(pInTerrain);
		}
	}

	void TerrainFoliage::Serialize(terrain_proto& out) const
	{
		for (Iterator& it = *graphicsObjects.GetIterator(); !it.IsDone(); it.Next())
		{
			GOFoliage* pGo = (GOFoliage*)((ListNode*)it.Curr())->Get();
			out.add_foliagetypeids((uint32_t)pGo->GetFoliageType()->GetName());

			std::vector<FoliageInstance> masterList;

			std::vector<FoliageInstance>* pStorage = pGo->GetInstanceStorage();

			for (uint i = 0u; i < pGo->GetNumVectors(); i++)
			{
				const std::vector<FoliageInstance>& vector = pStorage[i];
				masterList.insert(masterList.end(), vector.begin(), vector.end());
			}

			out.add_foliageinstancedata(masterList.data(), masterList.size() * sizeof(FoliageInstance));
		}
	}

	void TerrainFoliage::Deserialize(const terrain_proto& in)
	{
		for (uint i = 0u; i < (uint)in.foliagetypeids().size(); i++)
		{
			FoliageType::Name typeId = (FoliageType::Name)in.foliagetypeids().at(i);
			GOFoliage* pGo = AddFoliageType(typeId);

			std::string pInstanceRawData = in.foliageinstancedata().at(i);

			uint numInstances = pInstanceRawData.size() / sizeof(FoliageInstance);
			FoliageInstance* pRaw = (FoliageInstance*)pInstanceRawData.data();

			for (uint j = 0u; j < numInstances; j++)
			{
				FoliageInstance& inst = pRaw[j];
				pGo->AddInstance(inst);
			}
		}
	}

	GOFoliage* TerrainFoliage::AddFoliageType(FoliageType::Name inName)
	{
		GOFoliage* pGo = new GOFoliage(inName);
		AddGraphicsObject(pGo);
		return pGo;
	}

	void TerrainFoliage::AddGraphicsObject(GOFoliage* pInGo)
	{
		pInGo->BindToTerrain(TerrainSystem::GetCurrentTerrain());
		graphicsObjects.Add(new OwningListNode(pInGo));
	}

	void TerrainFoliage::RemoveFoliageType(FoliageType::Name inName)
	{
		for (Iterator& it = *graphicsObjects.GetIterator(); !it.IsDone(); it.Next())
		{
			GOFoliage* pGo = (GOFoliage*)((ListNode*)it.Curr())->Get();

			if (pGo->GetFoliageType()->GetName() == inName)
			{
				DLink* pCurr = graphicsObjects.Remove(it.Curr());
				delete pCurr;
				return;
			}
		}
	}
}