#ifndef _GOLightTexture
#define _GOLightTexture

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{
	class GOLightTexture :public GraphicsObject
	{
	public:
		GOLightTexture(Mesh* mesh, ShaderObject* pShaderObj, TextureObject* pTex, const Vec3& _pLightColor, const Vec3& _pLightPos);
		GOLightTexture() = delete;
		GOLightTexture(const GOLightTexture&) = delete;
		GOLightTexture& operator = (const GOLightTexture&) = delete;
		virtual ~GOLightTexture();

		virtual void ActivateGraphics() override;
		virtual void Draw() override;

		TextureObject* pTex;
		Vec3* poLightColor;
		Vec3* poLightPos;
	};
}

#endif