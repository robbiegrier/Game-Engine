#include "SurfaceBrushConvolution.h"
#include "SurfaceBrushConvolution.Cx.h"
#include "imgui.h"
#include "Engine.h"
#include "EditorGui.h"
#include "TerrainBrushConvolution.h"

namespace Azul
{
	SurfaceBrushConvolution::SurfaceBrushConvolution()
		: CSSurfaceBrush(g_SurfaceBrushConvolution_ComputeShader, sizeof(g_SurfaceBrushConvolution_ComputeShader))
	{
		SetConvolutionMatrix(TerrainBrushConvolution::MakeAverageBlurFilter());
	}

	SurfaceBrushConvolution::SurfaceBrushConvolution(float* pInMatrix)
		: CSSurfaceBrush(g_SurfaceBrushConvolution_ComputeShader, sizeof(g_SurfaceBrushConvolution_ComputeShader)), pConvolutionMatrix(nullptr), pMatrixData(nullptr)
	{
		SetConvolutionMatrix(pInMatrix);
	}

	SurfaceBrushConvolution::~SurfaceBrushConvolution()
	{
		if (pConvolutionMatrix)
		{
			SafeRelease(pConvolutionMatrix);
			SafeRelease(pConvolutionUAV);
			delete[] pMatrixData;
		}
	}

	PaintResult SurfaceBrushConvolution::ShiftPaint(float radius, const Vec3& center, float force)
	{
		return Paint(radius, center, -force);
	}

	void SurfaceBrushConvolution::UpdateGui()
	{
		CSSurfaceBrush::UpdateGui();

		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::CollapsingHeader("Blur Brush", flags))
		{
			ImGui::TextWrapped("Blur the texture on the terrain.");
		}
	}

	void SurfaceBrushConvolution::SetConvolutionMatrix(float* pInMatrix)
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

	void SurfaceBrushConvolution::Open()
	{
		Engine::GetContext()->CSSetUnorderedAccessViews(1u, 1, &pConvolutionUAV, nullptr);
	}
}