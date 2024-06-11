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
		static std::string FileToString(const char* const pMeshFileName);

		template<typename T>
		static T Lerp(T a, T b, float t)
		{
			return a + ((b - a) * t);
		}
	};
}

// Convenience macro for releasing COM objects.
#define SafeRelease(x) { if(x){ x->Release(); x = 0; } }

#endif