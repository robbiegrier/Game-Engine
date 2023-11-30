#include "EditorGui.h"
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
#include "Engine.h"
#include "GameObjectManager.h"
#include "PCSTree.h"
#include "PCSNode.h"
#include "CameraManager.h"
#include "GameObject.h"

namespace Azul
{
	void EditorGui::Update()
	{
		ImGui::ShowStyleEditor();

		bool open = true;
		ImGui::Begin("Objects", &open);

		bool selection = false;
		TreeBuildHelper(GameObjectManager::GetAllObjects().GetRoot(), selection);
		ImGui::End();

		ImGui::ShowUserGuide();
	}

	void EditorGui::TreeBuildHelper(PCSNode* pNode, bool& selection)
	{
		if (pNode)
		{
			char* name = new char[64];
			pNode->GetName(name, 64);

			if (pNode->GetChild())
			{
				bool treeOpen = ImGui::TreeNode(name);

				if (IsGoToRequested() && !selection)
				{
					selection = true;

					GameObject* pObject = GameObjectManager::FindObject(name);

					if (pObject)
					{
						CameraManager::GetCurrentCamera()->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), pObject->GetLocation(),
							pObject->GetLocation() + Vec3(0, 8, 15));
					}
				}

				if (treeOpen)
				{
					TreeBuildHelper(pNode->GetChild(), selection);

					ImGui::TreePop();
				}
			}
			else
			{
				ImGui::Bullet();
				if (ImGui::Selectable(name))
				{
				}
				if (IsGoToRequested() && !selection)
				{
					selection = true;

					GameObject* pObject = GameObjectManager::FindObject(name);

					if (pObject)
					{
						CameraManager::GetCurrentCamera()->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), pObject->GetLocation(),
							pObject->GetLocation() + Vec3(0, 8, 15));
					}
				}
			}

			delete name;

			TreeBuildHelper(pNode->GetNextSibling(), selection);
		}
	};

	bool EditorGui::IsGoToRequested()
	{
		return ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Middle) ||
			ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
	}

	void EditorGui::Initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.Fonts->AddFontFromFileTTF("../Models/Ubuntu-Regular.ttf", 14.f);

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(Engine::GetWindowHandle());
		ImGui_ImplDX11_Init(Engine::GetDevice(), Engine::GetContext());
	}

	void EditorGui::NewFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		GetInstance().Update();
	}

	void EditorGui::Draw()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorGui::Cleanup()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	EditorGui& EditorGui::GetInstance()
	{
		static EditorGui instance;
		return instance;
	}
}