#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOLightTexture.h"
#include "CameraManager.h"
#include "Engine.h"

namespace Azul
{
	GOLightTexture::GOLightTexture(Mesh* mesh, ShaderObject* pShaderObj, TextureObject* _pTex)
		: GraphicsObject(mesh, pShaderObj), pTex(_pTex)
	{
		assert(pTex);
		name = Name::GraphicsObjectLightTexture;
	}

	GOLightTexture::~GOLightTexture()
	{
	}

	void GOLightTexture::ActivateGraphics()
	{
		pTex->ActivateTexture();
	}

	void GOLightTexture::Draw()
	{
		pModel->RenderIndexBuffer();

		for (Iterator& it = *pModel->subMeshes.GetIterator(); !it.IsDone(); it.Next())
		{
			Mesh* pSubMesh = (Mesh*)it.Curr();
			pSubMesh->RenderIndexBuffer();
		}
	}

	GraphicsObject* GOLightTexture::Clone()
	{
		return new GOLightTexture(pModel, pShaderObject, pTex);
	}

	TextureObject* GOLightTexture::GetTexture()
	{
		return pTex;
	}
}