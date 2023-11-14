//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_LIGHT_TEXTURE_H
#define GRAPHICS_OBJECT_LIGHT_TEXTURE_H

#include "GraphicsObject.h"
#include "TextureObject.h"

class GraphicsObject_LightTexture :public GraphicsObject
{
public:
	GraphicsObject_LightTexture(Mesh *mesh,
								ShaderObject *pShaderObj,
								TextureObject *pTex,
								Vec3 &_pLightColor,
								Vec3 &_pLightPos);

	GraphicsObject_LightTexture() = delete;
	GraphicsObject_LightTexture(const GraphicsObject_LightTexture &) = delete;
	GraphicsObject_LightTexture &operator = (const GraphicsObject_LightTexture &) = delete;
	virtual ~GraphicsObject_LightTexture();

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here

	TextureObject *pTex;
	Vec3 *poLightColor;
	Vec3 *poLightPos;
};
#endif


// --- End of File ---
