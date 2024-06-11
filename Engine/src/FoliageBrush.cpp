#include "FoliageBrush.h"
#include "TerrainSystem.h"
#include "Terrain.h"
#include "TerrainMesh.h"
#include "TerrainFoliage.h"
#include "GOFoliage.h"
#include "ListNode.h"
#include "Engine.h"
#include "FoliageLibrary.h"
#include "EngineUtils.h"

namespace Azul
{
	FoliageBrush::FoliageBrush()
	{
		memset(selection, 0, sizeof(selection));
		selection[0] = true;

		foliageTypeOptions.push_back(FoliageType::Name::DryGrassA);
		foliageTypeOptions.push_back(FoliageType::Name::DryGrassB);
		foliageTypeOptions.push_back(FoliageType::Name::DryGrassC);
		foliageTypeOptions.push_back(FoliageType::Name::DryGrassD);
		foliageTypeOptions.push_back(FoliageType::Name::SmallRockA);
		foliageTypeOptions.push_back(FoliageType::Name::SmallRockB);
		foliageTypeOptions.push_back(FoliageType::Name::TreeA);
		foliageTypeOptions.push_back(FoliageType::Name::TreeB);
		foliageTypeOptions.push_back(FoliageType::Name::TreeBirchA);
	}

	FoliageBrush::~FoliageBrush()
	{
	}

	PaintResult FoliageBrush::Paint(float radius, const Vec3& center, float force)
	{
		deltaTimeTracker += Engine::GetDeltaTime() * force * 100000.f;

		if (deltaTimeTracker >= paintInterval)
		{
			deltaTimeTracker = 0.f;

			Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
			TerrainFoliage* pFoliage = pTerrain->GetTerrainMesh()->GetFoliage();

			int n = 0;
			for (Iterator& it = *pFoliage->GetGraphicsObjects().GetIterator(); !it.IsDone(); it.Next(), n++)
			{
				if (selection[n])
				{
					GOFoliage* pGo = (GOFoliage*)((ListNode*)it.Curr())->Get();

					FoliageInstance instance;

					float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float rRadius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					float rsign = rand() % 2 == 0 ? 1.f : -1.f;
					float rsign2 = rand() % 2 == 0 ? 1.f : -1.f;

					Vec3 spawnPos(r * rsign, 0.f, r2 * rsign2);
					spawnPos = spawnPos.getNorm() * radius * rRadius;
					spawnPos += center;

					r3 = std::min(r3, 1.f);
					r3 = EngineUtils::Lerp(1.f, r3, spawnScaleVariance);
					float objScale = r3 * pGo->GetSpawnScale() * spawnScaleModifier;

					instance.scale = objScale;
					instance.x = spawnPos.x();
					instance.z = spawnPos.z();

					instance.angle = 0.f;

					if (randomRotation)
					{
						float rAngle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
						instance.angle = EngineUtils::Lerp(0.f, MATH_2PI, rAngle);
					}

					if (useTintConstant)
					{
						float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
						r4 = EngineUtils::Lerp(1.f, r4, tintVariance);
						instance.color = Vec4(tintColorConstant.x, tintColorConstant.y, tintColorConstant.z, 1.f) * r4 * tintFactor;
					}
					else
					{
						float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
						float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
						float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
						r4 = EngineUtils::Lerp(1.f, r4, tintVariance);
						r5 = EngineUtils::Lerp(1.f, r5, tintVariance);
						r6 = EngineUtils::Lerp(1.f, r6, tintVariance);
						instance.color = Vec4(r4, r5, r6, 1) * tintFactor;
					}

					pGo->AddInstance(instance);
				}
			}
		}

		return PaintResult::Success;
	}

	PaintResult FoliageBrush::ShiftPaint(float radius, const Vec3& center, float force)
	{
		deltaTimeTracker += Engine::GetDeltaTime() * force * 100000.f;

		if (deltaTimeTracker >= paintInterval)
		{
			deltaTimeTracker = 0.f;

			Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
			TerrainFoliage* pFoliage = pTerrain->GetTerrainMesh()->GetFoliage();

			int n = 0;
			for (Iterator& it = *pFoliage->GetGraphicsObjects().GetIterator(); !it.IsDone(); it.Next(), n++)
			{
				if (selection[n])
				{
					GOFoliage* pGo = (GOFoliage*)((ListNode*)it.Curr())->Get();

					float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float rRadius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					float rsign = rand() % 2 == 0 ? 1.f : -1.f;
					float rsign2 = rand() % 2 == 0 ? 1.f : -1.f;

					Vec3 spawnPos(r * rsign, 0.f, r2 * rsign2);
					spawnPos = spawnPos.getNorm() * radius * rRadius;
					spawnPos += center;

					pGo->RemoveInstanceNear(spawnPos, center, radius);
				}
			}
		}

		return PaintResult::Success;
	}

	void FoliageBrush::Update()
	{
	}

	void FoliageBrush::UpdateGui()
	{
		ImGuiTreeNodeFlags flags;
		flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen;

		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Foliage Brush", flags))
		{
			ImGui::TextWrapped("Add foliage to the terrain.");

			Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
			TerrainFoliage* pFoliage = pTerrain->GetTerrainMesh()->GetFoliage();

			ImGui::Checkbox("Random Y Rotation", &randomRotation);
			ImGui::SliderFloat("Scale", &spawnScaleModifier, .01f, 10.f);
			ImGui::SliderFloat("Scale Variance", &spawnScaleVariance, .0f, 1.f);
			ImGui::SliderFloat("Tint Factor", &tintFactor, .0f, 1.f);
			ImGui::SliderFloat("Tint Variance", &tintVariance, .0f, 1.f);
			ImGui::Checkbox("Use Tint Color", &useTintConstant);
			ImGui::SameLine();
			ImGui::ColorEdit3("Tint Color", &tintColorConstant.x);

			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
			if (ImGui::BeginChild("Foliage_Brush_Loader", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY))
			{
				int n = 0;
				for (Iterator& it = *pFoliage->GetGraphicsObjects().GetIterator(); !it.IsDone(); it.Next(), n++)
				{
					GOFoliage* pGo = (GOFoliage*)((ListNode*)it.Curr())->Get();
					if (ImGui::Selectable(pGo->GetFoliageType()->NameToString(), selection[n]))
					{
						if (!ImGui::GetIO().KeyCtrl && !ImGui::GetIO().KeyShift)
							memset(selection, 0, sizeof(selection));
						selection[n] = true;
					}
				}
			}

			ImGui::PopStyleColor();
			ImGui::EndChild();

			ImGui::Indent();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("FoliageType_Details", ImVec2(), ImGuiChildFlags_None, window_flags);

			for (Iterator& it = *pFoliage->GetGraphicsObjects().GetIterator(); !it.IsDone(); )
			{
				GOFoliage* pGo = (GOFoliage*)((ListNode*)it.Curr())->Get();
				ImGui::Spacing();
				if (ImGui::CollapsingHeader(pGo->GetFoliageType()->NameToString(), flags))
				{
					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, { .33f, 0.1f, 0.1f, 1.f });
					if (ImGui::SmallButton((std::string("Remove ") + pGo->GetFoliageType()->NameToString()).c_str()))
					{
						DLink* pCurr = it.Curr();
						it.Next();
						DLink* pDelete = pFoliage->GetGraphicsObjects().Remove(pCurr);
						delete pDelete;
						ImGui::PopStyleColor();
						continue;
					}
					ImGui::PopStyleColor();

					ImGui::BulletText("  Total Instances: %d", pGo->GetTotalInstances());
					ImGui::BulletText("Visible Instances: %d", pGo->GetVisibleInstances());
				}

				it.Next();
			}

			ImGui::Spacing();

			std::string popupName = "Foliage_Picker";
			if (ImGui::Button("Add New Foliage Type"))
			{
				ImGui::OpenPopup(popupName.c_str());
			}

			if (ImGui::BeginPopup(popupName.c_str()))
			{
				ImGui::SeparatorText("Foliage Type");

				for (int j = 0; j < (int)foliageTypeOptions.size(); j++)
				{
					if (ImGui::Selectable(FoliageType::NameToString(foliageTypeOptions[j])))
					{
						selectedFoliage = (int)foliageTypeOptions[j];
					}
				}

				ImGui::EndPopup();
			}

			if (selectedFoliage != -1)
			{
				pFoliage->AddFoliageType(foliageTypeOptions[selectedFoliage]);
				selectedFoliage = -1;
			}

			ImGui::EndChild();
			ImGui::Unindent();
		}
	}
}