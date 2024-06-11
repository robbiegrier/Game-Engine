#ifndef _SurfaceBrushConvolution
#define _SurfaceBrushConvolution

#include "CSSurfaceBrush.h"

namespace Azul
{
	// Applies a blur filter from a convolution matrix to the surface weights. Matrix can be
	// injected into this class for different effects.
	class SurfaceBrushConvolution : public CSSurfaceBrush
	{
	public:
		// Big four
		SurfaceBrushConvolution();
		explicit SurfaceBrushConvolution(float* pInMatrix);
		SurfaceBrushConvolution(const SurfaceBrushConvolution&) = delete;
		SurfaceBrushConvolution& operator=(const SurfaceBrushConvolution&) = delete;
		virtual ~SurfaceBrushConvolution();

		// Brush overrides
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;
		virtual void UpdateGui() override;

		// Set the matrix
		void SetConvolutionMatrix(float* pInMatrix);

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
