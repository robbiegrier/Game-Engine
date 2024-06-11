#ifndef _EditActionTerrain
#define _EditActionTerrain

#include "EditAction.h"
#include "MathEngine.h"
#include "TerrainUtils.h"

namespace Azul
{
	class TerrainMesh;
	class Terrain;

	struct TerrainState
	{
		float* pHeightfield;
		float* pWeights;
	};

	class EditActionTerrain : public EditAction
	{
	public:
		// Big four
		EditActionTerrain() = default;
		EditActionTerrain(const EditActionTerrain&) = delete;
		EditActionTerrain& operator=(const EditActionTerrain&) = delete;
		virtual ~EditActionTerrain() = default;

		// Action contract
		virtual void Execute() override;
		virtual void Undo() override;
		virtual void Dump() override;

		void SetTerrain(const TerrainState& pTo, const TerrainState& pFrom);

		uint index;

		bool valid = true;
	};

	class TerrainHistory
	{
	public:
		TerrainHistory();
		~TerrainHistory();

		void OnTerrainChanged(Terrain* pInTerrain);

		void OnTerrainEdit(Terrain* pInTerrain);

		const TerrainState& GetStateAt(uint index) const { return pRing[index]; }
		const TerrainState& GetPrevStateAt(uint index) const { return pRing[(index == 0u ? DEPTH - 1u : index - 1u)]; }
		const TerrainState& GetCurrState() const;
		const TerrainState& GetPrevState() const;
		static constexpr uint DEPTH = 10u;

		void SaveTerrainState(uint index, TerrainMesh* pMesh);

		void Increment() { current = (current + 1u) % DEPTH; }
		void Decrement() { current = (current == 0u ? DEPTH - 1u : current - 1u); }

	private:
		TerrainState pRing[DEPTH];
		EditActionTerrain* pActionRing[DEPTH];

	public:
		uint current = 0u;
	};
}

#endif
