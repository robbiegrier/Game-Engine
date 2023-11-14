//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GraphicsObject_FlatTexture.h"

extern Camera *pHackCamera;

using namespace Azul;

// ---------------------------------------------
//  Transfer data to the constant buffer
//    CPU ---> GPU
//    World, View, Projection Matrix
// ---------------------------------------------
GraphicsObject_FlatTexture::GraphicsObject_FlatTexture(Mesh *mesh, 
													   ShaderObject *pShaderObj, 
													   TextureObject *_pTex)
	: GraphicsObject(mesh, pShaderObj),
	pTex(_pTex)
{
	assert(pTex);
}

void GraphicsObject_FlatTexture::SetState()
{
	// Future - settings to directX
	// say make it wireframe or change culling mode
	this->pTex->ActivateTexture();
}

void GraphicsObject_FlatTexture::SetDataGPU()
{
	pShaderObj->ActivateShader();
	pMesh->ActivateMesh();

	pMesh->TransferConstantBuffer(pHackCamera, this->poWorld);
}

void GraphicsObject_FlatTexture::Draw()
{
	pMesh->RenderIndexBuffer();
}

void GraphicsObject_FlatTexture::RestoreState()
{
	// Future - Undo settings to directX
}


// --- End of File ---
