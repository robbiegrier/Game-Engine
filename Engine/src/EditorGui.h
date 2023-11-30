#ifndef _EditorGui
#define _EditorGui

#include "MathEngine.h"
#include <d3d11.h>

namespace Azul
{
	class EditorGui
	{
	public:

		static void Initialize();
		static void NewFrame();
		static void Draw();
		static void Cleanup();

		static void InitializeWindow(UINT width, UINT height);

	private:

		void Update();

		static EditorGui& GetInstance();

		void TreeBuildHelper(class PCSNode* pNode, bool& selection);
		static bool IsGoToRequested();

		// World Window
		ID3D11Texture2D* renderTargetTextureMap;
		ID3D11RenderTargetView* renderTargetViewMap;
		ID3D11ShaderResourceView* shaderResourceViewMap;
		ID3D11DepthStencilView* pDepthStencilView;
		ID3D11Texture2D* pDepthStencilBuffer;
		ID3D11DepthStencilState* pDepthStencilState;
		D3D11_VIEWPORT viewport;

		UINT worldWidth = 0;
		UINT worldHeight = 0;
	};
}

#endif