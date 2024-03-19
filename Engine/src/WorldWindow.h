#ifndef _WorldWindow
#define _WorldWindow

#include "imgui.h"

namespace Azul
{
	// This is the window containing the world viewport.
	class WorldWindow
	{
	public:
		// Big four
		WorldWindow() = default;
		WorldWindow(const WorldWindow&) = delete;
		WorldWindow& operator=(const WorldWindow&) = delete;
		~WorldWindow() = default;

		void Update();

		bool isWorldInFocus = false;
		bool open;
		ImVec2 screenMin;
		ImVec2 screenMax;
	};
}

#endif