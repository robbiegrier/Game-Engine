#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOFlatTexture.h"
#include "CameraManager.h"

namespace Azul
{
	GOFlatTexture::GOFlatTexture(Mesh* mesh,
		ShaderObject* pShaderObj,
		TextureObject* _pTex)
		: GraphicsObject(mesh, pShaderObj),
		pTex(_pTex)
	{
		assert(pTex);
		name = Name::GraphicsObjectFlatTexture;
	}

	void GOFlatTexture::ActivateGraphics()
	{
		pTex->ActivateTexture();
	}

	void GOFlatTexture::Draw()
	{
		pModel->RenderIndexBuffer();
	}

	GraphicsObject* GOFlatTexture::Clone()
	{
		return new GOFlatTexture(pModel, pShaderObject, pTex);
	}

	TextureObject* GOFlatTexture::GetTexture()
	{
		return pTex;
	}
}