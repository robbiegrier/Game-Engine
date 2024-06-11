#include "TerrainWindow.h"
#include "imgui.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "TerrainMesh.h"
#include "Terrain.h"
#include "CameraManager.h"
#include "EditorGui.h"
#include "TerrainSystem.h"
#include "TerrainArea.h"
#include "EditorInput.h"
#include "Engine.h"
#include "EditorMath.h"
#include "TerrainBrushNoise.h"
#include "ShaderObjectManager.h"
#include "SOTerrain.h"
#include "EditActionTerrain.h"
#include "TextureObjectManager.h"
#include "TerrainFoliage.h"
#include "SOFoliage.h"
#include "Viewport.h"

namespace Azul
{
	TerrainWindow::TerrainWindow()
		: pAction(nullptr)
	{
		terrainTextureOptions.push_back(TextureObject::Name::Mud);
		terrainTextureOptions.push_back(TextureObject::Name::Cobblestone);
		terrainTextureOptions.push_back(TextureObject::Name::RockyTrail);
		terrainTextureOptions.push_back(TextureObject::Name::CoastalSandRocks);
		terrainTextureOptions.push_back(TextureObject::Name::Desert);
		terrainTextureOptions.push_back(TextureObject::Name::Stone);
		terrainTextureOptions.push_back(TextureObject::Name::SnowRocks);
		terrainTextureOptions.push_back(TextureObject::Name::Snow);
		terrainTextureOptions.push_back(TextureObject::Name::Duckweed);
		terrainTextureOptions.push_back(TextureObject::Name::Rocks);
		terrainTextureOptions.push_back(TextureObject::Name::Brick);
		terrainTextureOptions.push_back(TextureObject::Name::Red);
		terrainTextureOptions.push_back(TextureObject::Name::Green);
		terrainTextureOptions.push_back(TextureObject::Name::Blue);
		terrainTextureOptions.push_back(TextureObject::Name::Yellow);
		terrainTextureOptions.push_back(TextureObject::Name::Sandstone);
	}

	TerrainWindow::~TerrainWindow()
	{
	}

	void TerrainWindow::Update()
	{
		ImGui::Begin("Terrain");

		static bool pressed = false;
		if (GetKeyState('I') & 0x8000)
		{
			if (!pressed)
			{
				pressed = true;
				freeze = !freeze;
			}
		}
		else
		{
			pressed = false;
		}

		Terrain* pTerrainObject = TerrainSystem::GetCurrentTerrain();

		if (pTerrainObject)
		{
			TerrainMesh* pTerrainMesh = (TerrainMesh*)pTerrainObject->GetGraphicsObject()->GetModel();

			const float tilingPrev = tilingCache;
			ImGui::SliderFloat("UV Tiling", &tilingCache, 0.1f, 100.f);

			if (Util::isNotEqual(tilingPrev, tilingCache, MATH_TOLERANCE))
			{
				pTerrainMesh->UpdateTextureCoordinates(tilingCache);
			}

			if (!freeze)
			{
				Camera* pCamera = CameraManager::GetCurrentCamera();
				ImVec2 mousePosRelativeCenterNorm = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
				Vec3 rayDir = pCamera->GetRay(mousePosRelativeCenterNorm.x, mousePosRelativeCenterNorm.y);

				Vec3 raycast;
				bool raycastHit = pTerrainObject->GetQuadTree()->Raycast(pCamera->GetLocation(), rayDir, raycast);

				if (raycastHit)
				{
					ImGui::DragFloat3("Raycast Hit", &raycast[x]);
				}
				else
				{
					ImGui::Text("Raycast Missed");
				}
				ImGui::DragFloat2("Mouse Norm", &mousePosRelativeCenterNorm.x);
			}
		}
		else
		{
			ImGui::Text("No Terrain in the scene.");
		}

		if (ImGui::Button("New Terrain"))
		{
			ImGui::OpenPopup("New Terrain Creator");
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("New Terrain Creator", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("This is the popup modal");

			if (ImGui::BeginTabBar("Terrain Creation Type Tab"))
			{
				if (ImGui::BeginTabItem("Fresh Start"))
				{
					ImGui::InputInt("Vertex Resolution", &inputVertexResolution);
					ImGui::SetItemTooltip("The number of vertices in one dimension");
					ImGui::Text("Note: Vertex Resolution must be a power of 2");
					ImGui::InputFloat("World Scale", &inputWorldScale);
					ImGui::SetItemTooltip("The world space width of the terrain");
					ImGui::InputText("New Terrain Asset Name", inputTerrainName, 32);
					ImGui::SetItemTooltip("This will be the identifier for your new terrain asset");

					if (ImGui::Button("Create Terrain!", ImVec2(120, 0)))
					{
						TerrainSystem::NewTerrainFreshStart(inputTerrainName, inputVertexResolution, inputWorldScale);
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Heightmap"))
				{
					ImGui::InputText("Heightmap File", inputHeightmapName, 64);
					ImGui::SetItemTooltip("This image will be loaded for the initial elevation data");
					ImGui::InputFloat("World Scale", &inputWorldScale);
					ImGui::SetItemTooltip("The world space width of the terrain");
					ImGui::InputFloat("Max Height", &inputHeightScale);
					ImGui::SetItemTooltip("The max height of the terrain (white pixels will be this height)");
					ImGui::InputText("New Terrain Asset Name", inputTerrainName, 32);
					ImGui::SetItemTooltip("This will be the identifier for your new terrain asset");

					if (ImGui::Button("Create Terrain!", ImVec2(120, 0)))
					{
						TerrainSystem::NewTerrainHeightmap(inputTerrainName, inputHeightmapName, inputWorldScale, inputHeightScale);
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Copy Existing"))
				{
					ImGui::InputText("New Terrain Asset Name", inputTerrainName, 32);
					ImGui::SetItemTooltip("This will be the identifier for your new terrain asset");

					if (ImGui::Button("Create Terrain!", ImVec2(120, 0)))
					{
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		UpdateTerrainSelector();

		ImGui::Checkbox("Render Foliage", pTerrainObject->GetRenderFoliagePtr());

		SOFoliage* pShader = (SOFoliage*)ShaderObjectManager::Find(ShaderObject::Name::Foliage);
		ImGui::DragFloat("Foliage Render Distance", &pShader->GetFoliageParams().renderDistance);
		ImGui::DragFloat("Foliage Scale Falloff", &pShader->GetFoliageParams().scaleFalloffStart);

		UpdateBrush();
		UpdateTextures();

		ImGui::End();

		ImGui::Begin("Heightfield Image");
		UpdateHeighfieldImage();
		ImGui::End();
	}

	void TerrainWindow::Draw()
	{
		Terrain* pTerrainObject = TerrainSystem::GetCurrentTerrain();
		if (pTerrainObject)
		{
			if (!freeze)
			{
				Camera* pCamera = CameraManager::GetCurrentCamera();
				ImVec2 mousePosRelativeCenterNorm = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
				Vec3 rayDir = pCamera->GetRay(mousePosRelativeCenterNorm.x, mousePosRelativeCenterNorm.y);

				raycastAll = pTerrainObject->GetQuadTree()->RaycastAllLeaves(pCamera->GetLocation(), rayDir);

				for (const auto& hit : raycastAll)
				{
					pTerrainObject->GetQuadTree()->DrawNodeAndParents(hit.nodeIndex);
				}
			}
		}
	}

	void TerrainWindow::UpdateBrush()
	{
		Terrain* pTerrainObject = TerrainSystem::GetCurrentTerrain();

		if (pTerrainObject)
		{
			ImGuiTreeNodeFlags flags;
			flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;
			static int selected = 0;
			const char* items[] = { "None", "Raise", "Flatten", "Smooth", "Soften", "Perlin Noise", "Paint", "Smudge Paint", "Gaussian Blur Paint", "Foliage" };
			if (ImGui::CollapsingHeader("Select Brush", flags))
			{
				for (int n = 0; n < 10; n++)
				{
					char buf[32];
					memset(buf, 0, 32);
					sprintf_s(buf, "%s", items[n]);
					if (ImGui::Selectable(buf, selected == n))
						selected = n;
				}
			}

			EditorGui::GetTerrainEditor().SetBrush(items[selected]);

			if (EditorGui::GetTerrainEditor().GetBrush())
			{
				EditorGui::GetTerrainEditor().GetBrush()->Update();

				Camera* pCamera = CameraManager::GetCurrentCamera();
				ImVec2 mousePosRelativeCenterNorm = EditorGui::GetNormalizedMousePosition(ImGui::GetMousePos());
				Vec3 rayDir = pCamera->GetRay(mousePosRelativeCenterNorm.x, mousePosRelativeCenterNorm.y);
				Vec3 raycast;
				bool raycastHit = pTerrainObject->GetQuadTree()->Raycast(pCamera->GetLocation(), rayDir, raycast);

				//const float gridCellSize = pTerrainObject->GetTerrainMesh()->GetSingleGridCellScale();
				//ImGui::Text("[ %f ]\tGrid Cell Size", gridCellSize);

				ImGui::DragFloat("Brush Size", &brushSize);
				//ImGui::DragFloat("Brush Inner Size", &brushInnerRadius);
				ImGui::DragFloat("Brush Strength", &brushStrength);

				if (raycastHit)
				{
					ImGui::DragFloat3("Brush Raycast", &raycast[x]);
				}
				else
				{
					ImGui::Text("Mouse Off Terrain");
				}

				if (EditorInput::GetLeftClickDown() && EditorGui::IsMouseInWorld() && raycastHit)
				{
					float force = (brushStrength / pTerrainObject->GetTerrainMesh()->GetHeightScale()) * 0.001f;

					if (!clickDown)
					{
						clickDown = true;
						//Trace::out("Pressed!\n");
					}

					PaintResult result;

					if (EditorInput::GetModState() == InputModifier::Shift)
					{
						result = EditorGui::GetTerrainEditor().GetBrush()->ShiftPaint(brushSize, raycast, force);
					}
					else
					{
						result = EditorGui::GetTerrainEditor().GetBrush()->Paint(brushSize, raycast, force);
					}

					TerrainArea area(raycast, brushSize);
					//int counter = area.GetWidth() * area.GetHeight();
					//ImGui::Text("Updated %d Vertices\n", counter);
				}
				else
				{
					if (clickDown)
					{
						clickDown = false;
						//Trace::out("Released!\n");
						awaitingActionState = true;
					}
				}

				// Submitting to the history is delayed until the brush has completed the update
				if (awaitingActionState)
				{
					if (!EditorGui::GetTerrainEditor().GetBrush()->IsMapInFlight())
					{
						awaitingActionState = false;
						//EditorGui::GetTerrainEditor().GetHistory().OnTerrainEdit(pTerrainObject);
					}
				}

				SOTerrain* pShader = (SOTerrain*)ShaderObjectManager::Find(ShaderObject::Name::Terrain);
				pShader->SetBrushRadius(brushSize);
				pShader->SetBrushRaycast(Vec4(raycast, 1.f));

				EditorGui::GetTerrainEditor().GetBrush()->UpdateGui();
			}
			else
			{
				SOTerrain* pShader = (SOTerrain*)ShaderObjectManager::Find(ShaderObject::Name::Terrain);
				pShader->SetBrushRadius(0.f);
			}
		}
	}

	void TerrainWindow::UpdateHeighfieldImage()
	{
		ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
		ImVec2 contentMax = ImGui::GetWindowContentRegionMax();

		float imgSize = contentMax.x - contentMin.x;

		Terrain* pTerrainObject = TerrainSystem::GetCurrentTerrain();

		if (pTerrainObject)
		{
			ImGui::Image(
				pTerrainObject->GetTerrainMesh()->GetView(),
				ImVec2((float)imgSize, (float)imgSize),
				ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f),
				ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
				ImVec4(0.f, 0.f, 0.f, 0.f)
			);
		}
	}

	std::vector<std::string> GetFiles(const std::string& pattern)
	{
		std::vector<std::string> names;
		WIN32_FIND_DATA fd;

		HANDLE hFind = FindFirstFile(pattern.c_str(), &fd);

		if (hFind != INVALID_HANDLE_VALUE) {
			do
			{
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					names.push_back(fd.cFileName);
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		}

		return names;
	}

	void TerrainWindow::UpdateTerrainSelector()
	{
		static bool selected[20]{ false };

		if (ImGui::Button("Select Terrain"))
		{
			ImGui::OpenPopup("Terrain Selector");
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Terrain Selector", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Select an existing Terrain asset.");

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			if (ImGui::BeginChild("Foliage_Brush_Loader", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 5), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY))
			{
				auto files = GetFiles("..\\Terrains\\**.terrain.proto.azul");

				for (int i = 0; i < (int)files.size(); i++)
				{
					const auto& file = files[i];

					ImGuiSelectableFlags flags{ 0 };
					flags |= ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowDoubleClick;

					if (ImGui::Selectable(file.c_str(), selected[i], flags))
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
						{
							auto terrainName = file.substr(0, file.find(".terrain.proto.azul"));
							Trace::out("clicked %s\n", terrainName.c_str());

							TerrainSystem::ChangeTerrain(terrainName.c_str());
						}
					}
				}
			}

			ImGui::PopStyleColor();
			ImGui::EndChild();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void TerrainWindow::UpdateTextures()
	{
		static int selectedTex[TerrainMesh::NUM_SURFACES] = { -1, -1, -1, -1 };

		static bool firstFrame = true;

		ImGuiTreeNodeFlags flags{ 0 };
		//flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::CollapsingHeader("Surface Layers", flags))
		{
			Terrain* pTerrainObject = TerrainSystem::GetCurrentTerrain();

			if (pTerrainObject)
			{
				if (firstFrame)
				{
					for (uint i = 0u; i < TerrainMesh::NUM_SURFACES; i++)
					{
						selectedTex[i] = (int)pTerrainObject->GetTerrainMesh()->GetSurfaceTexture(i)->GetName();
					}
					firstFrame = false;
					return;
				}

				for (uint i = 0u; i < TerrainMesh::NUM_SURFACES; i++)
				{
					TextureObject* pSurfaceTex = pTerrainObject->GetTerrainMesh()->GetSurfaceTexture(i);

					ImGui::Text("Surface Layer %d: ", i);
					ImGui::SameLine();
					std::string popupName = "Texture_Picker_" + std::to_string(i);

					bool imgButtonPressed = ImGui::ImageButton(popupName.c_str(), pSurfaceTex->GetResourceView(), { 60.f, 60.f });

					if (imgButtonPressed)
					{
						ImGui::OpenPopup(popupName.c_str());
					}

					ImGui::SameLine();
					ImGui::Selectable(selectedTex[i] == -1 ? "<None>" : TextureObject::NameToString((TextureObject::Name)selectedTex[i]));
					if (ImGui::BeginPopup(popupName.c_str()))
					{
						ImGui::SeparatorText("Texture");
						for (uint j = 0u; j < terrainTextureOptions.size(); j++)
						{
							if (ImGui::Selectable(TextureObject::NameToString(terrainTextureOptions[j])))
							{
								selectedTex[i] = (int)terrainTextureOptions[j];
							}
						}

						ImGui::EndPopup();
					}

					if (pTerrainObject->GetTerrainMesh()->GetSurfaceTexture(i)->GetName() != (TextureObject::Name)selectedTex[i])
					{
						pTerrainObject->GetTerrainMesh()->SetSurfaceTexture(i, TextureObjectManager::Find((TextureObject::Name)selectedTex[i]));
					}

					ImGui::Separator();
				}
			}
		}
	}
}