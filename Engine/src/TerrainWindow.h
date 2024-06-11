#ifndef _TerrainWindow
#define _TerrainWindow

#include "QuadTree.h"
#include "CSTerrainBrush.h"
#include "TextureObject.h"

namespace Azul
{
	class EditActionTerrain;

	// Shows the terrian editing GUI
	class TerrainWindow
	{
	public:
		// Big four
		TerrainWindow();
		TerrainWindow(const TerrainWindow&) = delete;
		TerrainWindow& operator=(const TerrainWindow&) = delete;
		~TerrainWindow();

		void Update();
		void Draw();

	private:
		void UpdateBrush();
		void UpdateTextures();
		void UpdateHeighfieldImage();
		void UpdateTerrainSelector();

		float tilingCache = 10.f;
		bool freeze = true;
		std::vector<QTHit> raycastAll;

		char inputTerrainName[32];
		int inputVertexResolution = 2048;
		float inputWorldScale = 1000.f;
		float inputHeightScale = 500.f;
		char inputHeightmapName[64];

		float brushSize = 10.f;
		float brushStrength = 10.f;

		bool clickDown = false;
		bool awaitingActionState = false;
		EditActionTerrain* pAction;

		std::vector<TextureObject::Name> terrainTextureOptions;
	};
}

#endif