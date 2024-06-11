#include "TerrainBrushNoise.h"
#include "TerrainBrushNoise.Cx.h"
#include "imgui.h"
#include "Engine.h"
#include <algorithm>

namespace Azul
{
#define LERP(t, a, b) ((a) + (t)*((b)-(a)))

	// https://github.com/stegu/perlin-noise/tree/master

	float grad2(int hash, float x, float y)
	{
		int h = hash & 7; // Convert low 3 bits of hash code
		float u = h < 4 ? x : y; // into 8 simple gradient directions,
		float v = h < 4 ? y : x; // and compute the dot product with (x,y).
		return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
	}

#define FADE(t) ( t * t * t * ( t * ( t * 6 - 15 ) + 10 ) )

	float noise2(float x, float y, uint* perm)
	{
		int ix0, iy0, ix1, iy1;
		float fx0, fy0, fx1, fy1;
		float s, t, nx0, nx1, n0, n1;

		ix0 = (int)floor(x); // Integer part of x
		iy0 = (int)floor(y); // Integer part of y
		fx0 = x - ix0; // Fractional part of x
		fy0 = y - iy0; // Fractional part of y
		fx1 = fx0 - 1.0f;
		fy1 = fy0 - 1.0f;
		ix1 = (ix0 + 1) & 0xff; // Wrap to 0..255
		iy1 = (iy0 + 1) & 0xff;
		ix0 = ix0 & 0xff;
		iy0 = iy0 & 0xff;

		t = FADE(fy0);
		s = FADE(fx0);

		nx0 = grad2(perm[ix0 + perm[iy0]], fx0, fy0);
		nx1 = grad2(perm[ix0 + perm[iy1]], fx0, fy1);
		n0 = LERP(t, nx0, nx1);

		nx0 = grad2(perm[ix1 + perm[iy0]], fx1, fy0);
		nx1 = grad2(perm[ix1 + perm[iy1]], fx1, fy1);
		n1 = LERP(t, nx0, nx1);

		return 0.507f * (LERP(s, n0, n1));
	}

	TerrainBrushNoise::TerrainBrushNoise()
		: CSTerrainBrush(g_TerrainBrushNoise_ComputeShader, sizeof(g_TerrainBrushNoise_ComputeShader))
	{
		HRESULT hr;

		D3D11_BUFFER_DESC buffDescConstBuff{ 0 };
		buffDescConstBuff.Usage = D3D11_USAGE_DEFAULT;
		buffDescConstBuff.ByteWidth = sizeof(CBTerrainBrushNoise);
		buffDescConstBuff.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescConstBuff.CPUAccessFlags = 0;
		buffDescConstBuff.MiscFlags = 0;
		buffDescConstBuff.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescConstBuff, nullptr, &pConstBuffNoise);
		assert(SUCCEEDED(hr));

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0.f;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = Engine::GetDevice()->CreateSamplerState(&sampDesc, &pSampler);
		assert(SUCCEEDED(hr));

		SetMatrix(MakePerlinNoise());
	}

	TerrainBrushNoise::~TerrainBrushNoise()
	{
		SafeRelease(pSampler);
		SafeRelease(pShaderResourceView);
		SafeRelease(pMatrix);
		SafeRelease(pConstBuffNoise);

		delete[] pMatrixData;
	}

	PaintResult TerrainBrushNoise::ShiftPaint(float radius, const Vec3& center, float force)
	{
		return Paint(radius, center, -force);
	}

	void TerrainBrushNoise::UpdateGui()
	{
		CSTerrainBrush::UpdateGui();

		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::CollapsingHeader("Noise Brush", flags))
		{
			ImGui::TextWrapped("Add noise to the terrain by clicking somewhere.");

			ImGui::DragFloat("Noise Scale", &noiseScale);
			ImGui::DragInt("Octives", &octives);
			ImGui::DragFloat("Amplitude", &amplitude);
			ImGui::DragFloat("Frequency", &frequency);
			ImGui::DragFloat("Lacunarity", &lacunarity);
			ImGui::DragFloat("Persistence", &persistence);

			if (ImGui::Button("Randomize"))
			{
				SetMatrix(MakePerlinNoise());
			}

			if (!randomizedThisFrame)
			{
				ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
				ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

				float imgSize = contentMax.x - contentMin.x;

				ImGui::Image(
					GetView(),
					ImVec2((float)imgSize, (float)imgSize),
					ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
					ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
					ImVec4(0.f, 0.f, 0.f, 0.f)
				);
			}

			randomizedThisFrame = false;
		}
	}

	void TerrainBrushNoise::SetMatrix(float* pInMatrix)
	{
		if (pMatrix)
		{
			SafeRelease(pMatrix);
			SafeRelease(pShaderResourceView);
			delete[] pMatrixData;

			pMatrix = nullptr;
			pShaderResourceView = nullptr;
			pMatrixData = nullptr;
		}

		HRESULT hr;
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = NOISE_RESOLUTION;
		desc.Height = NOISE_RESOLUTION;
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
		initData.SysMemPitch = sizeof(float) * NOISE_RESOLUTION;

		hr = Engine::GetDevice()->CreateTexture2D(&desc, &initData, &pMatrix);
		assert(SUCCEEDED(hr));

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = desc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = Engine::GetDevice()->CreateShaderResourceView(pMatrix, &shaderResourceViewDesc, &pShaderResourceView);
		assert(SUCCEEDED(hr));

		pMatrixData = pInMatrix;

		randomizedThisFrame = true;
	}

	static uint perm[] =
	{
		151, 160, 137, 91, 90, 15,
	  131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
	  190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
	  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
	  77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
	  102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
	  135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
	  5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
	  223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
	  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
	  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
	  49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	  138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
	  151, 160, 137, 91, 90, 15,
	  131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
	  190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
	  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
	  77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
	  102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
	  135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
	  5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
	  223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
	  129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
	  251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
	  49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	  138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};

	float* TerrainBrushNoise::MakePerlinNoise()
	{
		std::random_shuffle(std::begin(perm), std::end(perm));

		float* pNoise = new float[NOISE_SIZE];

		for (uint i = 0; i < NOISE_RESOLUTION; i++)
		{
			for (uint j = 0; j < NOISE_RESOLUTION; j++)
			{
				float elevation = amplitude;
				float t_frequency = frequency;
				float t_amplitude = amplitude;

				for (int k = 0; k < octives; k++)
				{
					float sample_x = j * t_frequency;
					float sample_y = i * t_frequency;

					elevation += noise2(sample_x, sample_y, perm) * t_amplitude;
					t_frequency *= lacunarity;
					t_amplitude *= persistence;
				}

				elevation = std::max(std::min(elevation, maxHeight), minHeight);
				pNoise[(j * NOISE_RESOLUTION) + i] = elevation;
			}
		}

		return pNoise;
	}

	float* TerrainBrushNoise::MakeErosiveNoise()
	{
		std::random_shuffle(std::begin(perm), std::end(perm));

		float* pNoise = new float[NOISE_SIZE];

		for (uint i = 0; i < NOISE_RESOLUTION; i++)
		{
			for (uint j = 0; j < NOISE_RESOLUTION; j++)
			{
				float elevation = amplitude;
				float t_frequency = frequency;
				float t_amplitude = amplitude;

				float dx = 0.f;
				float dy = 0.f;
				float s = 1.f;

				float alpha = 1.f;

				for (int k = 0; k < octives; k++)
				{
					float sample_x = (j + dx) * t_frequency;
					float sample_y = (i + dy) * t_frequency;

					float sampleIn = s * (1.f - fabsf(noise2(sample_x, sample_y, perm)));
					elevation += (t_amplitude * sampleIn);

					dx += t_amplitude * (alpha * s * noise2(sample_x, sample_y, perm));
					dy += t_amplitude * (alpha * s * noise2(sample_x, sample_y, perm));

					s = s * std::min(1.f, std::max(0.f, elevation));

					//elevation += noise2(sample_x, sample_y, perm) * t_amplitude;

					t_frequency *= lacunarity;
					t_amplitude *= persistence;
				}

				//Trace::out("%f\n", elevation);
				elevation = std::max(std::min(elevation - 1.f, maxHeight), minHeight);
				pNoise[(j * NOISE_RESOLUTION) + i] = elevation;
			}
		}

		return pNoise;
	}

	void TerrainBrushNoise::Open()
	{
		Engine::GetContext()->CSSetShaderResources(0u, 1, &pShaderResourceView);
		Engine::GetContext()->CSSetSamplers(0u, 1, &pSampler);

		CBTerrainBrushNoise constants;
		constants.noiseScale = noiseScale;
		Engine::GetContext()->UpdateSubresource(pConstBuffNoise, 0u, nullptr, &constants, 0u, 0u);
		Engine::GetContext()->CSSetConstantBuffers(1u, 1u, &pConstBuffNoise);
	}
}