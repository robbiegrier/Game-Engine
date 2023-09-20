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
	}

	void GOColorByVertex::ActivateGraphics()
	{
		pModel->ActivateModel();
		pModel->TransferConstantBuffer(CameraManager::GetCurrentCamera(), *pWorld);
	}

	void GOColorByVertex::Draw()
	{
		pModel->RenderIndexBuffer();
	}
}