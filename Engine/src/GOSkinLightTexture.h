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
		virtual ~GOSkinLightTexture() = default;

		virtual void ActivateGraphics() override;
		virtual void Draw() override;

		virtual GraphicsObject* Clone() override;

		TextureObject* pTex;

		Mat4* pBoneWorld;
	};
}

#endif
