#ifndef _GOFlatTexture
#define _GOFlatTexture

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{
	class GOFlatTexture :public GraphicsObject
	{
	public:
		GOFlatTexture(Mesh* mesh, ShaderObject* pShaderObj, TextureObject* pTex);
		GOFlatTexture() = delete;
		GOFlatTexture(const GOFlatTexture&) = delete;
		GOFlatTexture& operator = (const GOFlatTexture&) = delete;
		virtual ~GOFlatTexture() = default;

		virtual void ActivateGraphics() override;
		virtual void Draw() override;
		virtual GraphicsObject* Clone() override;

		virtual TextureObject* GetTexture() override;

		TextureObject* pTex;
	};
}

#endif
