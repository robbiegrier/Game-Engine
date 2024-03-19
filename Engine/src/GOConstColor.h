#ifndef _GOConstColor
#define _GOConstColor

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{
	class GOConstColor :public GraphicsObject
	{
	public:
		GOConstColor(Mesh* mesh, ShaderObject* pShaderObj, const Vec3& _pLightColor);
		GOConstColor() = delete;
		GOConstColor(const GOConstColor&) = delete;
		GOConstColor& operator = (const GOConstColor&) = delete;
		virtual ~GOConstColor();

		virtual void ActivateGraphics() override;
		virtual void Draw() override;
		virtual GraphicsObject* Clone() override;

		virtual Vec4 GetColor() override;

		Vec3* poLightColor;
	};
}

#endif