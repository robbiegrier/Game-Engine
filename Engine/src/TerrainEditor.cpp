#include "TerrainEditor.h"
#include "TerrainBrushConvolution.h"
#include "TerrainBrushNoise.h"
#include "TerrainBrushRaise.h"
#include "TerrainBrushFlatten.h"
#include "SurfaceBrushPaint.h"
#include "SurfaceBrushConvolution.h"
#include "FoliageBrush.h"
#include "TerrainSystem.h"
#include "Terrain.h"
#include "TerrainMesh.h"
#include "Engine.h"

namespace Azul
{
	TerrainEditor::TerrainEditor()
		: pComputeOutBuffer(nullptr), pUAV(nullptr)
	{
		AddBrush("None", nullptr);
		AddBrush("Raise", new TerrainBrushRaise());
		AddBrush("Flatten", new TerrainBrushFlatten());
		AddBrush("Smooth", new TerrainBrushConvolution(TerrainBrushConvolution::MakeAverageBlurFilter()));
		AddBrush("Soften", new TerrainBrushConvolution(TerrainBrushConvolution::MakeGaussianBlurFilter(2.f)));
		AddBrush("Perlin Noise", new TerrainBrushNoise());
		AddBrush("Paint", new SurfaceBrushPaint());
		AddBrush("Smudge Paint", new SurfaceBrushConvolution(TerrainBrushConvolution::MakeAverageBlurFilter()));
		AddBrush("Gaussian Blur Paint", new SurfaceBrushConvolution(TerrainBrushConvolution::MakeGaussianBlurFilter(2.f)));
		AddBrush("Foliage", new FoliageBrush());
	}

	TerrainEditor::~TerrainEditor()
	{
		for (const auto& entry : brushes)
		{
			delete entry.second;
		}

		if (pComputeOutBuffer)
		{
			SafeRelease(pComputeOutBuffer);
			SafeRelease(pUAV);
			SafeRelease(pComputeOutBufferSurfaceWeights);
			SafeRelease(pSurfaceWeightsUAV);
		}
	}

	void TerrainEditor::AddBrush(const char* pName, ComputeShaderBrush* pBrush)
	{
		brushes.emplace(std::string(pName), pBrush);
	}

	ComputeShaderBrush* TerrainEditor::GetBrush(const char* pName) const
	{
		return brushes.find(std::string(pName))->second;
	}

	void TerrainEditor::OnTerrainChanged(Terrain* pNewTerrain)
	{
		UpdateComputeOutputBuffer(pNewTerrain);
		UpdateHeightfieldUAV(pNewTerrain);

		UpdateSurfaceWeightsComputeOutputBuffer(pNewTerrain);
		UpdateSurfaceWeightsUAV(pNewTerrain);

		//history.OnTerrainChanged(pNewTerrain);
	}

	void TerrainEditor::SetBrush(const char* pName)
	{
		pCurrentBrush = GetBrush(pName);
	}

	void TerrainEditor::UpdateComputeOutputBuffer(Terrain* pNewTerrain)
	{
		if (pComputeOutBuffer)
		{
			SafeRelease(pComputeOutBuffer);
		}

		const uint resolution = pNewTerrain->GetTerrainMesh()->GetVertexResolution();

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = resolution;
		desc.Height = resolution;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		HRESULT hr = Engine::GetDevice()->CreateTexture2D(&desc, nullptr, &pComputeOutBuffer);
		assert(SUCCEEDED(hr));
	}

	void TerrainEditor::UpdateHeightfieldUAV(Terrain* pNewTerrain)
	{
		if (pUAV)
		{
			SafeRelease(pUAV);
		}

		HRESULT hr = Engine::GetDevice()->CreateUnorderedAccessView(pNewTerrain->GetTerrainMesh()->GetHeightfieldTextureResource(), NULL, &pUAV);
		assert(SUCCEEDED(hr));
	}

	void TerrainEditor::UpdateSurfaceWeightsComputeOutputBuffer(Terrain* pNewTerrain)
	{
		if (pComputeOutBufferSurfaceWeights)
		{
			SafeRelease(pComputeOutBufferSurfaceWeights);
		}

		const uint resolution = pNewTerrain->GetTerrainMesh()->GetSurfaceWeightsTextureResolution();

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = resolution;
		desc.Height = resolution;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		HRESULT hr = Engine::GetDevice()->CreateTexture2D(&desc, nullptr, &pComputeOutBufferSurfaceWeights);
		assert(SUCCEEDED(hr));
	}

	void TerrainEditor::UpdateSurfaceWeightsUAV(Terrain* pNewTerrain)
	{
		if (pSurfaceWeightsUAV)
		{
			SafeRelease(pSurfaceWeightsUAV);
		}

		HRESULT hr = Engine::GetDevice()->CreateUnorderedAccessView(pNewTerrain->GetTerrainMesh()->GetSurfaceWeightsTexture(), NULL, &pSurfaceWeightsUAV);
		assert(SUCCEEDED(hr));
	}
}