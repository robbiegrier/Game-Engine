#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOSkinLightTexture.h"
#include "CameraManager.h"
#include "Engine.h"

namespace Azul
{
	GOSkinLightTexture::GOSkinLightTexture(Mesh* mesh, ShaderObject* pShaderObj, TextureObject* _pTex)
		: GraphicsObject(mesh, pShaderObj),
		pTex(_pTex)
	{
		assert(pTex);
	}

	void GOSkinLightTexture::ActivateGraphics()
	{
		pTex->ActivateTexture();

		pModel->TransferConstantBufferBoneWorldArray(this->pBoneWorld);
	}

	GraphicsObject* GOSkinLightTexture::Clone()
	{
		return new GOSkinLightTexture(pModel, pShaderObject, pTex);
	}

	void GOSkinLightTexture::Draw()
	{
		pModel->RenderIndexBuffer();
	}
}