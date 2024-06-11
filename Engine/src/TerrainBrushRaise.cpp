#include "TerrainBrushRaise.h"
#include "TerrainBrush.Cx.h"
#include "imgui.h"

namespace Azul
{
	TerrainBrushRaise::TerrainBrushRaise()
		: CSTerrainBrush(g_TerrainBrush_ComputeShader, sizeof(g_TerrainBrush_ComputeShader))
	{
	}

	PaintResult TerrainBrushRaise::ShiftPaint(float radius, const Vec3& center, float force)
	{
		return Paint(radius, center, -force);
	}

	void TerrainBrushRaise::UpdateGui()
	{
		CSTerrainBrush::UpdateGui();

		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::CollapsingHeader("Raise Brush", flags))
		{
			ImGui::TextWrapped("Raise the terrain by clicking somewhere. Lower the terrian by shift-clicking.");
		}
	}
}