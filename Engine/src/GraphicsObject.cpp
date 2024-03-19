#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Mesh.h"
#include "TextureObject.h"

namespace Azul
{
	GraphicsObject::GraphicsObject(Mesh* model, ShaderObject* _pShaderObj)
		: pModel(model), pShaderObject(_pShaderObj), pWorld{ new Mat4(Special::Identity) }
	{
		name = Name::None;
	}

	GraphicsObject::~GraphicsObject()
	{
		delete pWorld;
	}

	void GraphicsObject::Render()
	{
		pShaderObject->Open(this);
		ActivateGraphics();
		Draw();
		pShaderObject->Close();
	}

	Mesh* GraphicsObject::GetModel() const
	{
		return pModel;
	}

	ShaderObject* GraphicsObject::GetShader() const
	{
		return pShaderObject;
	}

	const Mat4& GraphicsObject::GetWorld() const
	{
		return *pWorld;
	}

	TextureObject* GraphicsObject::GetTexture()
	{
		return nullptr;
	}

	Vec4 GraphicsObject::GetColor()
	{
		return Vec4(1.f, 1.f, 1.f, 1.f);
	}

	GraphicsObject::Name GraphicsObject::GetName() const
	{
		return name;
	}

	void GraphicsObject::SetWorld(const Mat4& _world)
	{
		*pWorld = _world;
	}
}