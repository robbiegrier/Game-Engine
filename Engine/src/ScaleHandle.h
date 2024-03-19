#ifndef _ScaleHandle
#define _ScaleHandle

#include "EditorVisual.h"

namespace Azul
{
	// Draws a boxy axis line that can be dragged to scale the object along the handle's axis
	class ScaleHandle : public EditorVisual
	{
	public:
		// Big four
		ScaleHandle();
		ScaleHandle(const ScaleHandle&) = delete;
		ScaleHandle& operator=(const ScaleHandle&) = delete;
		virtual ~ScaleHandle();

		// Editor Visual Contract
		virtual void OnDragEnter() override;
		virtual void OnDragExit() override;
		virtual void OnDragUpdate() override;
		virtual float GetMouseOverDistance() const override;
		virtual bool IsMouseOver(float tolerance) const override;

	private:
		Vec3 prevVal;
		float objectTrueStartSize;

		Vec3 startScale;
		Vec3 endScale;

	private:
		constexpr static float MIN_SCALE_CLAMP = 0.01f;
	};
}

#endif