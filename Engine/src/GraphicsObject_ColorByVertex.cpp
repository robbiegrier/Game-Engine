//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_ColorByVertex.h"

extern Camera *pHackCamera;

using namespace Azul;

// ---------------------------------------------
//  Transfer data to the constant buffer
//    CPU ---> GPU
//    World, View, Projection Matrix
// ---------------------------------------------
GraphicsObject_ColorByVertex::GraphicsObject_ColorByVertex(Mesh *mesh, ShaderObject *pShaderObj)
	: GraphicsObject(mesh, pShaderObj)
{

}

void GraphicsObject_ColorByVertex::SetState()
{
	// Future - settings to directX
	// say make it wireframe or change culling mode
}

void GraphicsObject_ColorByVertex::SetDataGPU()
{
	pShaderObj->ActivateShader();
	pMesh->ActivateMesh();

	pMesh->TransferConstantBuffer(pHackCamera, this->poWorld);
}

void GraphicsObject_ColorByVertex::Draw()
{
	pMesh->RenderIndexBuffer();
}

void GraphicsObject_ColorByVertex::RestoreState()
{
	// Future - Undo settings to directX
}


// --- End of File ---
