#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Mesh.h"

namespace Azul
{
	GraphicsObject::GraphicsObject(Mesh* model, ShaderObject* _pShaderObj)
		: pModel(model), pShaderObject(_pShaderObj), pWorld{ new Mat4(Mat4::Special::Identity) }
	{
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

	const Mat4& GraphicsObject::GetWorld() const
	{
		return *pWorld;
	}

	void GraphicsObject::SetWorld(const Mat4& _world)
	{
		*pWorld = _world;
	}
}