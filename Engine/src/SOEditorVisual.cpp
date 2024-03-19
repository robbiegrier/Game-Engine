#include "SOEditorVisual.h"
#include "EditorGui.h"
#include "Viewport.h"

namespace Azul
{
	SOEditorVisual::SOEditorVisual()
		: SOFlatTexture()
	{
	}

	void SOEditorVisual::OnOpen(GraphicsObject* pObject)
	{
		SOFlatTexture::OnOpen(pObject);

		EditorGui::GetWorldViewport()->ToggleDepthTests(false);
	}

	void SOEditorVisual::OnClose()
	{
		SOFlatTexture::OnClose();

		EditorGui::GetWorldViewport()->ToggleDepthTests(true);
	}
}