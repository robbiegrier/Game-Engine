#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOConstColor.h"
#include "Engine.h"
#include "CameraManager.h"

namespace Azul
{
	GOConstColor::GOConstColor(Mesh* mesh, ShaderObject* pShaderObj, const Vec3& LightColor)
		: GraphicsObject(mesh, pShaderObj),
		poLightColor(nullptr)

	{
		poLightColor = new Vec3(LightColor);
		assert(poLightColor);
		name = Name::GraphicsObjectConstColor;
	}

	GOConstColor::~GOConstColor()
	{
		delete poLightColor;
	}

	void GOConstColor::ActivateGraphics()
	{
		pModel->TransferConstantBuffer(CameraManager::GetCurrentCamera(), *pWorld);
	}

	void GOConstColor::Draw()
	{
		Engine::GetContext()->UpdateSubresource(pModel->poConstantBuff_lightColor, 0, nullptr, poLightColor, 0, 0);
		pModel->RenderIndexBuffer();
	}

	GraphicsObject* GOConstColor::Clone()
	{
		return new GOConstColor(pModel, pShaderObject, *poLightColor);
	}

	Vec4 GOConstColor::GetColor()
	{
		return Vec4(*poLightColor, 1.f);
	}
}