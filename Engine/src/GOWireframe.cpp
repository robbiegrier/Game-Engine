#include <d3d11.h>
#include "Engine.h"
#include "MathEngine.h"
#include "Mesh.h"
#include "GOWireframe.h"
#include "ShaderObject.h"
#include "CameraManager.h"

namespace Azul
{
	//GOWireframe::GOWireframe(Mesh* _pModel, ShaderObject* _pShaderObj)
	//	: GraphicsObject(_pModel, _pShaderObj)
	//{
	//	assert(pModel);
	//	assert(pShaderObject);
	//	assert(pWorld);
	//}

	//void GOWireframe::OpenShader()
	//{
	//	D3D11_RASTERIZER_DESC rasterizerDesc;
	//	memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	//	rasterizerDesc.AntialiasedLineEnable = FALSE;
	//	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	//	rasterizerDesc.DepthBias = 0;
	//	rasterizerDesc.DepthBiasClamp = 0.0f;
	//	rasterizerDesc.DepthClipEnable = TRUE;
	//	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	//	rasterizerDesc.FrontCounterClockwise = FALSE;
	//	rasterizerDesc.MultisampleEnable = FALSE;

	//	// To Do add scissor rectangle... its faster
	//	rasterizerDesc.ScissorEnable = FALSE;
	//	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	//	// Create the rasterizer state object.
	//	ID3D11RasterizerState* pRasterState;
	//	auto hr = Engine::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterState);
	//	assert(SUCCEEDED(hr));
	//	static_cast<void>(hr);

	//	Engine::GetContext()->RSSetState(pRasterState);

	//	SafeRelease(pRasterState);

	//	pShaderObject->Open();
	//}

	//void GOWireframe::ActivateGraphics()
	//{
	//	pModel->ActivateModel();
	//	pModel->TransferConstantBuffer(CameraManager::GetCurrentCamera(), pWorld);
	//}

	//void GOWireframe::Draw()
	//{
	//	pModel->RenderIndexBuffer();
	//}

	//void GOWireframe::CloseShader()
	//{
	//	D3D11_RASTERIZER_DESC rasterizerDesc;
	//	memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

	//	rasterizerDesc.AntialiasedLineEnable = FALSE;
	//	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	//	rasterizerDesc.DepthBias = 0;
	//	rasterizerDesc.DepthBiasClamp = 0.0f;
	//	rasterizerDesc.DepthClipEnable = TRUE;
	//	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//	rasterizerDesc.FrontCounterClockwise = FALSE;
	//	rasterizerDesc.MultisampleEnable = FALSE;
	//	rasterizerDesc.ScissorEnable = FALSE;
	//	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	//	ID3D11RasterizerState* pRasterState;
	//	auto hr = Engine::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterState);
	//	assert(SUCCEEDED(hr));
	//	static_cast<void>(hr);

	//	Engine::GetContext()->RSSetState(pRasterState);

	//	SafeRelease(pRasterState);
	//}
}