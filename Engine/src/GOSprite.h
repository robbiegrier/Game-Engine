#ifndef _GOSprite
#define _GOSprite

#include "GraphicsObject.h"
#include "TextureObject.h"
#include "Image.h"
#include "Rect.h"

namespace Azul
{
	class GOSprite : public GraphicsObject
	{
	public:
		GOSprite(Mesh* pMesh, ShaderObject* pShader, Image* pInImage, Rect rect);
		GOSprite(Image* pInImage, Rect rect);

		GOSprite() = delete;
		GOSprite(const GOSprite&) = delete;
		GOSprite& operator = (const GOSprite&) = delete;
		virtual ~GOSprite();

		virtual void ActivateGraphics() override;
		virtual void Draw() override;

		void OverwriteImageProperties(TextureObject* pInTexture, Rect inRect);
		virtual GraphicsObject* Clone() override;

		Image* pImage;
		Mat4* poMatrix_uv;
		//Mat4* poMatrix_orig;

		float origWidth;
		float origHeight;

		float origPosX;
		float origPosY;
	};
}

#endif