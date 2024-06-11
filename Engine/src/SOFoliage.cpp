#include "SOFoliage.h"
#include "Mesh.h"
#include "Engine.h"
#include "CameraManager.h"
#include "GraphicsObject.h"
#include "Terrain.h"
#include "TerrainSystem.h"
#include "TerrainMesh.h"

// User generated headers...
#include "Foliage.Px.h"
#include "Foliage.Vx.h"

namespace Azul
{
	SOFoliage::SOFoliage()
	{
		this->CreateVertexShader(g_Foliage_VxShader, sizeof(g_Foliage_VxShader));

		// Create the input layout for the vertex shader.
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Norm, offsetof(VertexNorm,Norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "INSTANCEX", 0,	  DXGI_FORMAT_R32_FLOAT, 4, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCEZ", 0,	  DXGI_FORMAT_R32_FLOAT, 4, 4, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCESCALE", 0, DXGI_FORMAT_R32_FLOAT, 4, 8, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCEANGLE", 0, DXGI_FORMAT_R32_FLOAT, 4, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INSTANCECOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};

		CreateInputLayout(g_Foliage_VxShader, sizeof(g_Foliage_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));
		CreatePixelShader(g_Foliage_PxShader, sizeof(g_Foliage_PxShader));

		HRESULT hr;
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

		D3D11_BUFFER_DESC buffDescFoliageParams;
		buffDescFoliageParams.Usage = D3D11_USAGE_DEFAULT;
		buffDescFoliageParams.ByteWidth = sizeof(CBFoliageParameters);
		buffDescFoliageParams.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescFoliageParams.CPUAccessFlags = 0;
		buffDescFoliageParams.MiscFlags = 0;
		buffDescFoliageParams.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescFoliageParams, nullptr, &pConstBuffFoliageParams);
		assert(SUCCEEDED(hr));
	}

	SOFoliage::~SOFoliage()
	{
		SafeRelease(pConstBuffObject);
		SafeRelease(pConstBuffLightscape);
		SafeRelease(pConstBuffFog);
		SafeRelease(pConstBuffTerrainParams);
		SafeRelease(pConstBuffFoliageParams);
	}

	void SOFoliage::SetFogParameters(float start, float range, const Vec4& col)
	{
		fog.FogStart = start;
		fog.FogRange = range;
		fog.FogColor = col;
	}

	void SOFoliage::SetCurrentObject(GraphicsObject* pObject)
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

	void SOFoliage::BindLights()
	{
		CBLightscape lightscape;
		lightscape.numPointLights = LightSystem::MAX_LIGHTS;
		memcpy(lightscape.pointLights, LightSystem::GetPointLights(), sizeof(PointLight) * LightSystem::MAX_LIGHTS);
		lightscape.directionalLight = LightSystem::GetDirectionalLight();
		lightscape.eyePositionWorld = Vec4(GetCamera()->GetLocation(), 1.0f);
		Engine::GetContext()->UpdateSubresource(pConstBuffLightscape, 0, nullptr, &lightscape, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Lightscape, 1, &pConstBuffLightscape);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Lightscape, 1, &pConstBuffLightscape);
	}

	void SOFoliage::OnOpen(GraphicsObject* pObject)
	{
		// Per object data
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Object, 1, &pConstBuffObject);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Object, 1, &pConstBuffObject);
		SetCurrentObject(pObject);
	}

	void SOFoliage::UnifiedOpen()
	{
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

		Engine::GetContext()->UpdateSubresource(pConstBuffFoliageParams, 0, nullptr, &foliageParams, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::FoliageParams, 1, &pConstBuffFoliageParams);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::FoliageParams, 1, &pConstBuffFoliageParams);
	}

	void SOFoliage::FoliageTypeOpen(FoliageType* pFolaigeType)
	{
		foliageParams.followNormal = pFolaigeType->followNormal ? 1.f : -1.f;
		Engine::GetContext()->UpdateSubresource(pConstBuffFoliageParams, 0, nullptr, &foliageParams, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::FoliageParams, 1, &pConstBuffFoliageParams);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::FoliageParams, 1, &pConstBuffFoliageParams);
	}

	void SOFoliage::OnClose()
	{
	}
}