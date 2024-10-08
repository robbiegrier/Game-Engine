#include <d3d11.h>
#include <d3dcompiler.h>
#include "Engine.h"
#include "MathEngine.h"
#include "EditorGui.h"
#include "EditorInput.h"
#include "CameraManager.h"
#include "Viewport.h"
#include "EngineStatePlay.h"
#include "EngineStateEditor.h"

LPCSTR g_WindowClassName = "EngineWindowClass";

#if _DEBUG
#define createDeviceFlags_define  D3D11_CREATE_DEVICE_DEBUG

#else
#define createDeviceFlags_define  0
#endif

namespace Azul
{
	Engine* Engine::pEngineInstance = nullptr;

	Engine::Engine()
		: viewport{ 0 }, pName("Robbie's Engine"),
		windowWidth(WINDOW_WIDTH* WINDOW_SCALE), windowHeight(WINDOW_HEIGHT* WINDOW_SCALE), vsync(ENABLE_VSYNC),
		pWindowHandle(0), pDevice(nullptr), pContext(nullptr), pSwapChain(nullptr),
		pRenderTargetView(nullptr), pDepthStencilView(nullptr), pDepthStencilBuffer(nullptr),
		pDepthStencilState(nullptr), pRasterizerState(nullptr)
	{
		for (int i = 0; i < fpsSampleBucketSize; i++)
		{
			fpsSamples[i] = 60;
		}

		pEditorState = new EngineStateEditor();
		pPlayState = new EngineStatePlay();
		pCurrentState = pEditorState;
	}

	Engine::~Engine()
	{
		delete pEditorState;
		delete pPlayState;
	}

	int WINAPI Engine::Main(HINSTANCE pInstanceHandle, int cmdShow)
	{
		InitApplication(pInstanceHandle, cmdShow);
		InitDirectX();

		return Run();
	}

	void Engine::InitApplication(HINSTANCE pInstanceHandle, int cmdShow)
	{
		InitWindowClass(pInstanceHandle);
		RecreateWindow(pInstanceHandle, cmdShow);
	}

	void Engine::InitDirectX()
	{
		InitSwapChain();
		InitBackBuffer();
		InitDepthStencilBuffer();
		InitDepthStencilState();
		InitRasterizerState();
		InitViewport();
	}

	int Engine::InitWindowClass(HINSTANCE pInstanceHandle)
	{
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = EngineUtils::WndProc;
		wndClass.hInstance = pInstanceHandle;
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wndClass.hIcon = nullptr;
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndClass.lpszMenuName = nullptr;
		wndClass.lpszClassName = g_WindowClassName;
		wndClass.hIconSm = nullptr;
		if (!RegisterClassEx(&wndClass))
		{
			return -1;
		}

		return 0;
	}

	void Engine::RecreateWindow(HINSTANCE pInstanceHandle, int cmdShow)
	{
		static_cast<void>(cmdShow);

		RECT windowRect = { 0, 0, windowWidth, windowHeight };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		pWindowHandle = CreateWindowA(g_WindowClassName, pName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, pInstanceHandle, nullptr);
		assert(pWindowHandle);

		ShowWindow(pWindowHandle, SW_SHOWMAXIMIZED);
		UpdateWindow(pWindowHandle);
	}

	void Engine::InitSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = windowWidth;
		swapChainDesc.BufferDesc.Height = windowHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate = EngineUtils::QueryRefreshRate(windowWidth, windowHeight, vsync);
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = pWindowHandle;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0,D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,D3D_FEATURE_LEVEL_9_3,D3D_FEATURE_LEVEL_9_2,D3D_FEATURE_LEVEL_9_1 };

		D3D_FEATURE_LEVEL featureLevel;
		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags_define, featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, &pDevice, &featureLevel, &pContext);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);
	}

	void Engine::InitBackBuffer()
	{
		ID3D11Texture2D* backBuffer = nullptr;
		HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		assert(SUCCEEDED(hr));

		hr = pDevice->CreateRenderTargetView(backBuffer, nullptr, &pRenderTargetView);
		assert(SUCCEEDED(hr));
		SafeRelease(backBuffer);
		static_cast<void>(hr);
	}

	void Engine::InitDepthStencilBuffer()
	{
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc{ 0 };
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilBufferDesc.Width = windowWidth;
		depthStencilBufferDesc.Height = windowHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr = pDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &pDepthStencilBuffer);
		assert(SUCCEEDED(hr));

		hr = pDevice->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &pDepthStencilView);
		assert(SUCCEEDED(hr));
	}

	void Engine::InitDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilStateDesc.StencilEnable = FALSE;

		HRESULT hr = pDevice->CreateDepthStencilState(&depthStencilStateDesc, &pDepthStencilState);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);
	}

	void Engine::InitRasterizerState()
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

		HRESULT hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);
	}

	void Engine::InitViewport()
	{
		viewport.Width = static_cast<float>(windowWidth);
		viewport.Height = static_cast<float>(windowHeight);
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	int Engine::Run()
	{
		MSG msg = { 0 };

		static DWORD previousTime = timeGetTime();
		static const float targetFramerate = 60.0f;
		static const float maxTimeStep = 1.0f / targetFramerate;

		AnimTimer engineTime;

		if (!LoadContent())
		{
			MessageBox(nullptr, TEXT("Failed to load content."), TEXT("Error"), MB_OK);
			return -1;
		}

		EditorGui::Initialize();

		while (msg.message != WM_QUIT)
		{
			engineTime.Tic();

			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				const DWORD currentTime = timeGetTime();
				storedDeltaTime = (currentTime - previousTime) / 1000.0f;
				previousTime = currentTime;

				pCurrentState->Update(GetDeltaTime());

				SetDefaultTargetMode();
				ClearDepthStencilBuffer({ 0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f });
				pCurrentState->Draw();

				LockFramerate(engineTime);
				Present();
				UpdateWindowName(storedDeltaTime);
				mouseWheelDelta = 0;
			}
		}

		UnloadContent();
		Cleanup();

		return static_cast<int>(msg.wParam);
	}

	void Engine::LockFramerate(const Azul::AnimTimer& engineTime)
	{
		if (enforceMaxFrameRate)
		{
			AnimTime engineLoopTime = engineTime.Toc();

			// Current number of Frames... snap to the next 60Hz frame as target
			int NumFrames_60Hz = AnimTime::Quotient(engineLoopTime, AnimTime(AnimTime::Duration::NTSC_FRAME));
			int TargetNumFrames_60Hz = NumFrames_60Hz + 1;
			AnimTime Target_60Hz = TargetNumFrames_60Hz * AnimTime(AnimTime::Duration::NTSC_FRAME);

			// we are before the flip... give a little cushion
			AnimTime Overhead_Time = 500 * AnimTime(AnimTime::Duration::ONE_MICROSECOND);
			AnimTime EndTime = Target_60Hz - Overhead_Time;

			// Sit and spin.
			while (engineLoopTime < EndTime)
			{
				engineLoopTime = engineTime.Toc();
			}
		}
	}

	void Engine::UpdateWindowName(float deltaTime)
	{
		deltaTime = std::max(0.001f, deltaTime);
		const int fpsSample = (int)(1.f / deltaTime);
		fpsSamples[fpsSampleIndex] = fpsSample;

		fpsSampleIndex = (fpsSampleIndex + 1) % fpsSampleBucketSize;

		int fpsAverage = 0;

		for (int i = 0; i < fpsSampleBucketSize; i++)
		{
			fpsAverage += fpsSamples[i];
		}

		fpsAverage /= fpsSampleBucketSize;

		char buffer[64];
		sprintf_s(buffer, "%s | %d fps", pName, fpsAverage);
		SetWindowTextA(pWindowHandle, buffer);
	}

	void Engine::Present()
	{
		if (vsync)
		{
			pSwapChain->Present(1, 0);
		}
		else
		{
			pSwapChain->Present(0, 0);
		}
	}

	int Engine::GetWindowWidth()
	{
		return GetEngineInstance().windowWidth;
	}

	int Engine::GetWindowHeight()
	{
		return GetEngineInstance().windowHeight;
	}

	void Engine::CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
		pBackBuffer->Release();
	}

	void Engine::CleanupRenderTarget()
	{
		if (pRenderTargetView)
		{
			pRenderTargetView->Release();
			pRenderTargetView = nullptr;
		}
	}

	float Engine::GetAspectRatio()
	{
		RECT clientRect;
		GetClientRect(GetEngineInstance().pWindowHandle, &clientRect);

		float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
		float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);
		float ratio = clientWidth / clientHeight;

		return ratio;
	}

	void Engine::SetDefaultTargetMode()
	{
		Engine& self = GetEngineInstance();

		assert(self.pDevice);
		assert(self.pContext);

		self.pContext->RSSetState(self.pRasterizerState);
		self.pContext->RSSetViewports(1, &self.viewport);
		self.pContext->OMSetRenderTargets(1, &self.pRenderTargetView, self.pDepthStencilView);
		self.pContext->OMSetDepthStencilState(self.pDepthStencilState, 1);
	}

	void Engine::ClearDepthStencil(const Vec4& color)
	{
		GetEngineInstance().ClearDepthStencilBuffer(color);
	}

	void Engine::RevertRasterState()
	{
		Engine& self = GetEngineInstance();
		self.pContext->RSSetState(self.pRasterizerState);
	}

	void Engine::OnMouseWheelEvent(int wheelDelta)
	{
		GetEngineInstance().mouseWheelDelta = wheelDelta;
	}

	int Engine::GetMouseWheelDelta()
	{
		return GetEngineInstance().mouseWheelDelta;
	}

	void Engine::SetMouseVisibility(bool visible)
	{
		int cursorState = 0;

		if (visible)
		{
			while (cursorState <= 0)
			{
				cursorState = ShowCursor(TRUE);
			}
		}
		else
		{
			while (cursorState >= 0)
			{
				cursorState = ShowCursor(FALSE);
			}
		}
	}

	void Engine::ToggleMaxFramerate(bool enforceMax)
	{
		GetEngineInstance().enforceMaxFrameRate = enforceMax;
	}

	ID3D11Device* Engine::GetDevice()
	{
		return GetEngineInstance().pDevice;
	}

	ID3D11DeviceContext* Engine::GetContext()
	{
		return GetEngineInstance().pContext;
	}

	HWND Engine::GetWindowHandle()
	{
		return GetEngineInstance().pWindowHandle;
	}

	bool Engine::IsWindowFocused()
	{
		return GetWindowHandle() == GetFocus();
	}

	void Engine::Resize(unsigned int w, unsigned int h)
	{
		Engine& self = GetEngineInstance();

		self.windowWidth = w;
		self.windowHeight = h;

		if (self.pSwapChain)
		{
			GetContext()->OMSetRenderTargets(0, 0, 0);

			// Release all outstanding references to the swap chain's buffers.
			//self.pRenderTargetView->Release();

			SafeRelease(self.pRenderTargetView);

			HRESULT hr;
			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.
			hr = self.pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			// Perform error handling here!

			// Get buffer and create a render-target-view.
			SafeRelease(self.pDepthStencilBuffer);
			ID3D11Texture2D* pBuffer;
			hr = self.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
			// Perform error handling here!

			hr = GetDevice()->CreateRenderTargetView(pBuffer, NULL, &self.pRenderTargetView);
			// Perform error handling here!
			SafeRelease(pBuffer);
			//pBuffer->Release();

			GetContext()->OMSetRenderTargets(1, &self.pRenderTargetView, NULL);

			SafeRelease(self.pDepthStencilView);
			SafeRelease(self.pDepthStencilBuffer);
			SafeRelease(self.pDepthStencilState);
			SafeRelease(self.pRasterizerState);

			//self.InitSwapChain();
			//self.InitBackBuffer();
			self.InitDepthStencilBuffer();
			self.InitDepthStencilState();
			self.InitRasterizerState();
			//InitViewport();

			// Set up the viewport.
			self.viewport.Width = (float)self.windowWidth;
			self.viewport.Height = (float)self.windowHeight;
			self.viewport.MinDepth = 0.0f;
			self.viewport.MaxDepth = 1.0f;
			self.viewport.TopLeftX = 0;
			self.viewport.TopLeftY = 0;
			GetContext()->RSSetViewports(1, &self.viewport);

			CameraManager::GetCurrentCamera()->SetAspectRatio((float)self.windowWidth / (float)self.windowHeight);
			CameraManager::GetCurrentCamera2D()->SetViewport(0, 0, self.windowWidth, self.windowHeight);
			CameraManager::GetCurrentCamera2D()->SetOrthographic(
				(float)-CameraManager::GetCurrentCamera2D()->GetScreenWidth() / 2.0f, (float)CameraManager::GetCurrentCamera2D()->GetScreenWidth() / 2.0f,
				(float)-CameraManager::GetCurrentCamera2D()->GetScreenHeight() / 2.0f, (float)CameraManager::GetCurrentCamera2D()->GetScreenHeight() / 2.0f,
				1.0f, 1000.0f
			);
		}
	}

	ID3D11DepthStencilView* Engine::GetDepthStencilView()
	{
		return GetEngineInstance().pDepthStencilView;
	}

	bool Engine::GetEditorMode()
	{
		return GetEngineInstance().editorMode;
	}

	void Engine::SetEditorMode(bool enabled)
	{
		Engine& self = GetEngineInstance();

		GetEngineInstance().editorMode = enabled;

		if (enabled)
		{
			if (self.pCurrentState != self.pEditorState)
			{
				self.pCurrentState->Exit();
				self.pCurrentState->TransitionToEditor();
				self.pCurrentState = self.pEditorState;
				self.pCurrentState->Enter();
			}
		}
		else
		{
			if (self.pCurrentState != self.pPlayState)
			{
				self.pCurrentState->Exit();
				self.pCurrentState->TransitionToPlay();
				self.pCurrentState = self.pPlayState;
				self.pCurrentState->Enter();
			}
		}
	}

	float Engine::GetDeltaTime()
	{
		return GetEngineInstance().storedDeltaTime;
	}

	void Engine::NativeUpdate(float deltaTime)
	{
		GetEngineInstance().Update(deltaTime);
	}

	void Engine::NativeRender()
	{
		GetEngineInstance().Render();
	}

	Engine& Engine::GetEngineInstance()
	{
		assert(pEngineInstance);
		return *pEngineInstance;
	}

	void Engine::Cleanup()
	{
		SafeRelease(pDepthStencilView);
		SafeRelease(pRenderTargetView);
		SafeRelease(pDepthStencilBuffer);
		SafeRelease(pDepthStencilState);
		SafeRelease(pRasterizerState);
		SafeRelease(pSwapChain);
		SafeRelease(pContext);

		EditorGui::Cleanup();

#ifdef _DEBUG
		HRESULT hr = S_OK;

		// Now we set up the Debug interface, to be queried on shutdown
		ID3D11Debug* debugDev = nullptr;
		hr = pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));

		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);
		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		//debugDev->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
		SafeRelease(debugDev);
#endif

		SafeRelease(pDevice);
	}
}