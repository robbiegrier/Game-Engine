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
#include "Mesh.h"
#include "TextureObject.h"

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
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		bool p_open = true;
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					p_open = false;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();

		EditorGui& self = GetInstance();

		bool open = true;
		ImGui::Begin("Scene", &open);

		bool selection = false;
		TreeBuildHelper(GameObjectManager::GetAllObjects().GetRoot()->GetChild(), selection);
		ImGui::End();

		Mesh::Name meshToSpawn = Mesh::Name::None;
		TextureObject::Name textureToSpawn = TextureObject::Name::Rocks;
		Vec3 spawnScale = Vec3(1, 1, 1);

		ImGui::Begin("Pallete");
		if (ImGui::Button("Unit Cube")) meshToSpawn = Mesh::Name::Cube;
		if (ImGui::Button("Unit Sphere")) meshToSpawn = Mesh::Name::Sphere;
		if (ImGui::Button("Unit Pyramid")) meshToSpawn = Mesh::Name::Pyramid;
		if (ImGui::Button("Wooden Crate"))
		{
			meshToSpawn = Mesh::Name::Crate;
			textureToSpawn = TextureObject::Name::Crate;
		}
		if (ImGui::Button("Chicken Bot"))
		{
			meshToSpawn = Mesh::Name::Bone;
			textureToSpawn = TextureObject::Name::ChickenBot;
			spawnScale = Vec3(300, 300, 300);
		}
		if (ImGui::Button("Desert Rock 0")) { meshToSpawn = Mesh::Name::DesertRock0; textureToSpawn = TextureObject::Name::DesertRock0; }
		if (ImGui::Button("Desert Rock 1")) { meshToSpawn = Mesh::Name::DesertRock1; textureToSpawn = TextureObject::Name::DesertRock1; }
		if (ImGui::Button("Desert Rock 2")) { meshToSpawn = Mesh::Name::DesertRock2; textureToSpawn = TextureObject::Name::DesertRock2; }
		if (ImGui::Button("Desert Rock 3")) { meshToSpawn = Mesh::Name::DesertRock3; textureToSpawn = TextureObject::Name::DesertRock3; }
		if (ImGui::Button("Desert Rock 4")) { meshToSpawn = Mesh::Name::DesertRock4; textureToSpawn = TextureObject::Name::DesertRock4; }
		if (ImGui::Button("Desert Rock 5")) { meshToSpawn = Mesh::Name::DesertRock5; textureToSpawn = TextureObject::Name::DesertRock5; }
		if (ImGui::Button("Desert Rock 6")) { meshToSpawn = Mesh::Name::DesertRock6; textureToSpawn = TextureObject::Name::DesertRock6; }
		if (ImGui::Button("Desert Rock 7")) { meshToSpawn = Mesh::Name::DesertRock7; textureToSpawn = TextureObject::Name::DesertRock7; }
		if (ImGui::Button("Desert Rock 8")) { meshToSpawn = Mesh::Name::DesertRock8; textureToSpawn = TextureObject::Name::DesertRock8; }
		if (ImGui::Button("Desert Rock 9")) { meshToSpawn = Mesh::Name::DesertRock9; textureToSpawn = TextureObject::Name::DesertRock9; }
		if (ImGui::Button("Desert Rock 10")) { meshToSpawn = Mesh::Name::DesertRock10; textureToSpawn = TextureObject::Name::DesertRock10; }
		if (ImGui::Button("Desert Rock 11")) { meshToSpawn = Mesh::Name::DesertRock11; textureToSpawn = TextureObject::Name::DesertRock11; }
		if (ImGui::Button("Desert Rock 12")) { meshToSpawn = Mesh::Name::DesertRock12; textureToSpawn = TextureObject::Name::DesertRock12; }
		if (ImGui::Button("Desert Rock 13")) { meshToSpawn = Mesh::Name::DesertRock13; textureToSpawn = TextureObject::Name::DesertRock13; }

		ImGui::End();

		if (meshToSpawn != Mesh::Name::None)
		{
			static int spawned = 0;
			static float spawnDistance = 10.f;
			Vec3 spawnPos = CameraManager::GetCurrentCamera()->GetLocation() + (-CameraManager::GetCurrentCamera()->GetDirection().getNorm() * spawnDistance);

			std::string strName = std::string("Spawned Object ") + std::to_string(++spawned);

			GameObjectManager::SpawnObject(strName.c_str(),
				meshToSpawn,
				textureToSpawn,
				spawnPos
			)->SetRelativeScale(spawnScale);
		}

		ImGui::Begin("World", &open);

		static ImVec2 windowSize = ImVec2{ -1.f, -1.f };
		ImVec2 windowSizeTmp = ImGui::GetWindowSize();

		if ((fabs(fabs(windowSize.x) - fabs(windowSizeTmp.x)) > MATH_TOLERANCE) || (fabs(fabs(windowSize.y) - fabs(windowSizeTmp.y)) > MATH_TOLERANCE))
		{
			self.pWorldViewport->Resize((UINT)ImGui::GetWindowWidth(), (UINT)ImGui::GetWindowHeight());
			CameraManager::GetCurrentCamera()->SetAspectRatio((float)self.pWorldViewport->GetWidth() / (float)self.pWorldViewport->GetHeight());
		}

		windowSize = ImGui::GetWindowSize();

		AlignForWidth((float)self.pWorldViewport->GetWidth());
		AlignForHeight((float)self.pWorldViewport->GetHeight());

		ImGui::Image(
			self.pWorldViewport->GetShaderResourceView(),
			ImVec2((float)self.pWorldViewport->GetWidth(), (float)self.pWorldViewport->GetHeight()),
			ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
			ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
			ImVec4(0.f, 0.f, 0.f, 0.f)
		);
		ImGui::End();

		ImGui::Begin("Console");
		ImGui::Text("Put console here");
		ImGui::End();

		ImGui::Begin("Inspector");

		if (pSelection)
		{
			char* pName = new char[PCSNode::NAME_SIZE];
			pSelection->GetName(pName, PCSNode::NAME_SIZE);
			ImGui::Text(pName);
			delete pName;

			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::SeparatorText("Translation");
				ImGui::DragFloat3("Translate", (float*)&pSelection->RelativeLocation(), 0.5f);
				ImGui::SeparatorText("Rotation");
				ImGui::DragFloat4("Rotate", (float*)&pSelection->RelativeRotation(), 0.01f, -1.f, 1.f);
				ImGui::SeparatorText("Scale");
				ImGui::DragFloat3("Scale", (float*)&pSelection->RelativeScale(), 0.5f);
			}
			if (ImGui::CollapsingHeader("Graphics"))
			{
				GraphicsObject* pGO = pSelection->GetGraphicsObject();

				ImGui::BulletText("Model: %s", pGO->GetModel() ? pGO->GetModel()->NameToString() : "nullptr");
				ImGui::BulletText("Shader: %s", pGO->GetModel() ? pGO->GetShader()->NameToString() : "nullptr");
			}

			pSelection->SetShellColor(Vec4(1.f, 0.2f, 0.2f, 1.f));
		}
		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	void EditorGui::TreeBuildHelper(PCSNode* pNode, bool& selection)
	{
		if (pNode)
		{
			char* name = new char[64];
			pNode->GetName(name, 64);
			GameObject* pObject = GameObjectManager::FindObject(name);

			bool isCurrentlySelected = pObject != nullptr && pObject == pSelection;

			if (pNode->GetChild())
			{
				ImGuiTreeNodeFlags flags{ 0 };

				if (isCurrentlySelected) flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;

				bool treeOpen = ImGui::TreeNodeEx(name, flags);

				if (IsGoToRequested() && !selection)
				{
					selection = true;

					if (pObject)
					{
						CameraManager::GetCurrentCamera()->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), pObject->GetLocation(),
							pObject->GetLocation() + Vec3(0, 8, 15));

						pSelection = pObject;
					}
				}

				if (ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) && !selection)
				{
					pSelection = pObject;
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
				if (ImGui::Selectable(name, isCurrentlySelected))
				{
				}
				if (IsGoToRequested() && !selection)
				{
					selection = true;

					if (pObject)
					{
						CameraManager::GetCurrentCamera()->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), pObject->GetLocation(),
							pObject->GetLocation() + Vec3(0, 8, 15));
					}
				}

				if (ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) && !selection)
				{
					pSelection = pObject;
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
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.Fonts->AddFontFromFileTTF("../Models/Ubuntu-Regular.ttf", 18.f);

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

		if (self.pSelection)
		{
			self.pSelection->RenderShell();
		}
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