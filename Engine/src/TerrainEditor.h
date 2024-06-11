#ifndef _TerrainEditor
#define _TerrainEditor

#include <unordered_map>
#include <d3d11.h>
#include "EditActionTerrain.h"

namespace Azul
{
	class ComputeShaderBrush;
	class Terrain;

	// Contains common resources for terrain editing including the GPU brushes and
	// the common shader resources.
	class TerrainEditor
	{
	public:
		// Big four
		TerrainEditor();
		TerrainEditor(const TerrainEditor&) = delete;
		TerrainEditor& operator=(const TerrainEditor&) = delete;
		~TerrainEditor();

		// Add and remove GPU brush types
		void AddBrush(const char* pName, ComputeShaderBrush* pBrush);
		ComputeShaderBrush* GetBrush(const char* pName) const;

		// Update resources when the terrian asset changes
		void OnTerrainChanged(Terrain* pNewTerrain);

		// Get shader resources
		ID3D11Texture2D* GetComputeOutBuffer() const { return pComputeOutBuffer; }
		ID3D11UnorderedAccessView* GetTerrainUAV() const { return pUAV; }
		ID3D11Texture2D* GetSurfaceWeightsComputeOutBuffer() const { return pComputeOutBufferSurfaceWeights; }
		ID3D11UnorderedAccessView* GetSurfaceWeightsUAV() const { return pSurfaceWeightsUAV; }

		// Get and set currently active brush
		void SetBrush(const char* pName);
		ComputeShaderBrush* GetBrush() const { return pCurrentBrush; }

		// Get the history
		TerrainHistory& GetHistory() { return history; }

	private:
		void UpdateComputeOutputBuffer(Terrain* pNewTerrain);
		void UpdateHeightfieldUAV(Terrain* pNewTerrain);
		void UpdateSurfaceWeightsComputeOutputBuffer(Terrain* pNewTerrain);
		void UpdateSurfaceWeightsUAV(Terrain* pNewTerrain);

		// map of named brushes
		std::unordered_map<std::string, ComputeShaderBrush*> brushes;

		// sculpt shader resources
		ID3D11Texture2D* pComputeOutBuffer;
		ID3D11UnorderedAccessView* pUAV;

		// surface paint shader resources
		ID3D11Texture2D* pComputeOutBufferSurfaceWeights;
		ID3D11UnorderedAccessView* pSurfaceWeightsUAV;

		// current brush
		ComputeShaderBrush* pCurrentBrush;

		// history
		TerrainHistory history;
	};
}

#endif