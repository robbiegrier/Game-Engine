#include "SettingsWindow.h"
#include "imgui.h"
#include "EditorThemes.h"

namespace Azul
{
	void SettingsWindow::Update()
	{
		ImGui::Begin("Settings");

		const char* themeChoices[] = { "Dark", "Light", "Retro", "Azul", "Amber" };
		static int currentChoice = 4;
		static int previousChoice = -1;
		ImGui::Combo("Color Themes", &currentChoice, themeChoices, IM_ARRAYSIZE(themeChoices));
		if (previousChoice != currentChoice)
		{
			if (currentChoice == 0) EditorThemes::Dark();
			else if (currentChoice == 1) EditorThemes::Light();
			else if (currentChoice == 2) EditorThemes::Retro();
			else if (currentChoice == 3) EditorThemes::Azul();
			else if (currentChoice == 4) EditorThemes::Amber();
		}
		previousChoice = currentChoice;

		ImGui::ShowStyleEditor();

		ImGui::End();
	}
}