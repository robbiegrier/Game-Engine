#ifndef _EditorGuiUtils
#define _EditorGuiUtils

#include "imgui.h"
#include "MathEngine.h"

namespace Azul
{
	class EditorGuiUtils
	{
	public:
		static void AlignForWidth(float width, float alignment = 0.5f);
		static void AlignForHeight(float height, float alignment = 0.5f);
	};
}

#endif