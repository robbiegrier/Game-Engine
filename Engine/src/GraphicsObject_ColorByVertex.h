//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_COLOR_BY_Vertex_H
#define GRAPHICS_OBJECT_COLOR_BY_Vertex_H

#include "GraphicsObject.h"

class GraphicsObject_ColorByVertex :public GraphicsObject
{
public:
	GraphicsObject_ColorByVertex(Mesh *mesh, ShaderObject *pShaderObj);

	GraphicsObject_ColorByVertex() = delete;
	GraphicsObject_ColorByVertex(const GraphicsObject_ColorByVertex &) = delete;
	GraphicsObject_ColorByVertex &operator = (const GraphicsObject_ColorByVertex &) = delete;
	virtual ~GraphicsObject_ColorByVertex() = default;

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here

};
#endif


// --- End of File ---
