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
#include "TextureObjectManager.h"
#include "Viewport.h"

namespace Azul
{
	void EditorGui::AlignForWidth(float width, float alignment)
	{
		float avail = ImGui::GetContentRegionAvail().x;
		float off = (avail - width) * alignment;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
	}

	void EditorGui::AlignForHeight(float height, float alignment)
	{
		float avail = ImGui::GetContentRegionAvail().y;
		float off = (avail - height) * alignment;
		if (off > 0.0f)
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off);
	}

	void EditorGui::Update()
	{
		EditorGui& self = GetInstance();

		ImGui::ShowStyleEditor();

		bool open = true;
		ImGui::Begin("Objects", &open);

		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Scene"))
			{
				bool selection = false;
				TreeBuildHelper(GameObjectManager::GetAllObjects().GetRoot()->GetChild(), selection);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Pallete"))
			{
				static const ImVec2 buttonSize(100.f, 75.f);
				ImGui::Button("Unit Cube", buttonSize);
				ImGui::Button("Unit Sphere", buttonSize);
				ImGui::Button("Unit Pyramid", buttonSize);
				ImGui::Button("Wooden Crate", buttonSize);
				ImGui::Button("Chicken Bot", buttonSize);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();

		ImGui::ShowUserGuide();

		ImGui::Begin("World", &open);

		const UINT padding = 20;

		static ImVec2 windowSize = ImVec2{ -1.f, -1.f };
		ImVec2 windowSizeTmp = ImGui::GetWindowSize();
		//ImVec2 windowSizeTmp = ImVec2((float)self.pWorldViewport->GetWidth() + padding, (float)self.pWorldViewport->GetHeight() + padding);

		if ((fabs(fabs(windowSize.x) - fabs(windowSizeTmp.x)) > MATH_TOLERANCE) || (fabs(fabs(windowSize.y) - fabs(windowSizeTmp.y)) > MATH_TOLERANCE))
		{
			self.pWorldViewport->ResizeByWidth((UINT)ImGui::GetWindowWidth() - padding);

			if (ImGui::GetWindowHeight() < self.pWorldViewport->GetHeight())
			{
				self.pWorldViewport->ResizeByHeight((UINT)ImGui::GetWindowHeight() - padding);
			}
		}

		windowSize = ImGui::GetWindowSize();

		AlignForWidth((float)self.pWorldViewport->GetWidth());
		AlignForHeight((float)self.pWorldViewport->GetHeight());

		//ImGui::Text("width: %d, height: %d", self.pWorldViewport->GetWidth(), self.pWorldViewport->GetHeight());
		ImGui::Image(
			self.pWorldViewport->GetShaderResourceView(),
			ImVec2((float)self.pWorldViewport->GetWidth(), (float)self.pWorldViewport->GetHeight()),
			ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
			ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
			ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_BorderShadow)
		);
		ImGui::End();
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

		EditorGui& self = GetInstance();

		static const UINT scaleDiv = 2;
		self.pWorldViewport = new Viewport(Engine::GetWindowWidth() / scaleDiv, Engine::GetWindowHeight() / scaleDiv);
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
		EditorGui& self = GetInstance();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		self.pWorldViewport->Activate();
		GameObjectManager::Draw();
	}

	void EditorGui::Cleanup()
	{
		EditorGui& self = GetInstance();
		delete self.pWorldViewport;

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