#include <d3d11.h>
#include <d3dcompiler.h>
#include "Game.h"
#include "Camera.h"
#include "Colors.h"
#include "ShaderObject.h"
#include "SOColorByVertex.h"
#include "Mesh.h"
#include "CubeMesh.h"
#include "DiamondMesh.h"
#include "CrossMesh.h"
#include "PyramidMesh.h"
#include "Plane.h"
#include "GOColorByVertex.h"
#include "GOWireframe.h"
#include "GameObjectManager.h"
#include "ShaderObjectManager.h"
#include "MeshManager.h"
#include "CameraManager.h"
#include "DirectXTex.h"
#include "TextureObject.h"
#include "SOFlatTexture.h"
#include "TextureObjectManager.h"
#include "GOLightTexture.h"
#include "SOLightTexture.h"
#include "SOSprite.h"
#include "GOConstColor.h"
#include "SOConstColor.h"
#include "GOFlatTexture.h"
#include "SOFlatTexture.h"
#include "Player.h"
#include "GONull.h"
#include "SONull.h"
#include "SODefault.h"
#include "MeshProto.h"
#include "TextureProto.h"
#include "MeshSphere.h"
#include "Clip.h"
#include "SOEditorVisual.h"
#include "SceneManager.h"
#include "GameObjectSprite.h"
#include "GOSprite.h"
#include "ImageManager.h"
#include "Font.h"
#include "GameObjectText.h"
#include "FontLibrary.h"
#include "Skeleton.h"
#include "AnimationSystem.h"
#include "SkeletonManager.h"
#include "SOSkinLightTexture.h"
#include "ClipManager.h"
#include "ClipProto.h"
#include "Animator.h"
#include "GOSkinLightTexture.h"
#include "TerrainMesh.h"
#include "Terrain.h"
#include "TerrainSystem.h"
#include "SOTerrain.h"
#include "SOFoliage.h"
#include "GOFoliage.h"
#include "TerrainFoliage.h"
#include "FoliageLibrary.h"
#include "SOSurfaceShape.h"
#include "LightSystem.h"

namespace Azul
{
	Game* Game::pInstance = nullptr;

	ShaderObject* poShaderColorByVertex;
	Mesh* pModelPyramid;
	Mesh* pModelCube;

	Animator* pChickenAnimator;
	Animator* pMannequinAnimator;

	AnimTime animDeltaTime;

	GameObject* LightSource;
	GameObject* LightSource1;
	GameObject* LightSource2;
	GameObject* pPlayer;
	GameObject* pAntiqueCamera;

	GameObject* pChickenBot;
	GameObject* pMannequin;

	Vec3 lightColor = Vec3(Azul::Colors::LightYellow);
	Vec3 lightPos = Vec3(50, 2, -40);
	static float fogDistMin = 400.f;
	static float fogDistMax = 700.f;

	GOFoliage* pGoFoliage;
	GOFoliage* pGoFoliage2;

	Vec3 dirLightDirection;

	int WINAPI Game::Launch(HINSTANCE pInstanceHandle, int cmdShow)
	{
		return GetInstance().Main(pInstanceHandle, cmdShow);
	}

	void Game::Teardown()
	{
		delete pInstance;
		pInstance = nullptr;
		pEngineInstance = nullptr;
	}

	Game::Game()
	{
		globalTimer.Tic();
		intervalTimer.Tic();
	}

	Game::~Game()
	{
	}

	bool Game::LoadContent()
	{
		ShaderObjectManager::Create();
		TextureObjectManager::Create();
		MeshManager::Create();
		CameraManager::Create();
		GameObjectManager::Create();
		ClipManager::Create();
		SkeletonManager::Create();
		ImageManager::Create();
		FontLibrary::Create();
		AnimationSystem::Create();
		TerrainSystem::Create();
		FoliageLibrary::Create();
		LightSystem::Create();

		LoadShaders();
		LoadCameras();
		LoadAssets();

		ImageManager::Add(Image::Name::GreenBird, new Image(Image::Name::GreenBird, TextureObject::Name::Birds, Rect(244.0f, 134.0f, 102.0f, 75.0f)));
		ImageManager::Add(Image::Name::Text, new Image(Image::Name::Text, TextureObject::Name::CenturyFont, Rect(0.0f, 0.0f, 100.0f, 100.0f)));
		FontLibrary::Add(Font::Name::Century, new Font("Century.font.proto.azul", TextureObjectManager::Find(TextureObject::Name::CenturyFont)));

		SkeletonManager::Add(Skeleton::Name::ChickenBot, new Skeleton("walk_mesh.skeleton.proto.azul"));
		SkeletonManager::Add(Skeleton::Name::MixamoRig1, new Skeleton("mannequin.skeleton.proto.azul"));
		SkeletonManager::Add(Skeleton::Name::Paladin, new Skeleton("Paladin.skeleton.proto.azul"));
		SkeletonManager::Add(Skeleton::Name::Knight, new Skeleton("Knight.skeleton.proto.azul"));

		ClipManager::Add(Clip::Name::ShotUp, new ClipProto("shot_up.anim.proto.azul"));
		ClipManager::Add(Clip::Name::Run, new ClipProto("run.anim.proto.azul"));
		ClipManager::Add(Clip::Name::Walk, new ClipProto("walk_mesh.anim.proto.azul"));
		ClipManager::Add(Clip::Name::SidestepRight, new ClipProto("sidestep_right.anim.proto.azul"));
		ClipManager::Add(Clip::Name::HitFront, new ClipProto("hit_front.anim.proto.azul"));
		ClipManager::Add(Clip::Name::HumanoidRun, new ClipProto("mannequin.anim.proto.azul"));
		ClipManager::Add(Clip::Name::RunJump, new ClipProto("RunJump.anim.proto.azul"));
		ClipManager::Add(Clip::Name::PaladinWalk, new ClipProto("Paladin.anim.proto.azul"));
		ClipManager::Add(Clip::Name::PaladinRun, new ClipProto("PaladinWeirdRunning.anim.proto.azul"));
		ClipManager::Add(Clip::Name::PaladinIdle, new ClipProto("PaladinStandIdle.anim.proto.azul"));
		ClipManager::Add(Clip::Name::KnightWalk, new ClipProto("Knight.anim.proto.azul"));

		FoliageLibrary::Add(FoliageType::Name::DryGrassA, new FoliageType(Mesh::Name::DryGrass0, TextureObject::Name::DryGrass0, .5f));
		FoliageLibrary::Add(FoliageType::Name::DryGrassB, new FoliageType(Mesh::Name::DryGrass1, TextureObject::Name::DryGrass1, .5f));
		FoliageLibrary::Add(FoliageType::Name::DryGrassC, new FoliageType(Mesh::Name::DryGrass2, TextureObject::Name::DryGrass2, .5f));
		FoliageLibrary::Add(FoliageType::Name::DryGrassD, new FoliageType(Mesh::Name::DryGrass3, TextureObject::Name::DryGrass3, .5f));
		FoliageLibrary::Add(FoliageType::Name::SmallRockA, new FoliageType(Mesh::Name::SimpleRock, TextureObject::Name::SimpleRock, 6.f));
		FoliageLibrary::Add(FoliageType::Name::SmallRockB, new FoliageType(Mesh::Name::SimpleRock2, TextureObject::Name::SimpleRock2, 6.f));

		FoliageType* pTreeType = FoliageLibrary::Add(FoliageType::Name::TreeA, new FoliageType(Mesh::Name::TropicalTree0, TextureObject::Name::TropicalTree0, 0.5f));
		pTreeType->AddModel(Mesh::Name::TropicalTree1, TextureObject::Name::TropicalTree1);
		pTreeType->SetFollowNormal(false);

		FoliageType* pTreeType2 = FoliageLibrary::Add(FoliageType::Name::TreeB, new FoliageType(Mesh::Name::TropicalTree2, TextureObject::Name::TropicalTree2, 0.5f));
		pTreeType2->AddModel(Mesh::Name::TropicalTree3, TextureObject::Name::TropicalTree3);
		pTreeType2->SetFollowNormal(false);

		FoliageType* pTreeType3 = FoliageLibrary::Add(FoliageType::Name::TreeBirchA, new FoliageType(Mesh::Name::BirchTree0, TextureObject::Name::BirchTree0, 5.0f));
		pTreeType3->AddModel(Mesh::Name::BirchTree1, TextureObject::Name::BirchTree1);
		pTreeType3->SetFollowNormal(false);

		// Scene Directional Light
		SODefault* pShader = (SODefault*)ShaderObjectManager::Find(ShaderObject::Name::Default);
		pShader->SetFogParameters(fogDistMin, fogDistMax, Colors::LightSkyBlue * 0.4f);

		SOSkinLightTexture* pShaderSkin = (SOSkinLightTexture*)ShaderObjectManager::Find(ShaderObject::Name::SkinLightTexture);
		pShaderSkin->SetFogParameters(fogDistMin, fogDistMax, Colors::LightSkyBlue * 0.4f);

		SOTerrain* pShaderTerrain = (SOTerrain*)ShaderObjectManager::Find(ShaderObject::Name::Terrain);
		pShaderTerrain->SetFogParameters(fogDistMin, fogDistMax, Colors::LightSkyBlue * 0.4f);

		SOFoliage* pShaderFoliage = (SOFoliage*)ShaderObjectManager::Find(ShaderObject::Name::Foliage);
		pShaderFoliage->SetFogParameters(fogDistMin, fogDistMax, Colors::LightSkyBlue * 0.4f);

		Vec3 dirLightAmbient = .2f * Vec3(1, 1, .8f);
		Vec3 dirLightDiffuse = .9f * Vec3(1, 1, .8f);
		Vec3 dirLightSpecular = Vec3(0.5f, 0.5f, 0.5f);
		dirLightDirection = Vec3(-1, -1, 1).getNorm();

		//DirectionalLight dirLightData;
		//dirLightData.direction = Vec4(dirLightDirection, 1.f);
		//dirLightData.light.ambient = Vec4(dirLightAmbient, 1.f);
		//dirLightData.light.diffuse = Vec4(dirLightDiffuse, 1.f);
		//dirLightData.light.specular = Vec4(dirLightSpecular, 1.f);

		//LightSystem::SetDirectionalLight(dirLightData);

		GameObject::SetRenderShellGlobal(false);
		Engine::ToggleMaxFramerate(false);

		SceneManager::ChangeScene("AzulScene");

		static bool genTerrain = false;

		if (genTerrain)
		{
			//TerrainMesh* pTerrainMesh = new TerrainMesh(1000.f, 2048);
			TerrainMesh* pTerrainMesh = new TerrainMesh("Meadows", 1300.f, 1000.f, "..\\Models\\Heightmap_03_Meadows.png");
			//TerrainMesh* pTerrainMesh = new TerrainMesh(2200.f, 500.f, "..\\Models\\HeightMapExample.png");
			//TerrainMesh* pTerrainMesh = new TerrainMesh(1500.f, 500.f, "..\\Models\\Moro_River.heightmap.jpg");
			//TerrainMesh* pTerrainMesh = new TerrainMesh(1500.f, "..\\Models\\windingmap.tga");
			//TerrainMesh* pTerrainMesh = new TerrainMesh("Dunes", 1000.f, 500.f, "..\\Models\\Heightmap_05_Dunes.png");
			MeshManager::Add(Mesh::Name::Terrain, pTerrainMesh);

			//GraphicsObject* pGoTerrain = new GOConstColor(pTerrainMesh, ShaderObjectManager::Find(ShaderObject::Name::ConstColor), Vec3(0.f, 1.f, 0.f));
			//GraphicsObject* pGoTerrain = new GOFlatTexture(pTerrainMesh, ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Desert));
			GraphicsObject* pGoTerrain = new GOLightTexture(pTerrainMesh, ShaderObjectManager::Find(ShaderObject::Name::Default), TextureObjectManager::Find(TextureObject::Name::Desert));
			Terrain* pTerrainGameObject = new Terrain(pGoTerrain);

			GameObjectManager::SpawnObject("Terrain", pTerrainGameObject, Vec3());

			TerrainSystem::SetCurrentTerrain(pTerrainGameObject);
		}

		//Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
		//pTerrain->GetTerrainMesh()->GetFoliage()->AddFoliageType(FoliageType::Name::DryGrassA);
		//pTerrain->GetTerrainMesh()->GetFoliage()->AddFoliageType(FoliageType::Name::SmallRockA);

		//int totalInst = 1000000;
		//int totalInst = 100000;

		//int dimensionSize = totalInst / 4;
		//float objScaleMax = 0.5f;
		//float scale = 1200.f;
		//Vec3 offset(-scale / 2, 0, -scale / 2);
		//for (int i = 0; i < dimensionSize; i++)
		//{
		//	FoliageInstance instance;

		//	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r3 = .25f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		//	Vec3 spawnPos(scale * r, 0.f, scale * r2);
		//	spawnPos += offset;

		//	float objScale = r3 * objScaleMax;

		//	instance.scale = objScale;
		//	instance.x = spawnPos.x();
		//	instance.z = spawnPos.z();
		//	instance.angle = 0.f;

		//	float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float tintFactor = 0.1f;
		//	instance.color = Vec4(r4, r5, r6, 1) * tintFactor;

		//	pGoFoliage->AddInstance(instance);
		//}

		//Trace::out("Spawned %d rocks 1\n", dimensionSize);

		//int dimensionSize2 = totalInst / 4;
		//for (int i = 0; i < dimensionSize2; i++)
		//{
		//	FoliageInstance instance;
		//	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r3 = .25f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		//	Vec3 spawnPos(scale * r, 0.f, scale * r2);
		//	spawnPos += offset;

		//	float objScale = r3 * objScaleMax;
		//	instance.scale = objScale;
		//	instance.x = spawnPos.x();
		//	instance.z = spawnPos.z();
		//	instance.angle = 0.f;
		//	float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float tintFactor = 0.1f;
		//	instance.color = Vec4(r4, r5, r6, 1) * tintFactor;

		//	pGoFoliage2->AddInstance(instance);
		//}

		//for (int i = 0; i < dimensionSize2; i++)
		//{
		//	FoliageInstance instance;
		//	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r3 = .25f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		//	Vec3 spawnPos(scale * r, 0.f, scale * r2);
		//	spawnPos += offset;

		//	float objScale = r3 * objScaleMax;
		//	instance.scale = objScale;
		//	instance.x = spawnPos.x();
		//	instance.z = spawnPos.z();
		//	instance.angle = 0.f;
		//	float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float tintFactor = 0.1f;
		//	instance.color = Vec4(r4, r5, r6, 1) * tintFactor;

		//	pGoFoliage3->AddInstance(instance);
		//}

		//for (int i = 0; i < dimensionSize2; i++)
		//{
		//	FoliageInstance instance;
		//	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r3 = .25f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		//	Vec3 spawnPos(scale * r, 0.f, scale * r2);
		//	spawnPos += offset;

		//	float objScale = r3 * objScaleMax;
		//	instance.scale = objScale;
		//	instance.x = spawnPos.x();
		//	instance.z = spawnPos.z();
		//	instance.angle = 0.f;
		//	float r4 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r5 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float r6 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//	float tintFactor = 0.1f;
		//	instance.color = Vec4(r4, r5, r6, 1) * tintFactor;

		//	pGoFoliage4->AddInstance(instance);
		//}

		//Trace::out("Spawned %d rocks 2\n", dimensionSize2);

		//GameObjectManager::Dump();

		lightPos = GameObjectManager::FindObject("Light Yellow")->GetRelativeLocation();

		return true;
	}

	void Game::Update(float deltaTime)
	{
		UpdateDemo(deltaTime);
		//AnimationSystem::Update();
		LightSystem::Update();
		GameObjectManager::Update(deltaTime);
		EndFrame();
	}

	void Game::Render()
	{
		GameObjectManager::Draw();
	}

	void Game::UnloadContent()
	{
		GameObjectManager::Destroy();
		ShaderObjectManager::Destroy();
		TextureObjectManager::Destroy();
		MeshManager::Destroy();
		CameraManager::Destroy();
		SkeletonManager::Destroy();
		ClipManager::Destroy();
		ImageManager::Destroy();
		FontLibrary::Destroy();
		AnimationSystem::Destroy();
		TerrainSystem::Destroy();
		FoliageLibrary::Destroy();
		LightSystem::Destroy();

		//delete pGoFoliage;
		//delete pGoFoliage2;
	}

	void Game::EndFrame()
	{
		intervalTimer.Tic();
	}

	void Game::UpdateDemo(float deltaTime)
	{
		//if (GetKeyState('5') & 0x8000)
		//{
		//	pChickenAnimator->SetActiveClip(Clip::Name::Walk);
		//	pMannequinAnimator->SetActiveClip(Clip::Name::HumanoidRun);
		//}
		//else if (GetKeyState('6') & 0x8000)
		//{
		//	pChickenAnimator->SetActiveClip(Clip::Name::ShotUp);
		//	pMannequinAnimator->SetActiveClip(Clip::Name::RunJump);
		//}

		static float rot1 = 0.f;
		static float scale1 = 0.2f;
		static float scaleSign = 1.f;
		static float trans1 = 1.05f;
		static float transSign = 1.f;
		static float texTrans1 = 1.05f;
		static float texTransSign = 1.f;
		scale1 += 0.003f * scaleSign;

		if (scale1 > 0.5f)
		{
			scaleSign = -1.f;
		}
		else if (scale1 < .1f)
		{
			scaleSign = 1.f;
		}

		rot1 += 1.f * deltaTime;

		//if (pTexture1->GetWorldLocation()[x] < -15.f)
		//{
		//	texTransSign = 1.f;
		//}
		//else if (pTexture1->GetWorldLocation()[x] > 10.f)
		//{
		//	texTransSign = -1.f;
		//}

		//if (GetKeyState('F') & 0x8000)
		//{
		//	lightPos[y] += 10 * deltaTime;
		//}
		//if (GetKeyState('V') & 0x8000)
		//{
		//	lightPos[y] -= 10 * deltaTime;
		//}

		//if (GetKeyState(VK_LEFT) & 0x8000)
		//{
		//	lightPos[x] += 10 * deltaTime;
		//}
		//if (GetKeyState(VK_RIGHT) & 0x8000)
		//{
		//	lightPos[x] -= 10 * deltaTime;
		//}

		//if (GetKeyState(VK_UP) & 0x8000)
		//{
		//	lightPos[z] += 10 * deltaTime;
		//}
		//if (GetKeyState(VK_DOWN) & 0x8000)
		//{
		//	lightPos[z] -= 10 * deltaTime;
		//}
		lightPos = GameObjectManager::FindObject("Light Yellow")->GetWorldLocation();
		//GameObjectManager::FindObject("Light Yellow")->SetWorldLocation(lightPos);
		//GameObjectManager::FindObject("Light Red")->SetWorldLocation(lightPos + Vec3(5, 0, 0));
		//GameObjectManager::FindObject("Light Blue")->SetWorldLocation(lightPos + Vec3(-5, 0, 0));

		//float diffScale = 0.5f;
		//float radius = 300.f;
		//float attenScale = .02f;

		//Vec3 attenVec = attenScale * Vec3(1, 1, 1);
		//Vec3 a = 0.f * Vec3(0.3f, 0.3f, 0.3f);
		//Vec3 d = diffScale * Vec3(.8f, .1f, .1f);
		//Vec3 s = Vec3(0.8f, .8f, .6f);

		//Vec3 yellow = Vec3(0.8f, .8f, .6f) * diffScale;
		//Vec3 red = Vec3(.8f, .1f, .1f) * diffScale;
		//Vec3 blue = Vec3(.1f, .1f, .8f) * diffScale;

		//PointLight pointLightData0;
		//pointLightData0.attenuation = Vec4(attenVec, 1.f);
		//pointLightData0.position = Vec4(lightPos, 1.f);
		//pointLightData0.range = radius;
		//pointLightData0.light.ambient = Vec4(a, 1.f);
		//pointLightData0.light.diffuse = Vec4(yellow, 1.f);
		//pointLightData0.light.specular = Vec4(s, 1.f);
		//LightSystem::SetPointLight(0, pointLightData0);
	}

	void Game::LoadShaders()
	{
		ShaderObjectManager::Add(ShaderObject::Name::Null, new SONull());
		ShaderObjectManager::Add(ShaderObject::Name::ColorByVertex, new SOColorByVertex());
		ShaderObjectManager::Add(ShaderObject::Name::FlatTexture, new SOFlatTexture());
		ShaderObjectManager::Add(ShaderObject::Name::LightTexture, new SOLightTexture());
		ShaderObjectManager::Add(ShaderObject::Name::ConstColor, new SOConstColor());
		ShaderObjectManager::Add(ShaderObject::Name::Default, new SODefault());
		ShaderObjectManager::Add(ShaderObject::Name::EditorVisual, new SOEditorVisual());
		ShaderObjectManager::Add(ShaderObject::Name::Sprite, new SOSprite());
		ShaderObjectManager::Add(ShaderObject::Name::SkinLightTexture, new SOSkinLightTexture());
		ShaderObjectManager::Add(ShaderObject::Name::Terrain, new SOTerrain());
		ShaderObjectManager::Add(ShaderObject::Name::Foliage, new SOFoliage());
		ShaderObjectManager::Add(ShaderObject::Name::SurfaceShape, new SOSurfaceShape());
	}

	void Game::LoadCameras()
	{
		Camera* pCamera = CameraManager::Add(Camera::Name::Default, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), Vec3(0, 0, 10), Vec3(0, 100, 100));
		pCamera->SetPerspective(75.0f, GetAspectRatio(), 0.1f, 10000.0f);

		pCamera = CameraManager::Add(Camera::Name::Player, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), Vec3(0, 0, 10), Vec3(0, 0, 0));
		pCamera->SetPerspective(75.0f, GetAspectRatio(), 0.1f, 1000.0f);

		pCamera = CameraManager::Add(Camera::Name::Sprite, new Camera());
		pCamera->SetViewport(0, 0, Engine::GetWindowWidth(), Engine::GetWindowHeight());
		pCamera->SetOrientAndPosition(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 2.0f));
		pCamera->SetOrthographic(
			(float)-pCamera->GetScreenWidth() / 2.0f, (float)pCamera->GetScreenWidth() / 2.0f,
			(float)-pCamera->GetScreenHeight() / 2.0f, (float)pCamera->GetScreenHeight() / 2.0f,
			1.0f, 1000.0f
		);

		pCamera = CameraManager::Add(Camera::Name::High, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), Vec3(), Vec3(2.f, 6.f, 7.f));
		pCamera->SetPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

		pCamera = CameraManager::Add(Camera::Name::Low, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), Vec3(), Vec3(-2.f, -3.f, 7.f));
		pCamera->SetPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

		pCamera = CameraManager::Add(Camera::Name::Aux, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), Vec3(), Vec3(2.f, 6.f, -7.f));
		pCamera->SetPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

		CameraManager::SetCurrentCamera(Camera::Name::Default);
		CameraManager::SetCurrentCamera2D(Camera::Name::Sprite);
	}

	void Game::LoadAssets()
	{
		azulModel_proto aB_proto;
		aB_proto.ParseFromString(EngineUtils::FileToString("desert_rocks.proto.azul"));
		azulModel desertRocksData;
		desertRocksData.Deserialize(aB_proto);

		azulModel_proto aB_protoBuildings;
		aB_protoBuildings.ParseFromString(EngineUtils::FileToString("tattoine_buildings_kitbash.proto.azul"));
		azulModel tattoineData;
		tattoineData.Deserialize(aB_protoBuildings);

		TextureObject* pTexCurr;

		pTexCurr = TextureObjectManager::Add(TextureObject::Name::Cobblestone, new TextureProto("square_cobblestone.proto.azul"));
		pTexCurr->pDisplacement = TextureObjectManager::Add(TextureObject::Name::Cobblestone_Disp, new TextureProto("square_cobblestone_disp.proto.azul"));

		pTexCurr = TextureObjectManager::Add(TextureObject::Name::Mud, new TextureProto("brown_mud_rocks.proto.azul"));
		pTexCurr->pDisplacement = TextureObjectManager::Add(TextureObject::Name::Mud_Disp, new TextureProto("brown_mud_rocks_disp.proto.azul"));
		pTexCurr->pNormalMap = TextureObjectManager::Add(TextureObject::Name::Mud_Norm, new TextureProto("brown_mud_rocks_nor.proto.azul"));

		pTexCurr = TextureObjectManager::Add(TextureObject::Name::CoastalSandRocks, new TextureProto("coast_sand_rocks.proto.azul"));
		pTexCurr->pDisplacement = TextureObjectManager::Add(TextureObject::Name::CoastalSandRocks_Disp, new TextureProto("coast_sand_rocks_disp.proto.azul"));
		pTexCurr->pNormalMap = TextureObjectManager::Add(TextureObject::Name::CoastalSandRocks_Norm, new TextureProto("coast_sand_rocks_nor.proto.azul"));

		pTexCurr = TextureObjectManager::Add(TextureObject::Name::RockyTrail, new TextureProto("rocky_trail.proto.azul"));
		pTexCurr->pDisplacement = TextureObjectManager::Add(TextureObject::Name::RockyTrail_Disp, new TextureProto("rocky_trail_disp.proto.azul"));
		pTexCurr->pNormalMap = TextureObjectManager::Add(TextureObject::Name::RockyTrail_Norm, new TextureProto("rocky_trail_nor.proto.azul"));

		pTexCurr = TextureObjectManager::Add(TextureObject::Name::Sandstone, new TextureProto("sandstone_cracks.proto.azul"));
		pTexCurr->pNormalMap = TextureObjectManager::Add(TextureObject::Name::Sandstone_Norm, new TextureProto("sandstone_cracks_nor.proto.azul"));

		TextureObjectManager::Add(TextureObject::Name::SnowRocks, new TextureProto("snow_rocks.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Snow, new TextureProto("snow.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Brick, new TextureProto("RedBrick.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Rocks, new TextureProto("Rocks.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Duckweed, new TextureProto("Duckweed.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Stone, new TextureProto("Stone.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Desert, new TextureProto("desert.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Crate, new TextureProto("wooden crate.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Frigate, new TextureProto("space_frigate.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Fish, new TextureProto("BarramundiFish.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::DogHouse, new TextureProto("doghouse0908.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::R2D2, new TextureProto("R2-D2.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Duck, new TextureProto("Duck.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Corset, new TextureProto("Corset.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::AntiqueCameraTripod, new TextureProto("AntiqueCamera.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::AntiqueCamera, new TextureProto("AntiqueCamera.proto.azul", 1));
		TextureObjectManager::Add(TextureObject::Name::Dog, new TextureProto("dog.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::WesternTownHouse, new TextureProto("western_town_house.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::ChickenBot, new TextureProto("walk_mesh.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Red, new TextureProto("Red.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Blue, new TextureProto("Blue.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Yellow, new TextureProto("Yellow.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Green, new TextureProto("Green.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Black, new TextureProto("Black.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Hovered, new TextureProto("Hovered.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Disabled, new TextureProto("Disabled.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Birds, new TextureProto("Birds.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::CenturyFont, new TextureProto("CenturyFontTexture.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::ChickenBot, new TextureProto("walk_mesh.skin.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Mannequin, new TextureProto("mannequin.skin.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Paladin, new TextureProto("Paladin.skin.proto.azul"));
		TextureObjectManager::Add(TextureObject::Name::Knight, new TextureProto("Knight.skin.proto.azul"));

		int textI = 0;
		TextureObjectManager::Add(TextureObject::Name::DesertRock0, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock1, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock2, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock3, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock4, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock5, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock6, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock7, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock8, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock9, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock10, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock11, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock12, new TextureProto(desertRocksData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::DesertRock13, new TextureProto(desertRocksData.meshes[textI++]));

		textI = 0;
		TextureObjectManager::Add(TextureObject::Name::Tattoine0, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine1, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine2, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine3, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine4, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine5, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine6, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine7, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine8, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine9, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine10, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine11, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine12, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine13, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine14, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine15, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine16, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine17, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine18, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine19, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine20, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine21, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine22, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine23, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine24, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine25, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine26, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine27, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine28, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine29, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine30, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine31, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine32, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine33, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine34, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine35, new TextureProto(tattoineData.meshes[textI++]));
		TextureObjectManager::Add(TextureObject::Name::Tattoine36, new TextureProto(tattoineData.meshes[textI++]));

		//TextureObjectManager::Add(TextureObject::Name::Bone, new TextureProto("Bone.proto.azul", 0));
		//TextureObjectManager::Add(TextureObject::Name::Bone_001, new TextureProto("Bone_001.proto.azul", 0));
		//TextureObjectManager::Add(TextureObject::Name::Bone_L, new TextureProto("Bone_L.proto.azul", 0));
		//TextureObjectManager::Add(TextureObject::Name::Bone_L_001, new TextureProto("Bone_L_001.proto.azul", 0));
		//TextureObjectManager::Add(TextureObject::Name::Bone_L_002, new TextureProto("Bone_L_002.proto.azul", 0));
		//TextureObjectManager::Add(TextureObject::Name::Bone_R, new TextureProto("Bone_R.proto.azul", 0));
		//TextureObjectManager::Add(TextureObject::Name::Bone_R_001, new TextureProto("Bone_R_001.proto.azul", 0));
		//TextureObjectManager::Add(TextureObject::Name::Bone_R_002, new TextureProto("Bone_R_002.proto.azul", 0));

		MeshManager::Add(Mesh::Name::Sprite, new MeshProto("UnitSquare.proto.azul"));
		MeshManager::Add(Mesh::Name::Sphere, new MeshProto("UnitSphere.proto.azul"));
		MeshManager::Add(Mesh::Name::UnitCube, new MeshProto("UnitCube.proto.azul"));
		MeshManager::Add(Mesh::Name::Cylinder, new MeshProto("cylinder.proto.azul"));
		MeshManager::Add(Mesh::Name::Cube, new CubeMesh());
		MeshManager::Add(Mesh::Name::Pyramid, new PyramidMesh());
		MeshManager::Add(Mesh::Name::Diamond, new DiamondMesh());
		MeshManager::Add(Mesh::Name::Cross, new CrossMesh());
		MeshManager::Add(Mesh::Name::Plane, new PlaneMesh());
		MeshManager::Add(Mesh::Name::Crate, new MeshProto("wooden crate.proto.azul"));
		MeshManager::Add(Mesh::Name::Frigate, new MeshProto("space_frigate.proto.azul"));
		MeshManager::Add(Mesh::Name::Bracket, new MeshProto("bracket.proto.azul"));
		MeshManager::Add(Mesh::Name::Fish, new MeshProto("BarramundiFish.proto.azul"));
		MeshManager::Add(Mesh::Name::DogHouse, new MeshProto("doghouse0908.proto.azul"));
		MeshManager::Add(Mesh::Name::R2D2, new MeshProto("R2-D2.proto.azul"));
		MeshManager::Add(Mesh::Name::Duck, new MeshProto("Duck.proto.azul"));
		MeshManager::Add(Mesh::Name::Corset, new MeshProto("Corset.proto.azul"));
		MeshManager::Add(Mesh::Name::AntiqueCameraTripod, new MeshProto("AntiqueCamera.proto.azul"));
		MeshManager::Add(Mesh::Name::AntiqueCamera, new MeshProto("AntiqueCamera.proto.azul", 1));
		MeshManager::Add(Mesh::Name::Dog, new MeshProto("dog.proto.azul"));
		MeshManager::Add(Mesh::Name::WesternTownHouse, new MeshProto("western_town_house.proto.azul", 0));
		MeshManager::Add(Mesh::Name::UnitSphere, new MeshProto("UnitSphere.proto.azul"));
		MeshManager::Add(Mesh::Name::UnitIcoSphere, new MeshProto("UnitIcoSphere.proto.azul"));
		MeshManager::Add(Mesh::Name::TranslateHandle, new MeshProto("TranslateHandle.proto.azul"));
		MeshManager::Add(Mesh::Name::ScaleHandle, new MeshProto("ScaleHandle.proto.azul"));
		MeshManager::Add(Mesh::Name::RotateHandle, new MeshProto("RotateHandle.proto.azul"));
		MeshManager::Add(Mesh::Name::ChickenBotSkin, new MeshProto("walk_mesh.skin.proto.azul"));
		MeshManager::Add(Mesh::Name::MannequinSkin, new MeshProto("mannequin.skin.proto.azul"));
		MeshManager::Add(Mesh::Name::PaladinSkin, new MeshProto("Paladin.skin.proto.azul"));
		MeshManager::Add(Mesh::Name::KnightSkin, new MeshProto("Knight.skin.proto.azul"));

		TextureObjectManager::Add(TextureObject::Name::SimpleRock, new TextureProto("rocks_pack.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::SimpleRock2, new TextureProto("rocks_pack.proto.azul", 8));
		TextureObjectManager::Add(TextureObject::Name::SimpleRock3, new TextureProto("rocks_pack.proto.azul", 9));
		TextureObjectManager::Add(TextureObject::Name::SimpleRock4, new TextureProto("rocks_pack.proto.azul", 10));
		TextureObjectManager::Add(TextureObject::Name::SimpleRock5, new TextureProto("rocks_pack.proto.azul", 11));
		TextureObjectManager::Add(TextureObject::Name::SimpleRock6, new TextureProto("rocks_pack.proto.azul", 12));

		MeshManager::Add(Mesh::Name::SimpleRock, new MeshProto("rocks_pack.proto.azul", 0));
		MeshManager::Add(Mesh::Name::SimpleRock2, new MeshProto("rocks_pack.proto.azul", 8));
		MeshManager::Add(Mesh::Name::SimpleRock3, new MeshProto("rocks_pack.proto.azul", 9));
		MeshManager::Add(Mesh::Name::SimpleRock4, new MeshProto("rocks_pack.proto.azul", 10));
		MeshManager::Add(Mesh::Name::SimpleRock5, new MeshProto("rocks_pack.proto.azul", 11));
		MeshManager::Add(Mesh::Name::SimpleRock6, new MeshProto("rocks_pack.proto.azul", 12));

		TextureObjectManager::Add(TextureObject::Name::DryGrass0, new TextureProto("grass_var.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::DryGrass1, new TextureProto("grass_var.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::DryGrass2, new TextureProto("grass_var.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::DryGrass3, new TextureProto("grass_var.proto.azul", 0));

		MeshManager::Add(Mesh::Name::DryGrass0, new MeshProto("grass_var.proto.azul", 0));
		MeshManager::Add(Mesh::Name::DryGrass1, new MeshProto("grass_var.proto.azul", 1));
		MeshManager::Add(Mesh::Name::DryGrass2, new MeshProto("grass_var.proto.azul", 2));
		MeshManager::Add(Mesh::Name::DryGrass3, new MeshProto("grass_var.proto.azul", 3));

		TextureObjectManager::Add(TextureObject::Name::TropicalTree0, new TextureProto("trees_new.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::TropicalTree1, new TextureProto("trees_new.proto.azul", 1));
		TextureObjectManager::Add(TextureObject::Name::TropicalTree2, new TextureProto("trees_new.proto.azul", 2));
		TextureObjectManager::Add(TextureObject::Name::TropicalTree3, new TextureProto("trees_new.proto.azul", 3));
		MeshManager::Add(Mesh::Name::TropicalTree0, new MeshProto("trees_new.proto.azul", 0));
		MeshManager::Add(Mesh::Name::TropicalTree1, new MeshProto("trees_new.proto.azul", 1));
		MeshManager::Add(Mesh::Name::TropicalTree2, new MeshProto("trees_new.proto.azul", 2));
		MeshManager::Add(Mesh::Name::TropicalTree3, new MeshProto("trees_new.proto.azul", 3));

		TextureObjectManager::Add(TextureObject::Name::BirchTree0, new TextureProto("birch_trees.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::BirchTree1, new TextureProto("birch_trees.proto.azul", 1));
		MeshManager::Add(Mesh::Name::BirchTree0, new MeshProto("birch_trees.proto.azul", 0));
		MeshManager::Add(Mesh::Name::BirchTree1, new MeshProto("birch_trees.proto.azul", 1));

		int meshI = 0;
		MeshManager::Add(Mesh::Name::DesertRock0, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock1, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock2, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock3, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock4, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock5, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock6, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock7, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock8, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock9, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock10, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock11, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock12, new MeshProto(desertRocksData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::DesertRock13, new MeshProto(desertRocksData.meshes[meshI++]));

		meshI = 0;
		MeshManager::Add(Mesh::Name::Tattoine0, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine1, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine2, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine3, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine4, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine5, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine6, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine7, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine8, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine9, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine10, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine11, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine12, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine13, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine14, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine15, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine16, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine17, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine18, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine19, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine20, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine21, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine22, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine23, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine24, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine25, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine26, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine27, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine28, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine29, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine30, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine31, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine32, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine33, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine34, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine35, new MeshProto(tattoineData.meshes[meshI++]));
		MeshManager::Add(Mesh::Name::Tattoine36, new MeshProto(tattoineData.meshes[meshI++]));

		//ClipManager::Add(Clip::Name::ShotUp, new ClipProto("shot_up.anim.proto.azul"));
		//ClipManager::Add(Clip::Name::Run, new ClipProto("run.anim.proto.azul"));
		//ClipManager::Add(Clip::Name::Walk, new ClipProto("walk_mesh.anim.proto.azul"));
		//ClipManager::Add(Clip::Name::SidestepRight, new ClipProto("sidestep_right.anim.proto.azul"));
		//ClipManager::Add(Clip::Name::HitFront, new ClipProto("hit_front.anim.proto.azul"));
	}

	Game& Game::GetInstance()
	{
		if (!pInstance)
		{
			pInstance = new Game();
			pEngineInstance = pInstance;
		}

		return *pInstance;
	}

	void Game::ClearDepthStencilBuffer(const Vec4& color)
	{
		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;
		pContext->ClearRenderTargetView(pRenderTargetView, (const FLOAT*)&color);
		pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, (FLOAT)clearDepth, (UINT8)clearStencil);

		SODefault* pShader = (SODefault*)ShaderObjectManager::Find(ShaderObject::Name::Default);
		pShader->SetFogParameters(fogDistMin, fogDistMax, color);
		SOSkinLightTexture* pShaderSkin = (SOSkinLightTexture*)ShaderObjectManager::Find(ShaderObject::Name::SkinLightTexture);
		pShaderSkin->SetFogParameters(fogDistMin, fogDistMax, color);
		SOTerrain* pShaderTerrain = (SOTerrain*)ShaderObjectManager::Find(ShaderObject::Name::Terrain);
		pShaderTerrain->SetFogParameters(fogDistMin, fogDistMax, color);
		SOFoliage* pShaderFoliage = (SOFoliage*)ShaderObjectManager::Find(ShaderObject::Name::Foliage);
		pShaderFoliage->SetFogParameters(fogDistMin, fogDistMax, color);
	}
}