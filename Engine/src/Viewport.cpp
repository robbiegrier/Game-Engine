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
		Engine::GetContext()->RSSetViewports(1, &viewport);
		Engine::GetContext()->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
		Engine::GetContext()->OMSetDepthStencilState(pDepthStencilState, 1);

		Vec4 col = Vec4{ 0.1f, 0.1f, 0.1f, 1.000000000f };
		Engine::GetContext()->ClearRenderTargetView(pRenderTargetView, (const FLOAT*)&col);
		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;
		Engine::GetContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, (FLOAT)clearDepth, (UINT8)clearStencil);
		CameraManager::GetCurrentCamera()->SetAspectRatio((float)worldWidth / (float)worldHeight);
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

	void Viewport::Refresh()
	{
		Clean();

		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = worldWidth;
		textureDesc.Height = worldHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		Engine::GetDevice()->CreateTexture2D(&textureDesc, NULL, &pRenderTargetTexture);

		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		Engine::GetDevice()->CreateRenderTargetView(pRenderTargetTexture, &renderTargetViewDesc, &pRenderTargetView);

		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		Engine::GetDevice()->CreateShaderResourceView(pRenderTargetTexture, &shaderResourceViewDesc, &pShaderResourceView);

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

		HRESULT hr = Engine::GetDevice()->CreateTexture2D(&depthStencilBufferDesc, nullptr, &pDepthStencilBuffer);
		assert(SUCCEEDED(hr));

		hr = Engine::GetDevice()->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &pDepthStencilView);
		assert(SUCCEEDED(hr));

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilStateDesc.StencilEnable = FALSE;

		hr = Engine::GetDevice()->CreateDepthStencilState(&depthStencilStateDesc, &pDepthStencilState);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

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
	}
}