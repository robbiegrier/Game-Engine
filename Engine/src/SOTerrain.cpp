#include "SOTerrain.h"
#include "Mesh.h"
#include "Engine.h"
#include "CameraManager.h"
#include "GraphicsObject.h"
#include "Terrain.h"
#include "TerrainMesh.h"
#include "TerrainSystem.h"

// User generated headers...
#include "Terrain.Px.h"
#include "Terrain.Vx.h"

namespace Azul
{
	SOTerrain::SOTerrain()
	{
		HRESULT hr;

		this->CreateVertexShader(g_Terrain_VxShader, sizeof(g_Terrain_VxShader));

		// Create the input layout for the vertex shader.
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Norm, offsetof(VertexNorm,Norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateInputLayout(g_Terrain_VxShader, sizeof(g_Terrain_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));
		CreatePixelShader(g_Terrain_PxShader, sizeof(g_Terrain_PxShader));

		D3D11_BUFFER_DESC buffDescLightscape;
		buffDescLightscape.Usage = D3D11_USAGE_DEFAULT;
		buffDescLightscape.ByteWidth = sizeof(CBLightscape);
		buffDescLightscape.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescLightscape.CPUAccessFlags = 0;
		buffDescLightscape.MiscFlags = 0;
		buffDescLightscape.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescLightscape, nullptr, &pConstBuffLightscape);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC buffDescObject;
		buffDescObject.Usage = D3D11_USAGE_DEFAULT;
		buffDescObject.ByteWidth = sizeof(CBObject);
		buffDescObject.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescObject.CPUAccessFlags = 0;
		buffDescObject.MiscFlags = 0;
		buffDescObject.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescObject, nullptr, &pConstBuffObject);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC buffDescFog;
		buffDescFog.Usage = D3D11_USAGE_DEFAULT;
		buffDescFog.ByteWidth = sizeof(CBFog);
		buffDescFog.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescFog.CPUAccessFlags = 0;
		buffDescFog.MiscFlags = 0;
		buffDescFog.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescFog, nullptr, &pConstBuffFog);
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

		D3D11_BUFFER_DESC buffDescTerrainBrushArgs;
		buffDescTerrainBrushArgs.Usage = D3D11_USAGE_DEFAULT;
		buffDescTerrainBrushArgs.ByteWidth = sizeof(CBTerrainBrushArgs);
		buffDescTerrainBrushArgs.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescTerrainBrushArgs.CPUAccessFlags = 0;
		buffDescTerrainBrushArgs.MiscFlags = 0;
		buffDescTerrainBrushArgs.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescTerrainBrushArgs, nullptr, &pConstBuffTerrainBrushArgs);
		assert(SUCCEEDED(hr));
	}

	SOTerrain::~SOTerrain()
	{
		SafeRelease(pConstBuffObject);
		SafeRelease(pConstBuffLightscape);
		SafeRelease(pConstBuffFog);
		SafeRelease(pConstBuffTerrainParams);
		SafeRelease(pConstBuffTerrainBrushArgs);
	}

	void SOTerrain::SetFogParameters(float start, float range, const Vec4& col)
	{
		fog.FogStart = start;
		fog.FogRange = range;
		fog.FogColor = col;
	}

	void SOTerrain::SetCurrentObject(GraphicsObject* pObject)
	{
		Vec3 testAmb = Vec3(0.4f, 0.4f, 0.4f);
		Vec3 testDiff = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 testSpec = Vec3(0.5f, 0.5f, 1.0f);

		CBObject object;
		object.inverse = pObject->GetWorld().getInv();
		object.material.ambient = Vec4(testAmb, 1.0f);
		object.material.diffuse = Vec4(testDiff, 1.0f);
		object.material.specular = Vec4(testSpec, 1.0f);

		Engine::GetContext()->UpdateSubresource(pConstBuffObject, 0, nullptr, &object, 0, 0);
	}

	void SOTerrain::OnOpen(GraphicsObject* pObject)
	{
		// Object Specific
		CBObjectUniversal object;
		object.world = pObject->GetWorld();
		Engine::GetContext()->UpdateSubresource(pConstBuffObjectUniversal, 0, nullptr, &object, 0, 0);
		Engine::GetContext()->DSSetConstantBuffers((uint32_t)ConstantBufferSlot::World, 1u, &pConstBuffObjectUniversal);

		// Per Frame Lightscape data (can be optimized out of per-object rendering)
		CBLightscape lightscape;
		lightscape.numPointLights = LightSystem::MAX_LIGHTS;
		memcpy(lightscape.pointLights, LightSystem::GetPointLights(), sizeof(PointLight) * LightSystem::MAX_LIGHTS);
		lightscape.directionalLight = LightSystem::GetDirectionalLight();
		lightscape.eyePositionWorld = Vec4(GetCamera()->GetLocation(), 1.0f);
		Engine::GetContext()->UpdateSubresource(pConstBuffLightscape, 0, nullptr, &lightscape, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Lightscape, 1, &pConstBuffLightscape);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Lightscape, 1, &pConstBuffLightscape);

		Engine::GetContext()->UpdateSubresource(pConstBuffFog, 0, nullptr, &fog, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Fog, 1, &pConstBuffFog);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Fog, 1, &pConstBuffFog);

		CBTerrainParameters terrainParams;
		terrainParams.gridCellSize = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSingleGridCellScale();
		terrainParams.halfWorldScale = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHalfWorldScale();
		terrainParams.heightScale = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHeightScale();
		Engine::GetContext()->UpdateSubresource(pConstBuffTerrainParams, 0, nullptr, &terrainParams, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::TerrainParams, 1, &pConstBuffTerrainParams);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::TerrainParams, 1, &pConstBuffTerrainParams);

		Engine::GetContext()->UpdateSubresource(pConstBuffTerrainBrushArgs, 0, nullptr, &brushArgs, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::TerrainBrushArgs, 1, &pConstBuffTerrainBrushArgs);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::TerrainBrushArgs, 1, &pConstBuffTerrainBrushArgs);
		brushArgs = CBTerrainBrushArgs();

		// Per object data
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Object, 1, &pConstBuffObject);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Object, 1, &pConstBuffObject);
		SetCurrentObject(pObject);

		static bool tPressed = false;
		if (GetKeyState('T') & 0x8000 && GetKeyState(VK_SHIFT) & 0x8000)
		{
			if (!tPressed)
			{
				tPressed = true;
				wireframeMode = !wireframeMode;
			}
		}
		else
		{
			tPressed = false;
		}

		if (wireframeMode)
		{
			D3D11_RASTERIZER_DESC rasterizerDesc;
			memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

			rasterizerDesc.AntialiasedLineEnable = FALSE;
			rasterizerDesc.CullMode = D3D11_CULL_NONE;
			rasterizerDesc.DepthBias = 0;
			rasterizerDesc.DepthBiasClamp = 0.0f;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.MultisampleEnable = FALSE;
			rasterizerDesc.ScissorEnable = FALSE;
			rasterizerDesc.SlopeScaledDepthBias = 0.0f;

			ID3D11RasterizerState* pRasterState;
			HRESULT hr;
			hr = Engine::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterState);
			assert(SUCCEEDED(hr));

			Engine::GetContext()->RSSetState(pRasterState);

			SafeRelease(pRasterState);
		}
	}

	void SOTerrain::OnClose()
	{
		if (wireframeMode)
		{
			D3D11_RASTERIZER_DESC rasterizerDesc;
			memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

			rasterizerDesc.AntialiasedLineEnable = FALSE;
			rasterizerDesc.CullMode = D3D11_CULL_FRONT;
			rasterizerDesc.DepthBias = 0;
			rasterizerDesc.DepthBiasClamp = 0.0f;
			rasterizerDesc.DepthClipEnable = TRUE;
			rasterizerDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerDesc.FrontCounterClockwise = FALSE;
			rasterizerDesc.MultisampleEnable = FALSE;
			rasterizerDesc.ScissorEnable = FALSE;
			rasterizerDesc.SlopeScaledDepthBias = 0.0f;

			ID3D11RasterizerState* pRasterState;
			HRESULT hr;
			hr = Engine::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterState);
			assert(SUCCEEDED(hr));

			Engine::GetContext()->RSSetState(pRasterState);

			SafeRelease(pRasterState);
		}

		ID3D11ShaderResourceView* pDepthBufferView = nullptr;
		Engine::GetContext()->PSSetShaderResources(14u, 1, &pDepthBufferView);
	}
}