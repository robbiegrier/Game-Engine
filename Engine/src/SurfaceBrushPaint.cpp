#include "SurfaceBrushPaint.h"
#include "SurfaceBrushPaint.Cx.h"
#include "imgui.h"
#include "Engine.h"
#include "EditorGui.h"
#include "TerrainMesh.h"
#include "Terrain.h"
#include "TextureObject.h"
#include "TerrainSystem.h"

namespace Azul
{
	SurfaceBrushPaint::SurfaceBrushPaint()
		: CSSurfaceBrush(g_SurfaceBrushPaint_ComputeShader, sizeof(g_SurfaceBrushPaint_ComputeShader))
	{
		HRESULT hr;

		D3D11_BUFFER_DESC buffDescConstBuff{ 0 };
		buffDescConstBuff.Usage = D3D11_USAGE_DEFAULT;
		buffDescConstBuff.ByteWidth = sizeof(CBTerrainBrushPaint);
		buffDescConstBuff.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescConstBuff.CPUAccessFlags = 0;
		buffDescConstBuff.MiscFlags = 0;
		buffDescConstBuff.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescConstBuff, nullptr, &pConstBuffPaint);
		assert(SUCCEEDED(hr));

		layerMask = Vec4(1.f, 0.f, 0.f, 0.f);
	}

	SurfaceBrushPaint::~SurfaceBrushPaint()
	{
		SafeRelease(pConstBuffPaint);
	}

	PaintResult SurfaceBrushPaint::ShiftPaint(float radius, const Vec3& center, float force)
	{
		return Paint(radius, center, -force);
	}

	void SurfaceBrushPaint::UpdateGui()
	{
		CSSurfaceBrush::UpdateGui();

		static int layerSelected = -1;

		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::CollapsingHeader("Paint Brush", flags))
		{
			ImGui::TextWrapped("Select a surface layer and paint the texture onto the terrain.");

			Terrain* pTerrainObject = TerrainSystem::GetCurrentTerrain();

			for (uint i = 0u; i < TerrainMesh::NUM_SURFACES; i++)
			{
				TextureObject* pSurfaceTex = pTerrainObject->GetTerrainMesh()->GetSurfaceTexture(i);

				std::string imgName = "Layer_Select_" + std::to_string(i);

				if (layerSelected == (int)i)
				{
					if (ImGui::ImageButton(imgName.c_str(), pSurfaceTex->GetResourceView(), ImVec2{ 75.f, 75.f }))
					{
						layerSelected = (int)i;
					}
				}
				else
				{
					if (ImGui::ImageButton(imgName.c_str(), pSurfaceTex->GetResourceView(), { 60.f, 60.f }))
					{
						layerSelected = (int)i;
					}
				}

				ImGui::SameLine();
			}
		}
		ImGui::NewLine();
		ImGui::Separator();

		switch (layerSelected)
		{
		case 0: layerMask = Vec4(1.f, 0.f, 0.f, 0.f); break;
		case 1: layerMask = Vec4(0.f, 1.f, 0.f, 0.f); break;
		case 2: layerMask = Vec4(0.f, 0.f, 1.f, 0.f); break;
		case 3: layerMask = Vec4(0.f, 0.f, 0.f, 1.f); break;
		default: layerMask = Vec4(); break;
		}
	}

	void SurfaceBrushPaint::Open()
	{
		CBTerrainBrushPaint constants;
		constants.layerMask = layerMask;
		Engine::GetContext()->UpdateSubresource(pConstBuffPaint, 0u, nullptr, &constants, 0u, 0u);
		Engine::GetContext()->CSSetConstantBuffers(1u, 1u, &pConstBuffPaint);
	}
}