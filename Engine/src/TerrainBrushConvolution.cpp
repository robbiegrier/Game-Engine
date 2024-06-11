#include "TerrainBrushConvolution.h"
#include "TerrainBrushConvolution.Cx.h"
#include "imgui.h"
#include "Engine.h"

namespace Azul
{
	TerrainBrushConvolution::TerrainBrushConvolution()
		: CSTerrainBrush(g_TerrainBrushConvolution_ComputeShader, sizeof(g_TerrainBrushConvolution_ComputeShader)), pConvolutionMatrix(nullptr), pMatrixData(nullptr)
	{
		SetConvolutionMatrix(MakeAverageBlurFilter());
	}

	TerrainBrushConvolution::TerrainBrushConvolution(float* pInMatrix)
		: CSTerrainBrush(g_TerrainBrushConvolution_ComputeShader, sizeof(g_TerrainBrushConvolution_ComputeShader)), pConvolutionMatrix(nullptr), pMatrixData(nullptr)
	{
		SetConvolutionMatrix(pInMatrix);
	}

	TerrainBrushConvolution::~TerrainBrushConvolution()
	{
		if (pConvolutionMatrix)
		{
			SafeRelease(pConvolutionMatrix);
			SafeRelease(pConvolutionUAV);
			delete[] pMatrixData;
		}
	}

	void TerrainBrushConvolution::UpdateGui()
	{
		CSTerrainBrush::UpdateGui();

		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::CollapsingHeader("Convolution Brush", flags))
		{
			ImGui::TextWrapped("Apply the filter by clicking somewhere.");
		}
	}

	void TerrainBrushConvolution::SetConvolutionMatrix(float* pInMatrix)
	{
		if (pConvolutionMatrix)
		{
			SafeRelease(pConvolutionMatrix);
			SafeRelease(pConvolutionUAV);
			delete[] pMatrixData;
		}

		HRESULT hr;
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = CONVOLUTION_RESOLUTION;
		desc.Height = CONVOLUTION_RESOLUTION;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = pInMatrix;
		initData.SysMemPitch = sizeof(float) * CONVOLUTION_RESOLUTION;

		hr = Engine::GetDevice()->CreateTexture2D(&desc, &initData, &pConvolutionMatrix);
		assert(SUCCEEDED(hr));

		hr = Engine::GetDevice()->CreateUnorderedAccessView(pConvolutionMatrix, NULL, &pConvolutionUAV);
		assert(SUCCEEDED(hr));

		pMatrixData = pInMatrix;
	}

	float* TerrainBrushConvolution::MakeAverageBlurFilter()
	{
		float* pFilter = new float[CONVOLUTION_SIZE];

		float normalizedVal = 1.f / (float)CONVOLUTION_SIZE;

		for (uint i = 0; i < CONVOLUTION_SIZE; i++)
		{
			pFilter[i] = normalizedVal;
		}

		return pFilter;
	}

	float* TerrainBrushConvolution::MakeGaussianBlurFilter(float stdDev)
	{
		float* pFilter = new float[CONVOLUTION_SIZE];

		for (uint i = 0; i < CONVOLUTION_SIZE; i++)
		{
			pFilter[i] = 0.f;
		}

		float grid[CONVOLUTION_RESOLUTION][CONVOLUTION_RESOLUTION];

		const int convResSignedHalf = (int)CONVOLUTION_RESOLUTION / 2;
		const int convResSignedTruncHalf = convResSignedHalf;

		float stdv = stdDev;
		float r, s = 2.0f * stdv * stdv;
		float sum = 0.0f;

		for (int x = -convResSignedTruncHalf; x <= convResSignedTruncHalf; x++)
		{
			for (int y = -convResSignedTruncHalf; y <= convResSignedTruncHalf; y++)
			{
				r = sqrtf((float)x * (float)x + (float)y * (float)y);
				grid[x + convResSignedTruncHalf][y + convResSignedTruncHalf] = (expf(-(r * r) / s)) / (MATH_PI * s);
				sum += grid[x + convResSignedTruncHalf][y + convResSignedTruncHalf];
			}
		}

		for (int i = 0; i < CONVOLUTION_RESOLUTION; ++i)
		{
			for (int j = 0; j < CONVOLUTION_RESOLUTION; ++j)
			{
				pFilter[(j * CONVOLUTION_RESOLUTION) + i] = (grid[i][j]) / sum;
			}
		}

		return pFilter;
	}

	PaintResult TerrainBrushConvolution::ShiftPaint(float radius, const Vec3& center, float force)
	{
		return Paint(radius, center, -force);
	}

	void TerrainBrushConvolution::Open()
	{
		Engine::GetContext()->CSSetUnorderedAccessViews(1u, 1, &pConvolutionUAV, nullptr);
	}
}