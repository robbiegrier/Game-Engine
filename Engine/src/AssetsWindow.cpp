#include "AssetsWindow.h"
#include "imgui.h"
#include "Mesh.h"
#include "TextureObject.h"
#include "CameraManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "Player.h"
#include "EditActionSpawn.h"

namespace Azul
{
	void AssetsWindow::Update()
	{
		static int spawned = 0;
		static float spawnDistance = 10.f;

		Mesh::Name meshToSpawn = Mesh::Name::None;
		TextureObject::Name textureToSpawn = TextureObject::Name::Rocks;
		Vec3 spawnScale = Vec3(1, 1, 1);

		ImGui::Begin("Assets");
		if (ImGui::Button("Player"))
		{
			Vec3 spawnPos = CameraManager::GetCurrentCamera()->GetLocation() + (-CameraManager::GetCurrentCamera()->GetDirection().getNorm() * spawnDistance);
			std::string strName = std::string("Player");

			GameObjectManager::SpawnObject(strName.c_str(),
				new Player(), spawnPos
			)->SetRelativeScale(spawnScale);
		}
		if (ImGui::Button("World Sprite")) { meshToSpawn = Mesh::Name::Sprite; textureToSpawn = TextureObject::Name::ChickenBot; }
		if (ImGui::Button("Unit Cube")) { meshToSpawn = Mesh::Name::Cube; textureToSpawn = TextureObject::Name::Mannequin; }
		if (ImGui::Button("Unit Sphere")) meshToSpawn = Mesh::Name::Sphere;
		if (ImGui::Button("Unit Pyramid")) meshToSpawn = Mesh::Name::Pyramid;
		if (ImGui::Button("Wooden Crate"))
		{
			meshToSpawn = Mesh::Name::Crate;
			textureToSpawn = TextureObject::Name::Crate;
		}
		if (ImGui::Button("Chicken Bot"))
		{
			meshToSpawn = Mesh::Name::BoneTransform;
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

		if (ImGui::Button("Tattoine Pylon")) { meshToSpawn = Mesh::Name::Tattoine0; textureToSpawn = TextureObject::Name::Tattoine0; }
		if (ImGui::Button("Tattoine Short Pipe")) { meshToSpawn = Mesh::Name::Tattoine1; textureToSpawn = TextureObject::Name::Tattoine1; }
		if (ImGui::Button("Tattoine Long Pipe")) { meshToSpawn = Mesh::Name::Tattoine2; textureToSpawn = TextureObject::Name::Tattoine2; }
		if (ImGui::Button("Tattoine Pipe 90 1")) { meshToSpawn = Mesh::Name::Tattoine3; textureToSpawn = TextureObject::Name::Tattoine3; }
		if (ImGui::Button("Tattoine Pipe 90 2")) { meshToSpawn = Mesh::Name::Tattoine4; textureToSpawn = TextureObject::Name::Tattoine4; }
		if (ImGui::Button("Tattoine 5")) { meshToSpawn = Mesh::Name::Tattoine5; textureToSpawn = TextureObject::Name::Tattoine5; }
		if (ImGui::Button("Tattoine 6")) { meshToSpawn = Mesh::Name::Tattoine6; textureToSpawn = TextureObject::Name::Tattoine6; }
		if (ImGui::Button("Tattoine 7")) { meshToSpawn = Mesh::Name::Tattoine7; textureToSpawn = TextureObject::Name::Tattoine7; }
		if (ImGui::Button("Tattoine 8")) { meshToSpawn = Mesh::Name::Tattoine8; textureToSpawn = TextureObject::Name::Tattoine8; }
		if (ImGui::Button("Tattoine Door 1")) { meshToSpawn = Mesh::Name::Tattoine9; textureToSpawn = TextureObject::Name::Tattoine9; }
		if (ImGui::Button("Tattoine Door 2")) { meshToSpawn = Mesh::Name::Tattoine10; textureToSpawn = TextureObject::Name::Tattoine10; }
		if (ImGui::Button("Tattoine 11")) { meshToSpawn = Mesh::Name::Tattoine11; textureToSpawn = TextureObject::Name::Tattoine11; }
		if (ImGui::Button("Tattoine 12")) { meshToSpawn = Mesh::Name::Tattoine12; textureToSpawn = TextureObject::Name::Tattoine12; }
		if (ImGui::Button("Tattoine 13")) { meshToSpawn = Mesh::Name::Tattoine13; textureToSpawn = TextureObject::Name::Tattoine13; }
		if (ImGui::Button("Tattoine 14")) { meshToSpawn = Mesh::Name::Tattoine14; textureToSpawn = TextureObject::Name::Tattoine14; }
		if (ImGui::Button("Tattoine 15")) { meshToSpawn = Mesh::Name::Tattoine15; textureToSpawn = TextureObject::Name::Tattoine15; }
		if (ImGui::Button("Tattoine 16")) { meshToSpawn = Mesh::Name::Tattoine16; textureToSpawn = TextureObject::Name::Tattoine16; }
		if (ImGui::Button("Tattoine 17")) { meshToSpawn = Mesh::Name::Tattoine17; textureToSpawn = TextureObject::Name::Tattoine17; }
		if (ImGui::Button("Tattoine Long Bolt")) { meshToSpawn = Mesh::Name::Tattoine18; textureToSpawn = TextureObject::Name::Tattoine18; }
		if (ImGui::Button("Tattoine Bolt")) { meshToSpawn = Mesh::Name::Tattoine19; textureToSpawn = TextureObject::Name::Tattoine19; }
		if (ImGui::Button("Tattoine Radiator")) { meshToSpawn = Mesh::Name::Tattoine20; textureToSpawn = TextureObject::Name::Tattoine20; }
		if (ImGui::Button("Tattoine 21")) { meshToSpawn = Mesh::Name::Tattoine21; textureToSpawn = TextureObject::Name::Tattoine21; }
		if (ImGui::Button("Tattoine Tarp 1")) { meshToSpawn = Mesh::Name::Tattoine22; textureToSpawn = TextureObject::Name::Tattoine22; }
		if (ImGui::Button("Tattoine Tarp 2")) { meshToSpawn = Mesh::Name::Tattoine23; textureToSpawn = TextureObject::Name::Tattoine23; }
		if (ImGui::Button("Tattoine Pole")) { meshToSpawn = Mesh::Name::Tattoine30; textureToSpawn = TextureObject::Name::Tattoine30; }
		if (ImGui::Button("Tattoine Arches")) { meshToSpawn = Mesh::Name::Tattoine31; textureToSpawn = TextureObject::Name::Tattoine31; }
		if (ImGui::Button("Tattoine Dome Building")) { meshToSpawn = Mesh::Name::Tattoine32; textureToSpawn = TextureObject::Name::Tattoine32; }
		if (ImGui::Button("Tattoine Round Tower")) { meshToSpawn = Mesh::Name::Tattoine33; textureToSpawn = TextureObject::Name::Tattoine33; }
		if (ImGui::Button("Tattoine Round Building")) { meshToSpawn = Mesh::Name::Tattoine34; textureToSpawn = TextureObject::Name::Tattoine34; }
		if (ImGui::Button("Tattoine Square Tower")) { meshToSpawn = Mesh::Name::Tattoine35; textureToSpawn = TextureObject::Name::Tattoine35; }
		if (ImGui::Button("Tattoine Large Arch")) { meshToSpawn = Mesh::Name::Tattoine36; textureToSpawn = TextureObject::Name::Tattoine36; }

		ImGui::End();

		if (meshToSpawn != Mesh::Name::None)
		{
			Vec3 spawnPos = CameraManager::GetCurrentCamera()->GetLocation() + (-CameraManager::GetCurrentCamera()->GetDirection().getNorm() * spawnDistance);
			std::string strName = std::string("Spawned Object ") + std::to_string(++spawned);

			GameObject* pSpawn = GameObjectManager::SpawnObject(strName.c_str(),
				meshToSpawn,
				textureToSpawn,
				spawnPos
			);

			pSpawn->SetRelativeScale(spawnScale);

			EditActionSpawn* pAction = new EditActionSpawn();
			pAction->SetSingleObject(pSpawn);
			pAction->SetTargetParent(nullptr);
			pAction->Commit();
		}
	}
}