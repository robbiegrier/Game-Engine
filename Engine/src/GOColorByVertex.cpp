#include <d3d11.h>
#include "MathEngine.h"
#include "Mesh.h"
#include "GOColorByVertex.h"
#include "CameraManager.h"

namespace Azul
{
	GOColorByVertex::GOColorByVertex(Mesh* model, ShaderObject* pShaderObj)
		: GraphicsObject(model, pShaderObj)
	{
		name = Name::GraphicsObjectColorByVertex;
	}

	void GOColorByVertex::ActivateGraphics()
	{
	}

	void GOColorByVertex::Draw()
	{
		pModel->RenderIndexBuffer();
	}

	GraphicsObject* GOColorByVertex::Clone()
	{
		return new GOColorByVertex(pModel, pShaderObject);
	}
}