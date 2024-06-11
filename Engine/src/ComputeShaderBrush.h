#ifndef _ComputeShaderBrush
#define _ComputeShaderBrush

#include "MathEngine.h"
#include "TerrainUtils.h"

namespace Azul
{
	// Paint result. Success if the edit was queued, waiting if another edit is still in progress.
	enum class PaintResult
	{
		Success,
		Waiting
	};

	// Base class for all brushes. Provides painting contract for using a brush.
	class ComputeShaderBrush
	{
	public:
		virtual ~ComputeShaderBrush();

		// The main paint method. Implements the primary behavior of the brush.
		virtual PaintResult Paint(float radius, const Vec3& center, float force) = 0;

		// The auxillary paint method. Implements inverse or other useful secondary behavior.
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) = 0;

		// Update the brush when it is active.
		virtual void Update() = 0;

		// Update the GUI for this brush. Appears in the terrain window when the brush is active.
		virtual void UpdateGui() = 0;

		// Common method to test map flag. Indicates whether an edit is in progress.
		bool IsMapInFlight() const { return mapInFlight; }

	protected:
		// Set when an async edit is in progress
		bool mapInFlight = false;
	};

	// Common constant buffer definition for all compute shader brushes
	struct CBTerrainBrush
	{
		TerrainCoordinate offset;
		float force = .5f;
		float radius = 32.f;
		Vec4 center;
		float gridCellSize;
		float halfWorldScale;
		float pad1 = 0.f;
		float pad2 = 0.f;
	};
}

#endif