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
		pBoneWorld{ new Mat4[BONE_COUNT_MAX] },
		pTex(_pTex)
	{
		assert(pTex);
	}

	GOSkinLightTexture::~GOSkinLightTexture()
	{
		delete[] pBoneWorld;
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