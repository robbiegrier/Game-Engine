#include "ConsoleWindow.h"
#include "imgui.h"
#include "EditorGui.h"

namespace Azul
{
	void ConsoleWindow::Update()
	{
		ImGui::Begin("Console");
		ImGui::Text(text.c_str());
		ImGui::End();
	}

	void ConsoleWindow::Log(const char* fmt, ...)
	{
		va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
		va_start(args, fmt);
#pragma warning( pop )

		char privBuff[1024];

		vsprintf_s(&privBuff[0], 1024, fmt, args);

		text += privBuff;
		text += '\n';

		Trace::out(privBuff);
		Trace::out("\n");
	}
}