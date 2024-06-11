#include "TerrainBrushFlatten.h"
#include "Engine.h"
#include "TerrainBrushFlatten.Cx.h"
#include "TerrainSystem.h"
#include "Terrain.h"
#include "TerrainMesh.h"
#include "imgui.h"

namespace Azul
{
	TerrainBrushFlatten::TerrainBrushFlatten()
		: CSTerrainBrush(g_TerrainBrushFlatten_ComputeShader, sizeof(g_TerrainBrushFlatten_ComputeShader))
	{
		HRESULT hr;

		D3D11_BUFFER_DESC buffDescConstBuff{ 0 };
		buffDescConstBuff.Usage = D3D11_USAGE_DEFAULT;
		buffDescConstBuff.ByteWidth = sizeof(CBTerrainBrushFlatten);
		buffDescConstBuff.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescConstBuff.CPUAccessFlags = 0;
		buffDescConstBuff.MiscFlags = 0;
		buffDescConstBuff.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescConstBuff, nullptr, &pConstBuffFlatten);
		assert(SUCCEEDED(hr));
	}

	TerrainBrushFlatten::~TerrainBrushFlatten()
	{
		SafeRelease(pConstBuffFlatten);
	}

	PaintResult TerrainBrushFlatten::ShiftPaint(float radius, const Vec3& center, float force)
	{
		static_cast<void>(radius);
		static_cast<void>(force);

		const float worldHeight = TerrainSystem::GetCurrentTerrain()->ComputeBarycentricHeight(center);
		flattenToHeight = worldHeight / TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHeightScale();

		return PaintResult::Success;
	}

	void TerrainBrushFlatten::UpdateGui()
	{
		CSTerrainBrush::UpdateGui();

		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::CollapsingHeader("Flatten Brush", flags))
		{
			ImGui::TextWrapped("Set the flattening height by shift clicking somewhere on the terrain.");

			float val = flattenToHeight * TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHeightScale();
			ImGui::DragFloat("Flattening Height", &val);
		}
	}

	void TerrainBrushFlatten::Open()
	{
		CBTerrainBrushFlatten flattenArgs;
		flattenArgs.flattenToHeight = flattenToHeight;
		Engine::GetContext()->UpdateSubresource(pConstBuffFlatten, 0u, nullptr, &flattenArgs, 0u, 0u);
		Engine::GetContext()->CSSetConstantBuffers(1u, 1u, &pConstBuffFlatten);
	}
}