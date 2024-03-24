#include "Viewport.h"
#include "Engine.h"
#include "CameraManager.h"

namespace Azul
{
	Viewport::Viewport(UINT inWidth, UINT inHeight)
		: worldWidth(inWidth), worldHeight(inHeight)
	{
		Refresh();
	}

	Viewport::~Viewport()
	{
		Clean();
	}

	void Viewport::Activate()
	{
		//Vec4 col = Vec4{ 0.500000000f, 0.749019623f, 0.800000000f, 1.000000000f };
		Vec4 col = Vec4{ 0.5f, 0.5f, 0.5f, 1.f };
		GetContext()->ClearRenderTargetView(pRenderTargetView, (const FLOAT*)&col);

		GetContext()->RSSetState(pRasterizerState);
		GetContext()->RSSetViewports(1, &viewport);
		GetContext()->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

		GetContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, (FLOAT)1.f, (UINT8)0u);
		CameraManager::GetCurrentCamera()->SetAspectRatio((float)worldWidth / (float)worldHeight);

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT sampleMask = 0xffffffff;
		GetContext()->OMSetBlendState(pBlendStateAlpha, blendFactor, sampleMask);
		GetContext()->OMSetDepthStencilState(pDepthStencilState, 1);
	}

	void Viewport::Close()
	{
		//float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		//UINT sampleMask = 0xffffffff;
		//GetContext()->OMSetBlendState(pBlendStateOff, blendFactor, sampleMask);
	}

	void Viewport::Resize(UINT inWidth, UINT inHeight)
	{
		if (inWidth == worldWidth && inHeight == worldHeight) return;

		worldWidth = inWidth;
		worldHeight = inHeight;

		Refresh();
	}

	void Viewport::ResizeByWidth(UINT inWidth)
	{
		if (inWidth == worldWidth) return;

		const float aspectRatio = static_cast<float>(worldWidth) / static_cast<float>(worldHeight);
		worldWidth = inWidth;
		worldHeight = (UINT)(static_cast<float>(worldWidth) / aspectRatio);

		Refresh();
	}

	void Viewport::ResizeByHeight(UINT inHeight)
	{
		if (inHeight == worldHeight) return;

		const float aspectRatio = static_cast<float>(worldWidth) / static_cast<float>(worldHeight);
		worldHeight = inHeight;
		worldWidth = (UINT)(static_cast<float>(worldHeight) * aspectRatio);

		Refresh();
	}

	void Viewport::ToggleDepthTests(bool toggleDepth)
	{
		SafeRelease(pDepthStencilState);

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = toggleDepth ? D3D11_COMPARISON_LESS : D3D11_COMPARISON_ALWAYS;
		depthStencilStateDesc.StencilEnable = FALSE;

		HRESULT hr = GetDevice()->CreateDepthStencilState(&depthStencilStateDesc, &pDepthStencilState);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		GetContext()->OMSetDepthStencilState(pDepthStencilState, 1);
	}

	void Viewport::Refresh()
	{
		Clean();

		HRESULT hr;
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		ZeroMemory(&textureDesc, sizeof(textureDesc));
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

		textureDesc.Width = worldWidth;
		textureDesc.Height = worldHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		GetDevice()->CreateTexture2D(&textureDesc, NULL, &pRenderTargetTexture);

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		GetDevice()->CreateRenderTargetView(pRenderTargetTexture, &renderTargetViewDesc, &pRenderTargetView);

		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		GetDevice()->CreateShaderResourceView(pRenderTargetTexture, &shaderResourceViewDesc, &pShaderResourceView);

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc{ 0 };
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilBufferDesc.Width = worldWidth;
		depthStencilBufferDesc.Height = worldHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		hr = GetDevice()->CreateTexture2D(&depthStencilBufferDesc, nullptr, &pDepthStencilBuffer);
		assert(SUCCEEDED(hr));

		hr = GetDevice()->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &pDepthStencilView);
		assert(SUCCEEDED(hr));

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilStateDesc.StencilEnable = FALSE;

		hr = GetDevice()->CreateDepthStencilState(&depthStencilStateDesc, &pDepthStencilState);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

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

		hr = GetDevice()->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		CD3D11_BLEND_DESC BlendState;
		ZeroMemory(&BlendState, sizeof(CD3D11_BLEND_DESC));
		BlendState.RenderTarget[0].BlendEnable = FALSE;
		BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = GetDevice()->CreateBlendState(&BlendState, &pBlendStateOff);
		assert(SUCCEEDED(hr));

		CD3D11_BLEND_DESC BlendState2;
		ZeroMemory(&BlendState2, sizeof(CD3D11_BLEND_DESC));
		BlendState2.RenderTarget[0].BlendEnable = TRUE;
		BlendState2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendState2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendState2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendState2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendState2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		BlendState2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendState2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = GetDevice()->CreateBlendState(&BlendState2, &pBlendStateAlpha);
		assert(SUCCEEDED(hr));

		viewport.Width = static_cast<float>(worldWidth);
		viewport.Height = static_cast<float>(worldHeight);
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	void Viewport::Clean()
	{
		SafeRelease(pRenderTargetTexture);
		SafeRelease(pRenderTargetView);
		SafeRelease(pShaderResourceView);
		SafeRelease(pDepthStencilBuffer);
		SafeRelease(pDepthStencilView);
		SafeRelease(pDepthStencilState);
		SafeRelease(pRasterizerState);
		SafeRelease(pBlendStateAlpha);
		SafeRelease(pBlendStateOff);
	}

	ID3D11Device* Viewport::GetDevice() const
	{
		return Engine::GetDevice();
	}

	ID3D11DeviceContext* Viewport::GetContext() const
	{
		return Engine::GetContext();
	}
}