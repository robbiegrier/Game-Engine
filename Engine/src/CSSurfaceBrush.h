#ifndef _CSSurfaceBrush
#define _CSSurfaceBrush

#include "ComputeRSVBuffer.h"
#include "ComputeUAVBuffer.h"
#include "MathEngine.h"
#include "TerrainUtils.h"
#include "TerrainArea.h"
#include "AnimTimer.h"
#include "ComputeShaderBrush.h"

namespace Azul
{
	// Compute shader brush type for editing textures as surface weights.
	class CSSurfaceBrush : public ComputeShaderBrush
	{
	public:
		// Big four
		explicit CSSurfaceBrush(const void* pComputeShader, uint bytes);
		CSSurfaceBrush(const CSSurfaceBrush&) = delete;
		CSSurfaceBrush& operator=(const CSSurfaceBrush&) = delete;
		virtual ~CSSurfaceBrush();

		// Brush contract
		virtual PaintResult Paint(float radius, const Vec3& center, float force) override;
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;
		virtual void Update() override;
		virtual void UpdateGui() override {}

	protected:
		// Derived classes can add opening behavior, such as binding buffers
		virtual void Open() {}

	private:
		static uint GetDispatchDimension(uint in);
		void InitializeComputeShader(const void* pShaderCode, uint bytes);

		ID3D11ComputeShader* pComputeShader;
		ID3D11Buffer* pConstBuff;
		ID3D11UnorderedAccessView* pUAVNull = nullptr;
		TerrainArea workArea;
		AnimTimer flightTimer;

	public:
		static constexpr uint NUM_THREADS = 32u;
	};
}

#endif