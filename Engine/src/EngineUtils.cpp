#include "EngineUtils.h"
#include "Engine.h"
#include "File.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "EditorInput.h"

namespace Azul
{
	// Find Refresh rate: http://www.rastertek.com/dx11tut03.html
	DXGI_RATIONAL EngineUtils::QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
	{
		DXGI_RATIONAL refreshRate = { 0, 1 };
		if (vsync)
		{
			IDXGIFactory* factory = nullptr;
			IDXGIAdapter* adapter;
			IDXGIOutput* adapterOutput;
			DXGI_MODE_DESC* displayModeList;

			// Create a DirectX graphics interface factory.
			HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Could not create DXGIFactory instance."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to create DXGIFactory.");
			}

			hr = factory->EnumAdapters(0, &adapter);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapters."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to enumerate adapters.");
			}

			hr = adapter->EnumOutputs(0, &adapterOutput);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapter outputs."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to enumerate adapter outputs.");
			}

			UINT numDisplayModes;
			hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to query display mode list."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to query display mode list.");
			}

			displayModeList = new DXGI_MODE_DESC[numDisplayModes];
			assert(displayModeList);

			hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to query display mode list."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				throw new std::exception("Failed to query display mode list.");
			}

			// Now store the refresh rate of the monitor that matches the width and height of the requested screen.
			for (UINT i = 0; i < numDisplayModes; ++i)
			{
				if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight)
				{
					refreshRate = displayModeList[i].RefreshRate;
				}
			}

			delete[] displayModeList;
			SafeRelease(adapterOutput);
			SafeRelease(adapter);
			SafeRelease(factory);
		}

		return refreshRate;
	}

	// windows process - messages
	LRESULT CALLBACK EngineUtils::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT paintStruct;
		HDC hDC;

		extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
			return true;

		switch (message)
		{
		case WM_PAINT:
		{
			hDC = BeginPaint(hwnd, &paintStruct);
			EndPaint(hwnd, &paintStruct);
		}
		break;

		case WM_CHAR:
		{
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			short wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			EditorInput::OnMouseWheelEvent((int)wheelDelta);
			Engine::OnMouseWheelEvent((int)wheelDelta);
		}
		break;

		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
				return 0;
			Engine::Resize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
			return 1;
			//case WM_SYSCOMMAND:
			//	if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			//		return 0;
			//	break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		return 0;
	}

	std::string EngineUtils::FileToString(const char* const pMeshFileName)
	{
		File::Handle fh;
		File::Error ferror;
		DWORD length;

		ferror = File::Open(fh, pMeshFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Position::END, 0);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Tell(fh, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Position::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		char* poBuff = new char[length];
		assert(poBuff);

		ferror = File::Read(fh, poBuff, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		std::string output = std::string(poBuff, length);
		delete[] poBuff;

		return output;
	}
}