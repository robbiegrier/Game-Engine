#ifndef _TerrainBrushNoise
#define _TerrainBrushNoise

#include "CSTerrainBrush.h"

namespace Azul
{
	// Generates and applies a noise texture as the brush on the terrain.
	class TerrainBrushNoise : public CSTerrainBrush
	{
	public:
		// Big four
		TerrainBrushNoise();
		TerrainBrushNoise(const CSTerrainBrush&) = delete;
		TerrainBrushNoise& operator=(const TerrainBrushNoise&) = delete;
		virtual ~TerrainBrushNoise();

		// Brush overrides
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;
		virtual void UpdateGui() override;

		// Setting the noise texture
		void SetMatrix(float* pInMatrix);
		float* MakePerlinNoise();
		float* MakeErosiveNoise();

		// Get the texture for visualization
		ID3D11ShaderResourceView* GetView() const { return pShaderResourceView; }

	protected:
		virtual void Open() override;

	private:
		ID3D11Texture2D* pMatrix;
		ID3D11ShaderResourceView* pShaderResourceView;
		ID3D11SamplerState* pSampler;
		float* pMatrixData;

		struct CBTerrainBrushNoise
		{
			float noiseScale;
			float pad0 = 0.f;
			float pad1 = 0.f;
			float pad2 = 0.f;
		};
		ID3D11Buffer* pConstBuffNoise;

		float noiseScale = 2048.f;
		bool randomizedThisFrame = false;
		float maxHeight = 1.f;
		float minHeight = 0.f;
		float amplitude = maxHeight / 2.f;
		float frequency = 0.05f;
		int octives = 3;
		float lacunarity = 2.2f;
		float persistence = 0.5f;

	public:
		static constexpr uint NOISE_RESOLUTION = 256u;
		static constexpr uint NOISE_SIZE = NOISE_RESOLUTION * NOISE_RESOLUTION;
	};
}

#endif