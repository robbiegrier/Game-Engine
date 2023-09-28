#ifndef _EngineUtils
#define _EngineUtils

#include <d3d11.h>

namespace Azul
{
	class EngineUtils
	{
	public:
		static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}

// Convenience macro for releasing COM objects.
#define SafeRelease(x) { if(x){ x->Release(); x = 0; } }

#endif