#ifndef _GOTerrain
#define _GOTerrain

#include "GraphicsObject.h"
#include "TextureObject.h"
#include "TerrainUtils.h"
#include "TerrainMesh.h"

namespace Azul
{
	class GOTerrain :public GraphicsObject
	{
	public:
		GOTerrain(Mesh* pInMesh, ShaderObject* pInShaderObj);
		GOTerrain() = delete;
		GOTerrain(const GOTerrain&) = delete;
		GOTerrain& operator = (const GOTerrain&) = delete;
		virtual ~GOTerrain();

		virtual void ActivateGraphics() override;
		virtual void Draw() override;
		virtual GraphicsObject* Clone() override;

		virtual TextureObject* GetTexture() override;

		//TextureObject* pTex;
		ID3D11SamplerState* pSplattingSampler;
		ID3D11SamplerState* pNormSampler;
	};
}

#endif