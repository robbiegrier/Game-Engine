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

namespace Azul
{
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
		ImGui::Text("width: %d, height: %d", worldWidth, worldHeight);

		if (ImGui::Button("bigify"))
		{
			InitializeWindow(worldWidth + 10, worldHeight + 10);
		}

		ImGui::Image(
			self.shaderResourceViewMap,
			ImVec2((float)worldWidth, (float)worldHeight),
			ImVec2(0.0f, 0.0f),
			ImVec2(1.0f, 1.0f),
			ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
			ImGui::GetStyleColorVec4(ImGuiCol_Border)
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

		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		///////////////////////// Map's Texture
		// Initialize the  texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		EditorGui& self = GetInstance();
		static const UINT scaleDiv = 2;
		self.worldWidth = Engine::GetWindowWidth() / scaleDiv;
		self.worldHeight = Engine::GetWindowHeight() / scaleDiv;

		// Setup the texture description.
		// We will have our map be a square
		// We will need to have this texture bound as a render target AND a shader resource
		textureDesc.Width = self.worldWidth;
		textureDesc.Height = self.worldHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the texture
		Engine::GetDevice()->CreateTexture2D(&textureDesc, NULL, &self.renderTargetTextureMap);

		/////////////////////// Map's Render Target
		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		Engine::GetDevice()->CreateRenderTargetView(self.renderTargetTextureMap, &renderTargetViewDesc, &self.renderTargetViewMap);

		/////////////////////// Map's Shader Resource View
		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		Engine::GetDevice()->CreateShaderResourceView(self.renderTargetTextureMap, &shaderResourceViewDesc, &self.shaderResourceViewMap);

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc{ 0 };
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilBufferDesc.Width = self.worldWidth;
		depthStencilBufferDesc.Height = self.worldHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr = Engine::GetDevice()->CreateTexture2D(&depthStencilBufferDesc, nullptr, &self.pDepthStencilBuffer);
		assert(SUCCEEDED(hr));

		hr = Engine::GetDevice()->CreateDepthStencilView(self.pDepthStencilBuffer, nullptr, &self.pDepthStencilView);
		assert(SUCCEEDED(hr));

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilStateDesc.StencilEnable = FALSE;

		hr = Engine::GetDevice()->CreateDepthStencilState(&depthStencilStateDesc, &self.pDepthStencilState);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		self.viewport.Width = static_cast<float>(self.worldWidth);
		self.viewport.Height = static_cast<float>(self.worldHeight);
		self.viewport.TopLeftX = 0.0f;
		self.viewport.TopLeftY = 0.0f;
		self.viewport.MinDepth = 0.0f;
		self.viewport.MaxDepth = 1.0f;
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

		Engine::GetContext()->RSSetViewports(1, &self.viewport);

		Engine::GetContext()->OMSetRenderTargets(1, &self.renderTargetViewMap, self.pDepthStencilView);

		// Set (point to ) the Depth functions to be used
		Engine::GetContext()->OMSetDepthStencilState(self.pDepthStencilState, 1);

		//// Now clear the render target
		Vec4 col = Vec4(1, .5, .5, 1);
		Engine::GetContext()->ClearRenderTargetView(self.renderTargetViewMap, (const FLOAT*)&col);
		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;
		Engine::GetContext()->ClearDepthStencilView(self.pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, (FLOAT)clearDepth, (UINT8)clearStencil);

		GameObjectManager::Draw();
	}

	void EditorGui::Cleanup()
	{
		EditorGui& self = GetInstance();
		SafeRelease(self.renderTargetTextureMap);
		SafeRelease(self.renderTargetViewMap);
		SafeRelease(self.shaderResourceViewMap);
		SafeRelease(self.pDepthStencilBuffer);
		SafeRelease(self.pDepthStencilView);
		SafeRelease(self.pDepthStencilState);

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorGui::InitializeWindow(UINT width, UINT height)
	{
		EditorGui& self = GetInstance();
		self.worldWidth = width;
		self.worldHeight = height;

		SafeRelease(self.renderTargetTextureMap);
		SafeRelease(self.renderTargetViewMap);
		SafeRelease(self.shaderResourceViewMap);
		SafeRelease(self.pDepthStencilBuffer);
		SafeRelease(self.pDepthStencilView);
		SafeRelease(self.pDepthStencilState);

		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		///////////////////////// Map's Texture
		// Initialize the  texture description.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the texture description.
		// We will have our map be a square
		// We will need to have this texture bound as a render target AND a shader resource
		textureDesc.Width = self.worldWidth;
		textureDesc.Height = self.worldHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the texture
		Engine::GetDevice()->CreateTexture2D(&textureDesc, NULL, &self.renderTargetTextureMap);

		/////////////////////// Map's Render Target
		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		Engine::GetDevice()->CreateRenderTargetView(self.renderTargetTextureMap, &renderTargetViewDesc, &self.renderTargetViewMap);

		/////////////////////// Map's Shader Resource View
		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		Engine::GetDevice()->CreateShaderResourceView(self.renderTargetTextureMap, &shaderResourceViewDesc, &self.shaderResourceViewMap);

		D3D11_TEXTURE2D_DESC depthStencilBufferDesc{ 0 };
		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0;
		depthStencilBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilBufferDesc.Width = self.worldWidth;
		depthStencilBufferDesc.Height = self.worldHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr = Engine::GetDevice()->CreateTexture2D(&depthStencilBufferDesc, nullptr, &self.pDepthStencilBuffer);
		assert(SUCCEEDED(hr));

		hr = Engine::GetDevice()->CreateDepthStencilView(self.pDepthStencilBuffer, nullptr, &self.pDepthStencilView);
		assert(SUCCEEDED(hr));

		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ 0 };
		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilStateDesc.StencilEnable = FALSE;

		hr = Engine::GetDevice()->CreateDepthStencilState(&depthStencilStateDesc, &self.pDepthStencilState);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		self.viewport.Width = static_cast<float>(self.worldWidth);
		self.viewport.Height = static_cast<float>(self.worldHeight);
		self.viewport.TopLeftX = 0.0f;
		self.viewport.TopLeftY = 0.0f;
		self.viewport.MinDepth = 0.0f;
		self.viewport.MaxDepth = 1.0f;
	}

	EditorGui& EditorGui::GetInstance()
	{
		static EditorGui instance;
		return instance;
	}
}