#ifndef _RotateHandle
#define _RotateHandle

#include "EditorVisual.h"

namespace Azul
{
	// Draws a ring around the game object which rotates the object around the ring's normal when dragged
	class RotateHandle : public EditorVisual
	{
	public:
		// Big four
		RotateHandle();
		RotateHandle(const RotateHandle&) = delete;
		RotateHandle& operator=(const RotateHandle&) = delete;
		virtual ~RotateHandle();

		// Editor Visual Contract
		virtual void OnDragEnter() override;
		virtual void OnDragExit() override;
		virtual void OnDragUpdate() override;
		virtual float GetMouseOverDistance() const override;
		virtual bool IsMouseOver(float tolerance) const override;

	private:
		Vec3 startDragPoint;
		Vec3 prevTangentPoint;
		Vec3 prevDragPoint;
		Quat totalDelta;

	private:
		constexpr static float TOLERANCE_MODIFIER = 5.f;
		constexpr static float CAMERA_DIST_MODIFIER = 0.115f;
		constexpr static float DRAG_SCALAR_MODIFIER = 0.1f;
	};
}

#endif