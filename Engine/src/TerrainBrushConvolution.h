#ifndef _TerrainBrushConvolution
#define _TerrainBrushConvolution

#include "CSTerrainBrush.h"

namespace Azul
{
	// Applies a blur to the heightfield using a convolution matrix. The matrix can be injected into
	// this class for different effects.
	class TerrainBrushConvolution : public CSTerrainBrush
	{
	public:
		// Big four
		TerrainBrushConvolution();
		explicit TerrainBrushConvolution(float* pInMatrix);
		TerrainBrushConvolution(const CSTerrainBrush&) = delete;
		TerrainBrushConvolution& operator=(const TerrainBrushConvolution&) = delete;
		virtual ~TerrainBrushConvolution();

		// Brush overrides
		virtual void UpdateGui() override;
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;

		// Setting the matrix
		void SetConvolutionMatrix(float* pInMatrix);
		static float* MakeAverageBlurFilter();
		static float* MakeGaussianBlurFilter(float stdDev);

	protected:
		virtual void Open() override;

	private:
		ID3D11Texture2D* pConvolutionMatrix;
		ID3D11UnorderedAccessView* pConvolutionUAV;
		float* pMatrixData;

	public:
		static constexpr uint CONVOLUTION_RESOLUTION = 33u;
		static constexpr uint CONVOLUTION_SIZE = CONVOLUTION_RESOLUTION * CONVOLUTION_RESOLUTION;
	};
}

#endif