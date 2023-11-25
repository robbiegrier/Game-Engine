#ifndef _Engine
#define _Engine

#include "AnimTimer.h"
#include "EngineUtils.h"

namespace Azul
{
	// The Engine encapsulates low level interaction with Windows and DirectX
	// so that the Game derived class can be streamlined around game loop logic.
	class Engine
	{
	public:
		// Settings
		static constexpr bool ENABLE_VSYNC = true;
		static constexpr int WINDOW_WIDTH = 600;
		static constexpr int WINDOW_HEIGHT = 400;
		static constexpr int WINDOW_SCALE = 2;

		// Engine Queries
		static int GetWindowWidth();
		static int GetWindowHeight();
		static float GetAspectRatio();
		static void SetMouseVisibility(bool visible);
		static void ToggleMaxFramerate(bool enforceMax);
		static ID3D11Device* GetDevice();
		static ID3D11DeviceContext* GetContext();

	protected:
		// Big four
		Engine();
		Engine(const Engine&) = delete;
		Engine& operator = (const Engine&) = delete;
		virtual ~Engine() = default;

		// Engine/Game Interface
		virtual bool LoadContent() = 0;
		virtual void UnloadContent() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;
		virtual	void ClearDepthStencilBuffer() = 0;

		// Startup methods
		int WINAPI Main(HINSTANCE pInstance, int cmdShow);
		void InitApplication(HINSTANCE pInstance, int cmdShow);
		int InitWindowClass(HINSTANCE pInstance);
		void RecreateWindow(HINSTANCE pInstance, int cmdShow);
		void InitDirectX();
		void InitSwapChain();
		void InitBackBuffer();
		void InitDepthStencilBuffer();
		void InitDepthStencilState();
		void InitRasterizerState();
		void InitViewport();

		// Runtime
		int Run();
		void Present();
		void LockFramerate(const Azul::AnimTimer& engineTime);
		void UpdateWindowName(float deltaTime);
		void Cleanup();
		void SetDefaultTargetMode();

		// Singleton access
		static Engine& GetEngineInstance();

		// DirectX Resources
		HWND pWindowHandle;
		ID3D11Device* pDevice;
		ID3D11DeviceContext* pContext;
		IDXGISwapChain* pSwapChain;
		ID3D11RenderTargetView* pRenderTargetView;
		ID3D11DepthStencilView* pDepthStencilView;
		ID3D11Texture2D* pDepthStencilBuffer;
		ID3D11DepthStencilState* pDepthStencilState;
		ID3D11RasterizerState* pRasterizerState;
		D3D11_VIEWPORT viewport;

		// Display Parameters
		const char* pName;
		const int windowWidth;
		const int windowHeight;
		BOOL vsync;

		// FPS sampling for display
		constexpr static int fpsSampleBucketSize = 10;
		int fpsSamples[fpsSampleBucketSize];
		int fpsSampleIndex = 0;
		bool enforceMaxFrameRate = false;

		// Derived Singleton
		static Engine* pEngineInstance;
	};
}

#endif
