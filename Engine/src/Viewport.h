#ifndef _Viewport
#define _Viewport

#include "EngineUtils.h"

namespace Azul
{
	class Viewport
	{
	public:
		Viewport() = delete;
		Viewport(UINT inWidth, UINT inHeight);
		Viewport(const Viewport&) = delete;
		Viewport& operator=(const Viewport&) = delete;
		~Viewport();

		void Activate();
		void Close();

		void Resize(UINT inWidth, UINT inHeight);
		void ResizeByWidth(UINT inWidth);
		void ResizeByHeight(UINT inHeight);

		UINT GetWidth() const { return worldWidth; }
		UINT GetHeight() const { return worldHeight; }
		ID3D11ShaderResourceView* GetShaderResourceView() const { return pShaderResourceView; }
		ID3D11DepthStencilState* GetDepthStencilState() const { return pDepthStencilState; }
		ID3D11Texture2D* GetRenderTargetTexture() const { return pRenderTargetTexture; }
		ID3D11RenderTargetView* GetRenderTargetView() const { return pRenderTargetView; }

		void ToggleDepthTests(bool toggleDepth);
		void ToggleBlending(bool blendOn);

	private:
		void Refresh();
		void Clean();

		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetContext() const;

		IDXGISwapChain* pSwapChain;

		ID3D11Texture2D* pRenderTargetTexture;
		ID3D11RenderTargetView* pRenderTargetView;
		ID3D11ShaderResourceView* pShaderResourceView;
		ID3D11DepthStencilView* pDepthStencilView;
		ID3D11Texture2D* pDepthStencilBuffer;
		ID3D11DepthStencilState* pDepthStencilState;
		ID3D11BlendState* pBlendStateAlpha;
		ID3D11BlendState* pBlendStateOff;
		ID3D11RasterizerState* pRasterizerState;
		D3D11_VIEWPORT viewport;

		UINT worldWidth = 0;
		UINT worldHeight = 0;
	};
}

#endif