#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOSurfaceShape.h"
#include "CameraManager.h"

namespace Azul
{
	GOSurfaceShape::GOSurfaceShape(Mesh* mesh,
		ShaderObject* pShaderObj,
		TextureObject* _pTex)
		: GraphicsObject(mesh, pShaderObj),
		pTex(_pTex)
	{
		assert(pTex);
		name = Name::GraphicsObjectSurfaceShape;
	}

	void GOSurfaceShape::ActivateGraphics()
	{
		pTex->ActivateTexture();
	}

	void GOSurfaceShape::Draw()
	{
		pModel->RenderIndexBuffer();
	}

	GraphicsObject* GOSurfaceShape::Clone()
	{
		return new GOSurfaceShape(pModel, pShaderObject, pTex);
	}

	TextureObject* GOSurfaceShape::GetTexture()
	{
		return pTex;
	}
}