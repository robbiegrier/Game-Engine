#include "CSSurfaceBrush.h"
#include "Engine.h"
#include "TerrainBrush.Cx.h"
#include "TerrainSystem.h"
#include "Terrain.h"
#include "TerrainMesh.h"
#include "QuadTree.h"
#include "imgui.h"
#include "EditorGui.h"

namespace Azul
{
	CSSurfaceBrush::CSSurfaceBrush(const void* pComputeShader, uint bytes)
	{
		InitializeComputeShader(pComputeShader, bytes);

		HRESULT hr;

		D3D11_BUFFER_DESC buffDescConstBuff{ 0 };
		buffDescConstBuff.Usage = D3D11_USAGE_DEFAULT;
		buffDescConstBuff.ByteWidth = sizeof(CBTerrainBrush);
		buffDescConstBuff.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescConstBuff.CPUAccessFlags = 0;
		buffDescConstBuff.MiscFlags = 0;
		buffDescConstBuff.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescConstBuff, nullptr, &pConstBuff);
		assert(SUCCEEDED(hr));
	}

	CSSurfaceBrush::~CSSurfaceBrush()
	{
		SafeRelease(pComputeShader);
		SafeRelease(pConstBuff);
	}

	PaintResult CSSurfaceBrush::Paint(float radius, const Vec3& center, float force)
	{
		if (mapInFlight)
		{
			return PaintResult::Waiting;
		}

		Engine::GetContext()->CSSetShader(pComputeShader, nullptr, 0u);

		TerrainArea area(center, radius);

		CBTerrainBrush constants;
		constants.offset = area.GetMin();
		constants.force = force;
		constants.center = Vec4(center, 1.f);
		constants.radius = radius;
		constants.gridCellSize = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSingleGridCellScale();
		constants.halfWorldScale = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHalfWorldScale();

		Engine::GetContext()->UpdateSubresource(pConstBuff, 0u, nullptr, &constants, 0u, 0u);
		Engine::GetContext()->CSSetConstantBuffers(0u, 1u, &pConstBuff);

		ID3D11UnorderedAccessView* pUAV = EditorGui::GetTerrainEditor().GetSurfaceWeightsUAV();
		Engine::GetContext()->CSSetUnorderedAccessViews(0u, 1, &pUAV, nullptr);

		Open();

		Engine::GetContext()->Dispatch(GetDispatchDimension(area.GetWidth()), GetDispatchDimension(area.GetHeight()), 1u);

		mapInFlight = true;

		flightTimer.Tic();

		return PaintResult::Success;
	}

	PaintResult CSSurfaceBrush::ShiftPaint(float radius, const Vec3& center, float force)
	{
		static_cast<void>(radius);
		static_cast<void>(center);
		static_cast<void>(force);
		// Do Nothing

		return PaintResult::Success;
	}

	void CSSurfaceBrush::Update()
	{
		static bool isWait = false;

		if (mapInFlight)
		{
			ID3D11Texture2D* pComputeOutBuffer = EditorGui::GetTerrainEditor().GetSurfaceWeightsComputeOutBuffer();

			if (!isWait)
			{
				Engine::GetContext()->CopyResource(pComputeOutBuffer, TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSurfaceWeightsTexture());
			}

			D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
			HRESULT hr;

			hr = Engine::GetContext()->Map(pComputeOutBuffer, 0, D3D11_MAP_READ, D3D11_MAP_FLAG_DO_NOT_WAIT, &mappedResource);

			if (SUCCEEDED(hr))
			{
				assert(mappedResource.pData);
				float* pSurfaceWeightsDownload = (float*)mappedResource.pData;
				float* pTerrainSurfaceWeights = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSurfaceWeights();
				const uint resolution = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSurfaceWeightsTextureResolution();
				memcpy(pTerrainSurfaceWeights, pSurfaceWeightsDownload, resolution * resolution * sizeof(float) * 4u);
				Engine::GetContext()->Unmap(pComputeOutBuffer, 0);

				isWait = false;
				mapInFlight = false;
			}
			else
			{
				isWait = true;
			}
		}
	}

	uint CSSurfaceBrush::GetDispatchDimension(uint in)
	{
		uint out = in / NUM_THREADS;

		const bool cleanDiv = in % NUM_THREADS == 0u;
		if (!cleanDiv)
		{
			out += 1u;
		}

		return out;
	}

	void CSSurfaceBrush::InitializeComputeShader(const void* pShaderCode, uint bytes)
	{
		HRESULT hr;
		hr = Engine::GetDevice()->CreateComputeShader(pShaderCode, bytes, nullptr, &pComputeShader);
		assert(SUCCEEDED(hr));
		assert(pComputeShader);
	}
}