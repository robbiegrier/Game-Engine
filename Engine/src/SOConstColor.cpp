#include "SOConstColor.h"
#include "Mesh.h"
#include "Engine.h"

// User generated headers
#include "ConstColor.Px.h"
#include "ConstColor.Vx.h"

namespace Azul
{
	void SOConstColor::OnOpen(GraphicsObject* pObject)
	{
		D3D11_RASTERIZER_DESC rasterizerDesc;
		memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		ID3D11RasterizerState* pRasterState;
		HRESULT hr;
		hr = Engine::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterState);
		assert(SUCCEEDED(hr));

		Engine::GetContext()->RSSetState(pRasterState);

		SafeRelease(pRasterState);
	}

	void SOConstColor::OnClose()
	{
		D3D11_RASTERIZER_DESC rasterizerDesc;
		memset(&rasterizerDesc, 0, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.AntialiasedLineEnable = FALSE;
		rasterizerDesc.CullMode = D3D11_CULL_FRONT;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		ID3D11RasterizerState* pRasterState;
		HRESULT hr;
		hr = Engine::GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterState);
		assert(SUCCEEDED(hr));

		Engine::GetContext()->RSSetState(pRasterState);

		SafeRelease(pRasterState);
	}

	SOConstColor::SOConstColor()
		: ShaderObject()
	{
		this->CreateVertexShader(g_ConstColor_VxShader, sizeof(g_ConstColor_VxShader));

		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		this->CreateInputLayout(g_ConstColor_VxShader, sizeof(g_ConstColor_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));
		this->CreatePixelShader(g_ConstColor_PxShader, sizeof(g_ConstColor_PxShader));
	}
}