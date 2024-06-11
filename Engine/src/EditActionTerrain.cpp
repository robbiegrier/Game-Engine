#include "EditActionTerrain.h"
#include "CSTerrainBrush.h"
#include "TerrainEditor.h"
#include "EditorGui.h"
#include "TerrainMesh.h"
#include "Terrain.h"
#include "EditorHistory.h"
#include "TerrainSystem.h"
#include "TerrainArea.h"

namespace Azul
{
	void EditActionTerrain::Execute()
	{
		if (!valid) return;

		Trace::out("Edit Action Terrain: Execute\n");

		TerrainHistory& history = EditorGui::GetTerrainEditor().GetHistory();

		const TerrainState& pPrevState = history.GetPrevStateAt(index);
		const TerrainState& pCurrState = history.GetStateAt(index);

		SetTerrain(pCurrState, pPrevState);

		history.Increment();
	}

	void EditActionTerrain::Undo()
	{
		if (!valid) return;

		Trace::out("Edit Action Terrain: Undo\n");

		TerrainHistory& history = EditorGui::GetTerrainEditor().GetHistory();

		const TerrainState& pPrevState = history.GetPrevStateAt(index);
		const TerrainState& pCurrState = history.GetStateAt(index);

		SetTerrain(pPrevState, pCurrState);

		history.Decrement();
	}

	void EditActionTerrain::Dump()
	{
		Trace::out("Edit Action Terrain\n");
	}

	void EditActionTerrain::SetTerrain(const TerrainState& pTo, const TerrainState& pFrom)
	{
		Terrain* pTerrainObject = TerrainSystem::GetCurrentTerrain();
		const uint resolution = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetVertexResolution();
		float* pTerrainTexture = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetHeightfieldTexture();
		memcpy(pTerrainTexture, pTo.pHeightfield, resolution * resolution * sizeof(float));
		TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->PushHeightfieldSlow();

		TerrainArea area{ TerrainCoordinate(0, 0), TerrainCoordinate(resolution - 1, resolution - 1) };

		for (const TerrainCoordinate& coord : area)
		{
			uint gridIndex = pTerrainObject->GetTerrainMesh()->GetGridIndex(coord.x, coord.y);
			const float prevHeight = pFrom.pHeightfield[gridIndex] * pTerrainObject->GetTerrainMesh()->GetHeightScale();
			const float setHeight = pTo.pHeightfield[gridIndex] * pTerrainObject->GetTerrainMesh()->GetHeightScale();

			if (Util::isNotEqual(prevHeight, setHeight, MATH_TOLERANCE))
			{
				Vec3f vec = pTerrainObject->GetTerrainMesh()->GetVertexf(coord.y, coord.x);
				vec.y = setHeight;
				pTerrainObject->GetTerrainMesh()->EditVertex(coord.y, coord.x, vec);
				pTerrainObject->GetQuadTree()->OnVertexHeightChanged(coord, setHeight);
			}
		}

		for (const TerrainCoordinate& coord : area)
		{
			pTerrainObject->GetQuadTree()->OnVertexHeightChangedPropogate(coord);
		}

		float* pSurfaceWeights = TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSurfaceWeights();
		memcpy(pSurfaceWeights, pTo.pWeights, TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->GetSurfaceWeightsSize());
		TerrainSystem::GetCurrentTerrain()->GetTerrainMesh()->PushSurfaceWeightsSlow();
	}

	TerrainHistory::TerrainHistory()
	{
		for (uint i = 0; i < DEPTH; i++)
		{
			pRing[i] = TerrainState();
			pActionRing[i] = nullptr;
		}

		Trace::out("Intialized empty terrain ring\n");
	}

	TerrainHistory::~TerrainHistory()
	{
		for (uint i = 0; i < DEPTH; i++)
		{
			if (pRing[i].pHeightfield)
			{
				delete[] pRing[i].pHeightfield;
				delete[] pRing[i].pWeights;
			}
		}
	}

	void TerrainHistory::OnTerrainChanged(Terrain* pInTerrain)
	{
		for (uint i = 0; i < DEPTH; i++)
		{
			if (pRing[i].pHeightfield)
			{
				delete[] pRing[i].pHeightfield;
				delete[] pRing[i].pWeights;
			}

			pRing[i].pHeightfield = new float[pInTerrain->GetTerrainMesh()->GetTotalVertices()];
			pRing[i].pWeights = new float[pInTerrain->GetTerrainMesh()->GetNumSurfaceWeightFloats()];
		}

		// Save the intial state
		current = 0u;
		SaveTerrainState(current, pInTerrain->GetTerrainMesh());

		Trace::out("Intialized Initial terrain state in ring buffer\n");
	}

	void TerrainHistory::OnTerrainEdit(Terrain* pInTerrain)
	{
		Increment();

		if (pActionRing[current])
		{
			pActionRing[current]->valid = false;
		}

		// Next index can no longer be undone
		uint nextIndex = (current + 1u) % DEPTH;
		if (pActionRing[nextIndex])
		{
			pActionRing[nextIndex]->valid = false;
		}

		SaveTerrainState(current, pInTerrain->GetTerrainMesh());

		EditActionTerrain* pAction = new EditActionTerrain();
		pAction->index = current;
		pAction->Commit();
		pActionRing[current] = pAction;

		Trace::out("Committed terrain command %d\n", current);
	}

	const TerrainState& TerrainHistory::GetCurrState() const
	{
		return pRing[current];
	}

	const TerrainState& TerrainHistory::GetPrevState() const
	{
		return pRing[current == 0u ? (DEPTH - 1u) : (current - 1u)];
	}

	void TerrainHistory::SaveTerrainState(uint index, TerrainMesh* pMesh)
	{
		memcpy(pRing[index].pHeightfield, pMesh->GetHeightfieldTexture(), pMesh->GetHeightfieldTextureSize());
		memcpy(pRing[index].pWeights, pMesh->GetSurfaceWeights(), pMesh->GetSurfaceWeightsSize());
	}
}