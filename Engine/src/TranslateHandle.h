#ifndef _TranslateHandle
#define _TranslateHandle

#include "EditorVisual.h"

namespace Azul
{
	// Draws an arrow axis line that can be dragged to translate the object along the handle's axis
	class TranslateHandle : public EditorVisual
	{
	public:
		// Big four
		TranslateHandle();
		TranslateHandle(const TranslateHandle&) = delete;
		TranslateHandle& operator=(const TranslateHandle&) = delete;
		virtual ~TranslateHandle();

		// Editor Visual Contract
		virtual void OnDragEnter() override;
		virtual void OnDragExit() override;
		virtual void OnDragUpdate() override;
		virtual float GetMouseOverDistance() const override;
		virtual bool IsMouseOver(float tolerance) const override;

	private:
		Vec3 prevVal;
	};
}

#endif