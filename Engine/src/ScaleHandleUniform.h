#ifndef _ScaleHandleUniform
#define _ScaleHandleUniform

#include "EditorVisual.h"

namespace Azul
{
	// Draws a box that can be dragged to scale the object on all axes
	class ScaleHandleUniform : public EditorVisual
	{
	public:
		// Big four
		ScaleHandleUniform();
		ScaleHandleUniform(const ScaleHandleUniform&) = delete;
		ScaleHandleUniform& operator=(const ScaleHandleUniform&) = delete;
		virtual ~ScaleHandleUniform();

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