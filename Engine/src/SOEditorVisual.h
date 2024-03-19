#ifndef _SOEditorVisual
#define _SOEditorVisual

#include "SOFlatTexture.h"

namespace Azul
{
	class SOEditorVisual : public SOFlatTexture
	{
	public:
		SOEditorVisual();
		SOEditorVisual(const SOEditorVisual&) = delete;
		SOEditorVisual& operator = (const SOEditorVisual&) = delete;
		~SOEditorVisual() = default;

	protected:
		virtual void OnOpen(GraphicsObject* pObject) override;
		virtual void OnClose() override;
	};
}

#endif
