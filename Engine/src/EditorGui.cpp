#include "EditorGui.h"
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
#include "EditorThemes.h"
#include "PCSTreeForwardIterator.h"
#include "EditorInput.h"
#include "EditorObjectReference.h"
#include "EditorVisual.h"
#include "EditActionSpawn.h"
#include "SceneManager.h"
#include "EditorGuiUtils.h"
#include "EditorMath.h"
#include <DirectXTex.h>

namespace Azul
{
	bool EditorGui::HandleEditorVisualInteraction()
	{
		if (!selection.IsEmpty())
		{
			EditorVisual* pVisualIntersecting = GetEditorVisualIntersecting();

			if (pVisualIntersecting)
			{
				pVizDragging = pVisualIntersecting;
				pVizDragging->OnDragEnter();
				return true;
			}
		}

		return false;
	}

	bool EditorGui::HandleObjectSelection()
	{
		Camera* pCamera = CameraManager::GetCurrentCamera();
		ImVec2 mousePosRelativeCenterNorm = GetNormalizedMousePosition(ImGui::GetMousePos());
		Vec3 rayDir = pCamera->GetRay(mousePosRelativeCenterNorm.x, mousePosRelativeCenterNorm.y);
		GameObject* pClosestObject = GetClosestObjectIntersecting(pCamera->GetLocation(), rayDir);
		UpdateSelectionOnClick(EditorInput::GetModState(), pClosestObject);
		return pClosestObject != nullptr;
	}

	void EditorGui::OnLeftClickPressed()
	{
		if (IsMouseInWorld())
		{
			if (!HandleEditorVisualInteraction())
			{
				HandleObjectSelection();
			}
		}
	}

	void EditorGui::OnLeftClickReleased()
	{
		if (IsDraggingSomething())
		{
			ReleaseDrag();
		}
	}

	void EditorGui::OnLeftClickDragInProgress(const MousePosition& start, const MousePosition& curr)
	{
		if (IsDraggingSomething())
		{
			pVizDragging->OnDragUpdate();
		}
		else if (IsMouseInWorld())
		{
			ImGui::GetForegroundDrawList()->AddRect(ImVec2((float)start.x, (float)start.y), ImVec2((float)curr.x, (float)curr.y), IM_COL32(255, 191, 0, 255), 0, ImDrawFlags(), 2);
			ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2((float)start.x, (float)start.y), ImVec2((float)curr.x, (float)curr.y), IM_COL32(255, 191, 0, 40));
		}
	}

	bool EditorGui::IsMouseInWorld()
	{
		const ImVec2 mouseNormalizedPos = GetNormalizedMousePosition(ImGui::GetMousePos());
		return fabsf(mouseNormalizedPos.x) < 1.0f && fabsf(mouseNormalizedPos.y) < 1.0f;
	}

	bool EditorGui::IsDraggingSomething()
	{
		return GetInstance().pVizDragging != nullptr;
	}

	bool EditorGui::IsWorldInFocus()
	{
		return GetInstance().world.isWorldInFocus;
	}

	EditorVisual* EditorGui::GetDraggingVisual()
	{
		return GetInstance().pVizDragging;
	}

	void EditorGui::OnLeftClickDragEnd(const MousePosition& start, const MousePosition& end)
	{
		MousePosition rectPrime0(end.x, start.y);
		MousePosition rectPrime1(start.x, end.y);

		MousePosition points[4];
		points[0] = start;
		points[1] = end;
		points[2] = rectPrime0;
		points[3] = rectPrime1;

		SortMousePointsClockwise(points);

		// CONVERT TO NORMALIZED COORDS

		//Camera* pCamera = CameraManager::GetCurrentCamera();

		//Vec3 ray0 = pCamera->GetRay((float)start.x, (float)start.y);
		//Vec3 ray1 = pCamera->GetRay((float)rectPrime0.x, (float)rectPrime0.y);
		//Vec3 ray2 = pCamera->GetRay((float)rectPrime1.x, (float)rectPrime1.y);
		//Vec3 ray3 = pCamera->GetRay((float)end.x, (float)end.y);

		//Trace::out("Observed Drag End\n");
	}

	void EditorGui::OnScroll(int deltaMove)
	{
		if (IsWorldInFocus() && IsMouseInWorld())
		{
			Camera* pCamera = CameraManager::GetCurrentCamera();
			Vec3 cameraMove(0.f, 0.f, 1.f);

			pCamera->AddLocationOffset(cameraMove * static_cast<float>(deltaMove) * Engine::GetDeltaTime() * SCROLL_SPEED);
		}
	}

	void EditorGui::OnTranslateModePressed()
	{
		if (!IsDraggingSomething())
		{
			selection.EnterTranslateMode();
		}
	}

	void EditorGui::OnScaleModePressed()
	{
		if (!IsDraggingSomething())
		{
			selection.EnterScaleMode();
		}
	}

	void EditorGui::OnRotateModePressed()
	{
		if (!IsDraggingSomething())
		{
			selection.EnterRotateMode();
		}
	}

	void EditorGui::OnDeletePressed()
	{
		GetInstance().selection.DeleteObjects();
	}

	void EditorGui::OnCutPressed()
	{
		if (IsWorldInFocus())
		{
			clipboard.Copy(selection);
			GetInstance().selection.DeleteObjects();
		}
	}

	void EditorGui::OnCopyPressed()
	{
		if (IsWorldInFocus())
		{
			clipboard.Copy(selection);
		}
	}

	void EditorGui::OnPastePressed()
	{
		if (IsWorldInFocus())
		{
			clipboard.Paste(GetPointInFrontOfView());
		}
	}

	void EditorGui::OnSavePressed()
	{
		SceneManager::SaveScene("AzulScene");
		Console("Save Complete!");
	}

	void EditorGui::OnLoadPressed()
	{
		SceneManager::ChangeScene("AzulScene");
	}

	void EditorGui::SortMousePointsClockwise(MousePosition* arr)
	{
		MousePosition topLeft;
		MousePosition topRight;
		MousePosition botLeft;
		MousePosition botRight;

		int minX;
		int minY;
		int maxX;
		int maxY;

		// Top Left - min x and min y
		minX = std::numeric_limits<int>::max();
		minY = std::numeric_limits<int>::max();
		for (int i = 0; i < 4; i++)
		{
			if (arr[i].x <= minX && arr[i].y <= minY)
			{
				minX = arr[i].x;
				minY = arr[i].y;
				topLeft = arr[i];
			}
		}

		// Top Right - max x and min y
		maxX = std::numeric_limits<int>::min();
		minY = std::numeric_limits<int>::max();
		for (int i = 0; i < 4; i++)
		{
			if (arr[i].x >= maxX && arr[i].y <= minY)
			{
				maxX = arr[i].x;
				minY = arr[i].y;
				topRight = arr[i];
			}
		}

		// Bot Right - max x and max y
		maxX = std::numeric_limits<int>::min();
		maxY = std::numeric_limits<int>::min();
		for (int i = 0; i < 4; i++)
		{
			if (arr[i].x >= maxX && arr[i].y >= maxY)
			{
				maxX = arr[i].x;
				maxY = arr[i].y;
				botRight = arr[i];
			}
		}

		// Bot Left - min x and max y
		minX = std::numeric_limits<int>::max();
		maxY = std::numeric_limits<int>::min();
		for (int i = 0; i < 4; i++)
		{
			if (arr[i].x <= minX && arr[i].y >= maxY)
			{
				minX = arr[i].x;
				maxY = arr[i].y;
				botLeft = arr[i];
			}
		}

		arr[0] = topLeft;
		arr[1] = topRight;
		arr[2] = botRight;
		arr[3] = botLeft;
	}

	ImVec2 EditorGui::GetNormalizedMousePosition(const ImVec2& mousePos)
	{
		EditorGui& self = GetInstance();

		ImVec2 mousePosRelative = mousePos;
		mousePosRelative.x -= self.world.screenMin.x;
		mousePosRelative.y -= self.world.screenMin.y;

		Vec3 screenCenter;
		screenCenter[x] = (self.world.screenMax.x - self.world.screenMin.x) * .5f;
		screenCenter[y] = (self.world.screenMax.y - self.world.screenMin.y) * .5f;

		ImVec2 mousePosRelativeCenter = mousePosRelative;
		mousePosRelativeCenter.x -= screenCenter[x];
		mousePosRelativeCenter.y -= screenCenter[y];
		mousePosRelativeCenter.y = -mousePosRelativeCenter.y;

		const float desiredWidth = self.world.screenMax.x - self.world.screenMin.x;
		const float desiredHeight = self.world.screenMax.y - self.world.screenMin.y;

		ImVec2 mousePosRelativeCenterNorm = mousePosRelativeCenter;
		mousePosRelativeCenterNorm.x /= desiredWidth;
		mousePosRelativeCenterNorm.y /= desiredHeight;
		mousePosRelativeCenterNorm.x *= 2.f;
		mousePosRelativeCenterNorm.y *= 2.f;

		return mousePosRelativeCenterNorm;
	}

	GameObject* EditorGui::GetClosestObjectIntersecting(const Vec3& rayOrigin, const Vec3& rayDir)
	{
		float closestDistance = std::numeric_limits<float>().max();
		GameObject* pClosestObject = nullptr;

		for (PCSTreeForwardIterator it(GameObjectManager::GetAllObjects().GetRoot()); !it.IsDone(); it.Next())
		{
			GameObject* pCurr = (GameObject*)it.Current();

			if (pCurr->IsSelectable())
			{
				const float sphereRadius = pCurr->GetShellRadius();
				const Vec3& sphereCenter = pCurr->GetShellCenter();

				if (EditorMath::RayVsSphere(rayOrigin, rayDir, sphereCenter, sphereRadius))
				{
					const float dist = (rayOrigin - pCurr->GetWorldLocation()).len();

					if (dist < closestDistance)
					{
						closestDistance = dist;
						pClosestObject = pCurr;
					}
				}
			}
		}

		return pClosestObject;
	}

	EditorVisual* EditorGui::GetEditorVisualIntersecting()
	{
		EditorGui& self = GetInstance();

		float closestDistance = std::numeric_limits<float>().max();
		EditorVisual* pVisual = nullptr;

		for (Iterator& it = *self.selection.GetEditorVisuals().GetIterator(); !it.IsDone(); it.Next())
		{
			EditorVisual* pCurr = (EditorVisual*)it.Curr();

			const Vec3& camPos = CameraManager::GetCurrentCamera()->GetLocation();
			const float distCam = (camPos - pCurr->GetLocation()).len();
			const float scaledTolerance = std::max(distCam * 0.1f, 1.f) * WIDGET_TOLERANCE;

			if (pCurr->IsMouseOver(scaledTolerance))
			{
				const float dist = pCurr->GetMouseOverDistance();

				if (dist < closestDistance)
				{
					closestDistance = dist;
					pVisual = pCurr;
				}
			}
		}

		return pVisual;
	}

	void EditorGui::UpdateSelectionOnClick(InputModifier mod, GameObject* pObjectClicked)
	{
		EditorGui& self = GetInstance();

		if (pObjectClicked)
		{
			if (mod == InputModifier::None)
			{
				self.selection.Clear();
				self.selection.AddObject(pObjectClicked);
			}
			else if (mod == InputModifier::Control)
			{
				self.selection.AddObject(pObjectClicked);
			}
		}
		else
		{
			self.selection.Clear();
		}
	}

	void EditorGui::Console(const char* msg)
	{
		GetInstance().console.Log(msg);
	}

	Selection& EditorGui::GetSelection()
	{
		return GetInstance().selection;
	}

	void EditorGui::ReleaseDrag()
	{
		pVizDragging->OnDragExit();
		pVizDragging = nullptr;
	}

	EditorController& EditorGui::Controller()
	{
		return GetInstance().controller;
	}

	Vec3 EditorGui::GetPointInFrontOfView(float dist)
	{
		return CameraManager::GetCurrentCamera()->GetLocation() + CameraManager::GetCurrentCamera()->GetDirection() * -dist;
	}

	void EditorGui::OnGameObjectRemoved(GameObject* pObject)
	{
		GetInstance().selection.RemoveObject(pObject);
	}

	bool EditorGui::IsGoToRequested()
	{
		return ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Middle) ||
			ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
	}

	void EditorGui::Initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.Fonts->AddFontFromFileTTF("../Models/Fredoka-Regular.ttf", 18.f);
		io.Fonts->AddFontFromFileTTF("../Models/Ubuntu-Regular.ttf", 18.f);
		io.Fonts->Build();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(Engine::GetWindowHandle());
		ImGui_ImplDX11_Init(Engine::GetDevice(), Engine::GetContext());

		EditorGui& self = GetInstance();

		static const UINT scaleDiv = 2;
		self.pWorldViewport = new Viewport(Engine::GetWindowWidth() / scaleDiv, Engine::GetWindowHeight() / scaleDiv);

		EditorInput::AddObserver(&self);
	}

	void EditorGui::NewFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		GetInstance().Update();
	}

	void EditorGui::Update()
	{
		selection.Update();
		UpdateDockspace();
		console.Update();
		hierarchy.Update();
		settings.Update();
		inspector.Update();
		world.Update();
		assets.Update();
	}

	void EditorGui::Draw()
	{
		EditorGui& self = GetInstance();

		self.pWorldViewport->Activate();

		GameObjectManager::Draw();
		self.selection.Draw();
		self.pWorldViewport->Close();

		const bool fDown = (GetKeyState('F') & 0x8000);
		static bool switchf = false;

		if (fDown)
		{
			if (!switchf)
			{
				switchf = true;
				DirectX::ScratchImage scratchImage;
				ID3D11Resource* resource;
				//HRESULT hr = DirectX::CaptureTexture(Engine::GetDevice(), Engine::GetContext(), self.pWorldViewport->GetRenderTargetTexture(), scratchImage);
				//self.pWorldViewport->GetShaderResourceView()->GetResource(&resource);
				self.pWorldViewport->GetRenderTargetView()->GetResource(&resource);
				HRESULT hr = DirectX::CaptureTexture(Engine::GetDevice(), Engine::GetContext(), resource, scratchImage);

				assert(SUCCEEDED(hr));

				const wchar_t* file = L"VIEWPORT_RENDER.hdr";
				const DirectX::Image& image = scratchImage.GetImages()[0];
				hr = DirectX::SaveToHDRFile(image, file);
				assert(SUCCEEDED(hr));
				Trace::out("Save to %ls\n", file);
			}
		}
		else
		{
			switchf = false;
		}

		Engine::SetDefaultTargetMode();
		Engine::ClearDepthStencil({ 0.1f, 0.1f, 0.1f, 1.000000000f });

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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

	// ImGui Boilerplate
	void EditorGui::UpdateDockspace()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (fullscreen)
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
		if (!padding)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		}

		bool p_open = true;

		ImGui::Begin("DockSpace", &p_open, window_flags);

		if (!padding)
		{
			ImGui::PopStyleVar();
		}

		if (fullscreen)
		{
			ImGui::PopStyleVar(2);
		}

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Fullscreen", NULL, &fullscreen);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Fullscreen", NULL, &fullscreen);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Fullscreen", NULL, &fullscreen);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &fullscreen);
				ImGui::MenuItem("Padding", NULL, &padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL)) p_open = false;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	void Log(const char* fmt, ...)
	{
		va_list args;

#pragma warning( push )
#pragma warning( disable : 26492 )
#pragma warning( disable : 26481 )
		va_start(args, fmt);
#pragma warning( pop )

		char privBuff[1024];

		vsprintf_s(&privBuff[0], 1024, fmt, args);

		EditorGui::Console(privBuff);
	}
}