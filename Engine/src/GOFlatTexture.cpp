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
	}

	void GOFlatTexture::ActivateGraphics()
	{
		pTex->ActivateTexture();

		pModel->TransferConstantBuffer(CameraManager::GetCurrentCamera(), *pWorld);
	}

	void GOFlatTexture::Draw()
	{
		pModel->RenderIndexBuffer();
	}
}