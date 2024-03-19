#include "WorldWindow.h"
#include "EditorGui.h"
#include "Viewport.h"
#include "CameraManager.h"
#include "EditorGuiUtils.h"

namespace Azul
{
	void WorldWindow::Update()
	{
		ImGui::SetNextWindowScroll(ImVec2(.5f, .5f));

		ImGuiWindowFlags worldWindowFlags{ 0 };
		worldWindowFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
		worldWindowFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollWithMouse;
		ImGui::Begin("World", &open, worldWindowFlags);
		isWorldInFocus = ImGui::IsWindowFocused();
		static ImVec2 windowSize = ImVec2{ -1.f, -1.f };
		ImVec2 windowSizeTmp = ImGui::GetWindowSize();

		ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
		ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

		screenMin.x = contentMin.x + ImGui::GetWindowPos().x;
		screenMin.y = contentMin.y + ImGui::GetWindowPos().y;
		screenMax.x = contentMax.x + ImGui::GetWindowPos().x;
		screenMax.y = contentMax.y + ImGui::GetWindowPos().y;

		float desiredWidth = contentMax.x - contentMin.x;
		float desiredHeight = contentMax.y - contentMin.y;

		if ((fabs(fabs(windowSize.x) - fabs(windowSizeTmp.x)) > MATH_TOLERANCE) || (fabs(fabs(windowSize.y) - fabs(windowSizeTmp.y)) > MATH_TOLERANCE))
		{
			EditorGui::GetWorldViewport()->Resize((UINT)desiredWidth, (UINT)desiredHeight);
			CameraManager::GetCurrentCamera()->SetAspectRatio((float)EditorGui::GetWorldViewport()->GetWidth() / (float)EditorGui::GetWorldViewport()->GetHeight());
		}

		windowSize = ImGui::GetWindowSize();

		EditorGuiUtils::AlignForWidth(desiredWidth);
		EditorGuiUtils::AlignForHeight(desiredHeight);

		ImGui::Image(
			EditorGui::GetWorldViewport()->GetShaderResourceView(),
			ImVec2((float)EditorGui::GetWorldViewport()->GetWidth(), (float)EditorGui::GetWorldViewport()->GetHeight()),
			ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
			ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
			ImVec4(0.f, 0.f, 0.f, 0.f)
		);
		ImGui::End();
	}
}