#ifndef _GOLightTexture
#define _GOLightTexture

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{
	class GOLightTexture :public GraphicsObject
	{
	public:
		GOLightTexture(Mesh* pInMesh, ShaderObject* pInShaderObj, TextureObject* pInTex);
		GOLightTexture() = delete;
		GOLightTexture(const GOLightTexture&) = delete;
		GOLightTexture& operator = (const GOLightTexture&) = delete;
		virtual ~GOLightTexture();

		virtual void ActivateGraphics() override;
		virtual void Draw() override;
		virtual GraphicsObject* Clone() override;

		virtual TextureObject* GetTexture() override;

		TextureObject* pTex;
	};
}

#endif