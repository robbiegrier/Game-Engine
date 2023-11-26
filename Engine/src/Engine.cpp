#include <d3d11.h>
#include <d3dcompiler.h>

#include "Engine.h"
#include "MathEngine.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

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
		ImGui_ImplDX11_Init(pDevice, pContext);
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
		RECT windowRect = { 0, 0, windowWidth, windowHeight };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		pWindowHandle = CreateWindowA(g_WindowClassName, pName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, pInstanceHandle, nullptr);
		assert(pWindowHandle);

		ShowWindow(pWindowHandle, cmdShow);
		UpdateWindow(pWindowHandle);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(pWindowHandle);
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

		/*bool show_demo_window = true;
		bool show_another_window = false;*/
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
				DWORD currentTime = timeGetTime();
				float deltaTime = (currentTime - previousTime) / 1000.0f;
				previousTime = currentTime;

				// Cap the delta time to the max time step (useful if your
				// debugging and you don't want the deltaTime value to explode.
				//deltaTime = std::min<float>(deltaTime, maxTimeStep);
				SetDefaultTargetMode();

				// Handle window resize (we don't resize directly in the WM_SIZE handler)
			/*	if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
				{
					CleanupRenderTarget();
					pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
					g_ResizeWidth = g_ResizeHeight = 0;
					CreateRenderTarget();
				}*/

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();

				//if (show_demo_window)
				//	ImGui::ShowDemoWindow(&show_demo_window); // Show demo window! :)
				//{
				//	static float f = 0.0f;
				//	static int counter = 0;

				//	ImGui::Begin("Objects");
				//	ImGui::Text("Welcome to the editor!");
				//	ImGui::Checkbox("Demo Window", &show_demo_window);
				//	ImGui::Checkbox("Another Window", &show_another_window);

				//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
				//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				//		counter++;
				//	ImGui::SameLine();
				//	ImGui::Text("counter = %d", counter);

				//	ImGuiIO& io = ImGui::GetIO();
				//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				//	ImGui::End();
				//}

				//if (show_another_window)
				//{
				//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
				//	ImGui::Text("Hello from another window!");
				//	if (ImGui::Button("Close Me"))
				//		show_another_window = false;
				//	ImGui::End();
				//}

				Update(deltaTime);
				Vec4 color = Vec4(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
				ClearDepthStencilBuffer(color);
				Render();
				ImGui::Render();

				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				LockFramerate(engineTime);

				Present();
				UpdateWindowName(deltaTime);
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
		assert(pDevice);
		assert(pContext);

		// Set (point to ) the Rasterizers functions to be used
		pContext->RSSetState(pRasterizerState);

		// Set (point to ) the Viewport to be used
		pContext->RSSetViewports(1, &viewport);

		// Set (point to ) render target: Only one Target, this maps to Pixel shader
		pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

		// Set (point to ) the Depth functions to be used
		pContext->OMSetDepthStencilState(pDepthStencilState, 1);
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

	void Engine::Resize(unsigned int w, unsigned int h)
	{
		Engine& self = GetEngineInstance();
		self.g_ResizeWidth = w;
		self.g_ResizeHeight = h;
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

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

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