#ifndef _GOSurfaceShape
#define _GOSurfaceShape

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{
	class GOSurfaceShape :public GraphicsObject
	{
	public:
		GOSurfaceShape(Mesh* mesh, ShaderObject* pShaderObj, TextureObject* pTex);
		GOSurfaceShape() = delete;
		GOSurfaceShape(const GOSurfaceShape&) = delete;
		GOSurfaceShape& operator = (const GOSurfaceShape&) = delete;
		virtual ~GOSurfaceShape() = default;

		virtual void ActivateGraphics() override;
		virtual void Draw() override;
		virtual GraphicsObject* Clone() override;

		virtual TextureObject* GetTexture() override;

		TextureObject* pTex;
	};
}

#endif
