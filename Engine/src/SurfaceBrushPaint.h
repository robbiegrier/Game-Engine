#ifndef _SurfaceBrushPaint
#define _SurfaceBrushPaint

#include "CSSurfaceBrush.h"

namespace Azul
{
	// Edits the surface weights based on a layer mask. Usually only one channel of the mask
	// is used so that a specific texture is being painted.
	class SurfaceBrushPaint : public CSSurfaceBrush, public Align16
	{
	public:
		// Big four
		SurfaceBrushPaint();
		SurfaceBrushPaint(const SurfaceBrushPaint&) = delete;
		SurfaceBrushPaint& operator=(const SurfaceBrushPaint&) = delete;
		virtual ~SurfaceBrushPaint();

		// brush overrides
		virtual PaintResult ShiftPaint(float radius, const Vec3& center, float force) override;
		virtual void UpdateGui() override;

	protected:
		virtual void Open() override;

	private:
		struct CBTerrainBrushPaint
		{
			Vec4 layerMask;
		};
		ID3D11Buffer* pConstBuffPaint;

		Vec4 layerMask;
	};
}

#endif
