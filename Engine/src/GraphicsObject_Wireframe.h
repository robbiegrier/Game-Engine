//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_WIREFRAME_H
#define GRAPHICS_OBJECT_WIREFRAME_H

#include "GraphicsObject.h"

class GraphicsObject_Wireframe :public GraphicsObject
{
public:
	GraphicsObject_Wireframe(Mesh *mesh, ShaderObject *pShaderObj, Vec3 &_pLightColor);

	GraphicsObject_Wireframe() = delete;
	GraphicsObject_Wireframe(const GraphicsObject_Wireframe &) = delete;
	GraphicsObject_Wireframe &operator = (const GraphicsObject_Wireframe &) = delete;
	virtual ~GraphicsObject_Wireframe();

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here
	Vec3 *poLightColor;
};

#endif

// ---  End of File ---
