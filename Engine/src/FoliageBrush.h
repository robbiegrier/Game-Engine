#ifndef _FoliageBrush
#define _FoliageBrush

#include "ComputeShaderBrush.h"
#include "TerrainUtils.h"
#include "FoliageType.h"
#include "imgui.h"

namespace Azul
{
	// A brush that paints foliage onto the terrain.
	class FoliageBrush : public ComputeShaderBrush
	{
	public:
		// Big four
		explicit FoliageBrush();
		FoliageBrush(const FoliageBrush&) = delete;
		FoliageBrush& operator=(const FoliageBrush&) = delete;
		virtual ~FoliageBrush();

		// Brush contract
		virtual PaintResult Paint(float radius, const Vec3& center, float force) override;
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;
		virtual void Update() override;
		virtual void UpdateGui() override;

	private:
		float paintInterval = .2f;
		float deltaTimeTracker = paintInterval;

		bool selection[20];
		int selectedFoliage = -1;

		std::vector<FoliageType::Name> foliageTypeOptions;

		float spawnScaleModifier = 1.f;
		float spawnScaleVariance = 0.0f;
		float tintFactor = 0.0f;
		float tintVariance = 0.0f;

		bool randomRotation = true;
		bool useTintConstant = true;
		ImVec4 tintColorConstant{ 1.f, 1.f, 1.f, 1.f };
	};
}

#endif