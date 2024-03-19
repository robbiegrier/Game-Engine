#include "EditorGuiUtils.h"

namespace Azul
{
	void EditorGuiUtils::AlignForWidth(float width, float alignment)
	{
		float avail = ImGui::GetContentRegionAvail().x;
		float off = (avail - width) * alignment;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

	void EditorGuiUtils::AlignForHeight(float height, float alignment)
	{
		float avail = ImGui::GetContentRegionAvail().y;
		float off = (avail - height) * alignment;
		if (off > 0.0f)
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off);
	}
}