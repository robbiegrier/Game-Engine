#ifndef _CSTerrainBrush
#define _CSTerrainBrush

#include "ComputeRSVBuffer.h"
#include "ComputeUAVBuffer.h"
#include "MathEngine.h"
#include "TerrainArea.h"
#include "AnimTimer.h"
#include "ComputeShaderBrush.h"

namespace Azul
{
	// The main type of brush that can sculpt the terrain by editing the heightfield.
	// Derived types shoud provide a compute shader to bind.
	class CSTerrainBrush : public ComputeShaderBrush
	{
	public:
		// Big four
		explicit CSTerrainBrush(const void* pComputeShader, uint bytes);
		CSTerrainBrush(const CSTerrainBrush&) = delete;
		CSTerrainBrush& operator=(const CSTerrainBrush&) = delete;
		virtual ~CSTerrainBrush();

		// Brush contract
		virtual PaintResult Paint(float radius, const Vec3& center, float force) override;
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;
		virtual void Update() override;
		virtual void UpdateGui() override {}

	protected:
		// Derived classes can add opening behavior before painting, such as binding buffers to their shader.
		virtual void Open() {}

	private:
		void InitializeComputeShader(const void* pShaderCode, uint bytes);
		virtual void UpdateGuiDebug();
		void ProcessWorkAreaPerFrame();
		static uint GetDispatchDimension(uint in);

		ID3D11ComputeShader* pComputeShader;
		ID3D11Buffer* pConstBuff;
		ID3D11UnorderedAccessView* pUAVNull = nullptr;
		TerrainArea mappedArea;
		TerrainArea workArea;
		AnimTimer flightTimer;

	public:
		static constexpr uint NUM_THREADS = 32u;
	};
}

#endif