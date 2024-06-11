#include "CSTerrainBrush.h"
#include "Engine.h"
#include "TerrainBrush.Cx.h"
#include "TerrainSystem.h"
#include "Terrain.h"
#include "TerrainMesh.h"
#include "QuadTree.h"
#include "imgui.h"
#include "EditorGui.h"

namespace Azul
{
	CSTerrainBrush::CSTerrainBrush(const void* pComputeShader, uint bytes)
	{
		InitializeComputeShader(pComputeShader, bytes);

		HRESULT hr;

		D3D11_BUFFER_DESC buffDescConstBuff{ 0 };
		buffDescConstBuff.Usage = D3D11_USAGE_DEFAULT;
		buffDescConstBuff.ByteWidth = sizeof(CBTerrainBrush);
		buffDescConstBuff.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescConstBuff.CPUAccessFlags = 0;
		buffDescConstBuff.MiscFlags = 0;
		buffDescConstBuff.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescConstBuff, nullptr, &pConstBuff);
		assert(SUCCEEDED(hr));
	}

	CSTerrainBrush::~CSTerrainBrush()
	{
		SafeRelease(pComputeShader);
		SafeRelease(pConstBuff);
	}

	PaintResult CSTerrainBrush::Paint(float radius, const Vec3& center, float force)
	{
		if (mapInFlight)
		{
			return PaintResult::Waiting;
		}

		Engine::GetContext()->CSSetShader(pComputeShader, nullptr, 0u);

		TerrainArea area(center, radius);

		CBTerrainBrush constants;
		constants.offset = area.GetMin();
		constants.force = force;
		constants.center = Vec4(center, 1.f);
		constants.radius = radius;
		constants.gridCellSize = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSingleGridCellScale();
		constants.halfWorldScale = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHalfWorldScale();

		Engine::GetContext()->UpdateSubresource(pConstBuff, 0u, nullptr, &constants, 0u, 0u);
		Engine::GetContext()->CSSetConstantBuffers(0u, 1u, &pConstBuff);
		auto pUAV = EditorGui::GetTerrainEditor().GetTerrainUAV();
		Engine::GetContext()->CSSetUnorderedAccessViews(0u, 1, &pUAV, nullptr);

		Open();

		Engine::GetContext()->Dispatch(GetDispatchDimension(area.GetWidth()), GetDispatchDimension(area.GetHeight()), 1u);

		mapInFlight = true;
		mappedArea = area;

		flightTimer.Tic();

		return PaintResult::Success;
	}

	PaintResult CSTerrainBrush::ShiftPaint(float radius, const Vec3& center, float force)
	{
		static_cast<void>(radius);
		static_cast<void>(center);
		static_cast<void>(force);
		// Do Nothing

		return PaintResult::Success;
	}

	void CSTerrainBrush::Update()
	{
		static bool isWait = false;

		if (mapInFlight)
		{
			ID3D11Texture2D* pComputeOutBuffer = EditorGui::GetTerrainEditor().GetComputeOutBuffer();

			if (!isWait)
			{
				Engine::GetContext()->CopyResource(pComputeOutBuffer, TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHeightfieldTextureResource());
			}

			D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
			HRESULT hr;

			hr = Engine::GetContext()->Map(pComputeOutBuffer, 0, D3D11_MAP_READ, D3D11_MAP_FLAG_DO_NOT_WAIT, &mappedResource);

			if (SUCCEEDED(hr))
			{
				assert(mappedResource.pData);
				float* pHeightfieldDownload = (float*)mappedResource.pData;
				float* pTerrainTexture = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHeightfieldTexture();
				const uint resolution = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetVertexResolution();
				memcpy(pTerrainTexture, pHeightfieldDownload, resolution * resolution * sizeof(float));
				Engine::GetContext()->Unmap(pComputeOutBuffer, 0);

				if (workArea.IsEmpty())
				{
					workArea = mappedArea;
				}
				else
				{
					workArea.UnionWith(mappedArea);
				}

				isWait = false;
				mapInFlight = false;
			}
			else
			{
				isWait = true;
			}
		}

		ProcessWorkAreaPerFrame();
	}

	void CSTerrainBrush::ProcessWorkAreaPerFrame()
	{
		if (workArea.IsEmpty())
		{
			return;
		}

		TerrainArea thisFrameArea = workArea;

		while (thisFrameArea.GetHeight() * thisFrameArea.GetWidth() > 20'000u && thisFrameArea.GetHeight() > 0u)
		{
			thisFrameArea = thisFrameArea.GetBisectionHorizontal();
		}

		Terrain* pTerrainObject = TerrainSystem::GetCurrentTerrain();
		float* pTerrainTexture = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHeightfieldTexture();

		for (const TerrainCoordinate& coordinate : thisFrameArea)
		{
			if (coordinate.x < pTerrainObject->GetTerrainMesh()->GetVertexResolution() && coordinate.y < pTerrainObject->GetTerrainMesh()->GetVertexResolution())
			{
				const Vec3f& prevVertex = pTerrainObject->GetTerrainMesh()->GetVertexf(coordinate.y, coordinate.x);
				uint gridIndex = pTerrainObject->GetTerrainMesh()->GetGridIndex(coordinate.x, coordinate.y);
				float downloadedHeight = pTerrainTexture[gridIndex] * pTerrainObject->GetTerrainMesh()->GetHeightScale();

				Vec3f newVertex = prevVertex;
				newVertex.y = downloadedHeight;

				pTerrainObject->GetTerrainMesh()->EditVertex(coordinate.y, coordinate.x, newVertex);
				pTerrainObject->GetQuadTree()->OnVertexHeightChanged(coordinate, newVertex.y);
			}
		}

		for (const TerrainCoordinate& coordinate : thisFrameArea)
		{
			pTerrainObject->GetQuadTree()->OnVertexHeightChangedPropogate(coordinate);
		}

		workArea = workArea.GetDifferenceSouthWise(thisFrameArea);
	}

	uint CSTerrainBrush::GetDispatchDimension(uint in)
	{
		uint out = in / NUM_THREADS;

		const bool cleanDiv = in % NUM_THREADS == 0u;
		if (!cleanDiv)
		{
			out += 1u;
		}

		return out;
	}

	void CSTerrainBrush::UpdateGuiDebug()
	{
		if (ImGui::CollapsingHeader("Debug Brush Info"))
		{
			if (mapInFlight)
			{
				AnimTime time = flightTimer.Toc();

				ImGui::Text("GPU download waiting... %dms %dus", (int)AnimTime::Quotient(time, AnimTime(AnimTime::Duration::ONE_MILLISECOND)),
					(int)AnimTime::Quotient(AnimTime::Remainder(time, AnimTime(AnimTime::Duration::ONE_MILLISECOND)), AnimTime(AnimTime::Duration::ONE_MICROSECOND)));
			}
			else
			{
				ImGui::Text("Heightfield downloaded.");
			}

			if (!workArea.IsEmpty())
			{
				ImGui::Text("Vertex updates queued: %d\n", workArea.GetWidth() * workArea.GetHeight());
			}
			else
			{
				ImGui::Text("Vertices up to date.");
			}
		}
	}

	void CSTerrainBrush::InitializeComputeShader(const void* pShaderCode, uint bytes)
	{
		HRESULT hr;
		hr = Engine::GetDevice()->CreateComputeShader(pShaderCode, bytes, nullptr, &pComputeShader);
		assert(SUCCEEDED(hr));
		assert(pComputeShader);
	}
}