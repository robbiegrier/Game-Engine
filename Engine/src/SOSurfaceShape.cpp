#include "SOSurfaceShape.h"
#include "Mesh.h"
#include "Engine.h"
#include "Terrain.h"
#include "TerrainSystem.h"
#include "TerrainMesh.h"

#include "SurfaceShape.Px.h"
#include "SurfaceShape.Vx.h"

namespace Azul
{
	SOSurfaceShape::SOSurfaceShape()
	{
		this->CreateVertexShader(g_SurfaceShape_VxShader, sizeof(g_SurfaceShape_VxShader));

		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		this->CreateInputLayout(g_SurfaceShape_VxShader, sizeof(g_SurfaceShape_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));
		this->CreatePixelShader(g_SurfaceShape_PxShader, sizeof(g_SurfaceShape_PxShader));

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
		hr = Engine::GetDevice()->CreateBlendState(&bsd, &pBlendState);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC buffDescTerrainParams;
		buffDescTerrainParams.Usage = D3D11_USAGE_DEFAULT;
		buffDescTerrainParams.ByteWidth = sizeof(CBTerrainParameters);
		buffDescTerrainParams.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescTerrainParams.CPUAccessFlags = 0;
		buffDescTerrainParams.MiscFlags = 0;
		buffDescTerrainParams.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescTerrainParams, nullptr, &pConstBuffTerrainParams);
		assert(SUCCEEDED(hr));

		D3D11_RASTERIZER_DESC rasterizerDesc;
		memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));
		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		hr = Engine::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
		assert(SUCCEEDED(hr));
	}

	SOSurfaceShape::~SOSurfaceShape()
	{
		SafeRelease(pBlendState);
		SafeRelease(pConstBuffTerrainParams);
		SafeRelease(pRasterizerState);
	}

	void SOSurfaceShape::OnOpen(GraphicsObject* pObject)
	{
		static_cast<void>(pObject);
		Engine::GetContext()->OMSetBlendState(pBlendState, blendFactor, sampleMask);

		TerrainMesh* pTerrainMesh = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh();
		ID3D11ShaderResourceView* const pHeightfieldView = pTerrainMesh->GetView();
		Engine::GetContext()->PSSetShaderResources(1u, 1, &pHeightfieldView);

		CBTerrainParameters terrainParams;
		terrainParams.gridCellSize = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSingleGridCellScale();
		terrainParams.halfWorldScale = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHalfWorldScale();
		terrainParams.heightScale = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHeightScale();
		Engine::GetContext()->UpdateSubresource(pConstBuffTerrainParams, 0, nullptr, &terrainParams, 0, 0);
		Engine::GetContext()->PSSetConstantBuffers(8u, 1, &pConstBuffTerrainParams);

		Engine::GetContext()->RSSetState(pRasterizerState);
	}

	void SOSurfaceShape::OnClose()
	{
		Engine::GetContext()->OMSetBlendState(nullptr, blendFactor, sampleMask);
		Engine::RevertRasterState();
	}
}