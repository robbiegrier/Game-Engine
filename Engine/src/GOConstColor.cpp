#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOConstColor.h"
#include "Engine.h"
#include "CameraManager.h"
#include "MeshManager.h"
#include "ShaderObjectManager.h"

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
	}

	void GOConstColor::Draw()
	{
		Engine::GetContext()->UpdateSubresource(pModel->GetConstantBufferLightColor(), 0, nullptr, poLightColor, 0, 0);
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

	GOConstColor* GOConstColor::MakeWireframeCube(const Vec3& lightColor)
	{
		return new GOConstColor(
			MeshManager::Find(Mesh::Name::UnitCube),
			ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
			lightColor
		);
	}
}