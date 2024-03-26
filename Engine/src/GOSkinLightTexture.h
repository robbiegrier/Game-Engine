#ifndef _GOSkinLightTexture
#define _GOSkinLightTexture

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{
	class GOSkinLightTexture :public GraphicsObject
	{
	public:
		GOSkinLightTexture(Mesh* mesh, ShaderObject* pShaderObj, TextureObject* pTex);
		GOSkinLightTexture() = delete;
		GOSkinLightTexture(const GOSkinLightTexture&) = delete;
		GOSkinLightTexture& operator = (const GOSkinLightTexture&) = delete;
		virtual ~GOSkinLightTexture();

		virtual void ActivateGraphics() override;
		virtual void Draw() override;

		virtual GraphicsObject* Clone() override;

		virtual TextureObject* GetTexture() override { return pTex; }

		TextureObject* pTex;

		Mat4* pBoneWorld;
	};
}

#endif
