#ifndef _TerrainBrushRaise
#define _TerrainBrushRaise

#include "CSTerrainBrush.h"

namespace Azul
{
	// Basic sculpt brush to raise and lower the terrain.
	class TerrainBrushRaise : public CSTerrainBrush
	{
	public:
		// Big four
		TerrainBrushRaise();
		TerrainBrushRaise(const CSTerrainBrush&) = delete;
		TerrainBrushRaise& operator=(const TerrainBrushRaise&) = delete;
		virtual ~TerrainBrushRaise() = default;

		// Brush overrides
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;
		virtual void UpdateGui() override;
	};
}

#endif