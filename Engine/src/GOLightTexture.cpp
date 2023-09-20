#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOLightTexture.h"
#include "CameraManager.h"
#include "Engine.h"

namespace Azul
{
	// ---------------------------------------------
	//  Transfer data to the constant buffer
	//    CPU ---> GPU
	//    World, View, Projection Matrix
	// ---------------------------------------------
	GOLightTexture::GOLightTexture(Mesh* mesh,
		ShaderObject* pShaderObj,
		TextureObject* _pTex,
		const Vec3& LightColor,
		const Vec3& LightPos)
		: GraphicsObject(mesh, pShaderObj),
		pTex(_pTex),
		poLightColor(nullptr),
		poLightPos(nullptr)
	{
		assert(pTex);

		poLightColor = new Vec3(LightColor);
		poLightPos = new Vec3(LightPos);

		assert(poLightColor);
		assert(poLightPos);
	}

	GOLightTexture::~GOLightTexture()
	{
		delete poLightColor;
		delete poLightPos;
	}

	void GOLightTexture::ActivateGraphics()
	{
		pTex->ActivateTexture();
		pModel->ActivateModel();
		pModel->TransferConstantBuffer(CameraManager::GetCurrentCamera(), *pWorld);

		Engine::GetContext()->UpdateSubresource(pModel->poConstantBuff_lightColor, 0, nullptr, poLightColor, 0, 0);
		Engine::GetContext()->UpdateSubresource(pModel->poConstantBuff_lightPos, 0, nullptr, poLightPos, 0, 0);
	}

	void GOLightTexture::Draw()
	{
		pModel->RenderIndexBuffer();
	}
}