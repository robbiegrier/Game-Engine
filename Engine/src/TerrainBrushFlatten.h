#ifndef _TerrainBrushFlatten
#define _TerrainBrushFlatten

#include "CSTerrainBrush.h"

namespace Azul
{
	// Flattens the terrain to a specific height. Shift click to sample the height.
	class TerrainBrushFlatten : public CSTerrainBrush
	{
	public:
		// Big four
		TerrainBrushFlatten();
		TerrainBrushFlatten(const CSTerrainBrush&) = delete;
		TerrainBrushFlatten& operator=(const TerrainBrushFlatten&) = delete;
		virtual ~TerrainBrushFlatten();

		// brush overrides
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;
		virtual void UpdateGui() override;

	protected:
		virtual void Open() override;

	private:
		struct CBTerrainBrushFlatten
		{
			float flattenToHeight;
			float pad0 = 0.f;
			float pad1 = 0.f;
			float pad2 = 0.f;
		};
		ID3D11Buffer* pConstBuffFlatten;

		float flattenToHeight = 0.f;
	};
}

#endif