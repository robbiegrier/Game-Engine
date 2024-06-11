#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOTerrain.h"
#include "CameraManager.h"
#include "Engine.h"
#include "CSTerrainBrush.h"
#include "TextureObjectManager.h"

namespace Azul
{
	GOTerrain::GOTerrain(Mesh* mesh, ShaderObject* pShaderObj)
		: GraphicsObject(mesh, pShaderObj)
	{
		name = Name::GraphicsObjectTerrain;

		HRESULT hr;

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampDesc.MaxAnisotropy = 16u;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0.f;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = Engine::GetDevice()->CreateSamplerState(&sampDesc, &pSplattingSampler);
		assert(SUCCEEDED(hr));

		D3D11_SAMPLER_DESC sampDescNorm;
		ZeroMemory(&sampDescNorm, sizeof(sampDescNorm));
		sampDescNorm.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDescNorm.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDescNorm.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDescNorm.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDescNorm.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDescNorm.MinLOD = 0.f;
		sampDescNorm.MaxLOD = D3D11_FLOAT32_MAX;
		hr = Engine::GetDevice()->CreateSamplerState(&sampDescNorm, &pNormSampler);
		assert(SUCCEEDED(hr));
	}

	GOTerrain::~GOTerrain()
	{
		SafeRelease(pSplattingSampler);
		SafeRelease(pNormSampler);
	}

	void GOTerrain::ActivateGraphics()
	{
		TerrainMesh* pTerrainMesh = (TerrainMesh*)pModel;

		for (uint i = 0u; i < TerrainMesh::NUM_SURFACES; i++)
		{
			TextureObject* pTexture = pTerrainMesh->GetSurfaceTexture(i);
			ID3D11ShaderResourceView* pSurfaceView = pTexture->GetResourceView();
			Engine::GetContext()->PSSetShaderResources(TerrainMesh::SURFACES_TEXTURE_REGISTER + i, 1, &pSurfaceView);

			TextureObject* pNormal = pTerrainMesh->GetSurfaceTexture(i)->pNormalMap;

			if (!pNormal)
			{
				pNormal = TextureObjectManager::Find(TextureObject::Name::Black);
			}

			ID3D11ShaderResourceView* pSurfaceNormalView = pNormal->GetResourceView();
			Engine::GetContext()->PSSetShaderResources(TerrainMesh::SURFACES_NORMALS_REGISTER + i, 1, &pSurfaceNormalView);
		}

		ID3D11ShaderResourceView* pWeightsView = pTerrainMesh->GetSurfaceWeightsView();
		Engine::GetContext()->PSSetShaderResources(TerrainMesh::SURFACES_WEIGHTS_REGISTER, 1, &pWeightsView);

		Engine::GetContext()->PSSetSamplers(0u, 1, &pSplattingSampler);
		Engine::GetContext()->PSSetSamplers(1u, 1, &pNormSampler);

		ID3D11ShaderResourceView* const pHeightfieldView = pTerrainMesh->GetView();
		Engine::GetContext()->VSSetShaderResources(1u, 1, &pHeightfieldView);
	}

	void GOTerrain::Draw()
	{
		pModel->RenderIndexBuffer();

		for (Iterator& it = *pModel->GetSubMeshList().GetIterator(); !it.IsDone(); it.Next())
		{
			Mesh* pSubMesh = (Mesh*)it.Curr();
			pSubMesh->RenderIndexBuffer();
		}

		static ID3D11ShaderResourceView* pSRVNull = nullptr;
		Engine::GetContext()->VSSetShaderResources(1u, 1, &pSRVNull);

		Engine::GetContext()->PSSetShaderResources(TerrainMesh::SURFACES_WEIGHTS_REGISTER, 1, &pSRVNull);
		Engine::GetContext()->VSSetShaderResources(TerrainMesh::SURFACES_WEIGHTS_REGISTER, 1, &pSRVNull);
	}

	GraphicsObject* GOTerrain::Clone()
	{
		return new GOTerrain(pModel, pShaderObject);
	}

	TextureObject* GOTerrain::GetTexture()
	{
		TerrainMesh* pTerrainMesh = (TerrainMesh*)pModel;
		return pTerrainMesh->GetSurfaceTexture(0);
	}
}