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
#include "AnimController.h"
#include "GameObjectBasic.h"
#include "Anim.h"
#include "ClipManager.h"
#include "ClipProto.h"
#include "SOEditorVisual.h"
#include "SceneManager.h"
#include "GameObjectSprite.h"
#include "GOSprite.h"
#include "ImageManager.h"

namespace Azul
{
	Game* Game::pInstance = nullptr;

	ShaderObject* poShaderColorByVertex;
	Mesh* pModelPyramid;
	Mesh* pModelCube;

	AnimController* pAnimController;
	AnimController* pAnimController1;
	AnimController* pAnimController2;
	AnimController* pAnimController3;
	AnimController* pAnimController4;

	AnimTime animDeltaTime;

	GameObject* pPyramid;
	GameObject* pPyramid1;
	GameObject* pPyramid2;
	GameObject* pPyramid3;
	GameObject* pPyramid4;
	GameObject* pCube;
	GameObject* pLightBlock;
	GameObject* pLightPyramid;
	GameObject* pLightDiamond;
	GameObject* pTexture1;
	GameObject* pTexture2;
	GameObject* pTexture3;
	GameObject* pCube1;
	GameObject* pCube2;
	GameObject* pCube3;
	GameObject* pCenter;
	GameObject* pTexObject;
	GameObject* LightSource;
	GameObject* LightSource1;
	GameObject* LightSource2;
	GameObject* pPlayer;
	GameObject* pAntiqueCamera;

	Vec3 lightColor = Vec3(Azul::Colors::LightYellow);
	Vec3 lightPos = Vec3(50, 2, -40);

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

		LoadShaders();
		LoadCameras();
		LoadAssets();

		ImageManager::Create();
		ImageManager::Add(Image::Name::ALLBirds, new Image(Image::Name::ALLBirds, TextureObjectManager::Find(TextureObject::Name::Birds), Rect(0.0f, 0.0f, 377.0f, 234.0f)));
		ImageManager::Add(Image::Name::WhiteBird, new Image(Image::Name::WhiteBird, TextureObject::Name::Birds, Rect(139.0f, 131.0f, 84.0f, 97.0f)));
		ImageManager::Add(Image::Name::GreenBird, new Image(Image::Name::GreenBird, TextureObject::Name::Birds, Rect(244.0f, 134.0f, 102.0f, 75.0f)));
		//ImageManager::Add(Image::Name::Stitch, new Image(Image::Name::Stitch, TextureObject::Name::Stitch, Rect(0.0f, 0.0f, 300.0f, 410.0f)));
		//ImageManager::Add(Image::Name::Text, new Image(Image::Name::Text, TextureObject::Name::CenturyFont, Rect(0.0f, 0.0f, 100.0f, 100.0f)));

		// Scene Directional Light
		SODefault* pShader = (SODefault*)ShaderObjectManager::Find(ShaderObject::Name::Default);
		pShader->SetDirectionalLightParameters(Vec3(-1, -1, 1).getNorm(), .01f * Vec3(1, 1, 1), .5f * Vec3(1, 1, 1), Vec3(0.5f, 0.5f, 0.5f));

		GameObject::SetRenderShellGlobal(false);
		Engine::ToggleMaxFramerate(false);

		SceneManager::ChangeScene("AzulScene");
		//GameObjectManager::Dump();

		GameObjectManager::SpawnObject("pBirdsTmp 2",
			new GameObjectSprite(new GOSprite(ImageManager::Find(Image::Name::ALLBirds), Rect(400, 300, 100, 100))),
			Vec3(100.f, 100.f, 0.f)
		);

		//GameObjectManager::SpawnObject("Stitch",
		//	new GameObjectSprite(new GOSprite(ImageManager::Find(Image::Name::Stitch), Rect(400, 300, 100, 150))),
		//	Vec3(1075.f, 250.f, 0.f)
		//)->SetRelativeRotation(Rot(Rot1::Z, MATH_PI4));

		return true;
	}

	void Game::Update(float deltaTime)
	{
		UpdateDemo(deltaTime);
		GameObjectManager::Update(intervalTimer.Toc());
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
		ClipManager::Destroy();
		ImageManager::Destroy();

		delete pAnimController;
		delete pAnimController1;
		delete pAnimController2;
		delete pAnimController3;
		delete pAnimController4;
	}

	void Game::EndFrame()
	{
		intervalTimer.Tic();
	}

	void Game::LoadFloor(int size)
	{
		float gridSize = (float)size;
		float floorScale = 1.f;
		float step = floorScale / 2.f;

		for (float xf = -gridSize; xf < gridSize; xf += step)
		{
			for (float yf = -gridSize; yf < gridSize; yf += step)
			{
				TextureObject* pTex = TextureObjectManager::Find(TextureObject::Name::Rocks);

				if (((xf <= 0 && xf >= -5) || (xf >= 0 && xf < 5)) && ((yf <= 0 && yf >= -5) || (yf >= 0 && yf <= 5)))
				{
					pTex = TextureObjectManager::Find(TextureObject::Name::Stone);
				}

				float zCoord = -floorScale;

				if (fabs(fabs(xf) - gridSize) < 0.01f)
					zCoord += step;
				if (fabs(fabs(xf + 1) - gridSize) < 0.01f)
					zCoord += step;
				if (fabs(fabs(yf) - gridSize) < 0.01f)
					zCoord += step;
				if (fabs(fabs(yf + 1) - gridSize) < 0.01f)
					zCoord += step;

				GameObject* pFloor = GameObjectManager::SpawnObject("Floor",
					new GOLightTexture(
						MeshManager::Find(Mesh::Name::Cube),
						ShaderObjectManager::Find(ShaderObject::Name::Default),
						pTex
					),
					Vec3((float)xf, zCoord, (float)yf)
				);
				pFloor->SetRelativeScale(Vec3(floorScale, floorScale, floorScale));
			}
		}
	}

	void Game::LoadClock(float size, const Vec3& pos)
	{
		static_cast<void>(lightPos);
		static_cast<void>(lightColor);

		Vec3 scale = Vec3(size, size, size);

		pCenter = GameObjectManager::SpawnObject("Clock Center",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cross),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 0)), pos);
		pCenter->SetRelativeScale(scale);

		GameObject* pC1 = GameObjectManager::SpawnObject("Clock Inner Cube 1",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cube),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 1)), Vec3(3.f, 0.f, 0.f) * size, pCenter);
		pC1->SetRelativeScale(scale * 2);

		GameObject* pC2 = GameObjectManager::SpawnObject("Clock Inner Cube 2",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cube),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 1)), Vec3(0.f, 3.f, 0.f) * size, pCenter);
		pC2->SetRelativeScale(scale * 2);

		GameObject* pC3 = GameObjectManager::SpawnObject("Clock Inner Cube 3",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cube),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 1)), Vec3(0.f, -3.f, 0.f) * size, pCenter);
		pC3->SetRelativeScale(scale * 2);

		GameObject* pC4 = GameObjectManager::SpawnObject("Clock Inner Cube 4",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cube),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 1)), Vec3(-3.f, 0.f, 0.f) * size, pCenter);
		pC4->SetRelativeScale(scale * 2);

		GameObject* pPyr1 = GameObjectManager::SpawnObject("Clock Outer Pyramid 1",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(.5f, 0.f, 0.f) * size, pC1);
		pPyr1->SetRelativeScale(scale * .1f);
		pPyr1->SetRelativeRotation(Rot(Rot1::Z, MATH_PI * 1.5f));

		GameObject* pPyr2 = GameObjectManager::SpawnObject("Clock Outer Pyramid 2",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(0.f, .5f, 0.f) * size, pC2);
		pPyr2->SetRelativeScale(scale * .1f);
		pPyr2->SetRelativeRotation(Rot(Rot1::Z, MATH_PI * 2.f));

		GameObject* pPyr3 = GameObjectManager::SpawnObject("Clock Outer Pyramid 3",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(0.f, -.5f, 0.f) * size, pC3);
		pPyr3->SetRelativeScale(scale * .1f);
		pPyr3->SetRelativeRotation(Rot(Rot1::Z, MATH_PI * 1.f));

		GameObject* pPyr4 = GameObjectManager::SpawnObject("Clock Outer Pyramid 4",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(-.5f, 0.f, 0.f) * size, pC4);
		pPyr4->SetRelativeScale(scale * .1f);
		pPyr4->SetRelativeRotation(Rot(Rot1::Z, MATH_PI * 2.5f));
	}

	void Game::LoadPylon(float size, const Vec3& pos)
	{
		static_cast<void>(lightPos);
		static_cast<void>(lightColor);

		Vec3 scale = Vec3(size, size, size);

		pPyramid = GameObjectManager::SpawnObject("Diamond Main", Mesh::Name::Diamond, ShaderObject::Name::ColorByVertex, pos);
		pPyramid1 = GameObjectManager::SpawnObject("Small Pyramid 1", Mesh::Name::Pyramid, ShaderObject::Name::ColorByVertex, Vec3(1.3f, 1.3f, 1.3f), pPyramid);
		pPyramid2 = GameObjectManager::SpawnObject("Small Pyramid 2", Mesh::Name::Pyramid, ShaderObject::Name::ColorByVertex, Vec3(-1.3f, 1.3f, 1.3f), pPyramid);
		pPyramid3 = GameObjectManager::SpawnObject("Small Pyramid 3", Mesh::Name::Pyramid, ShaderObject::Name::ColorByVertex, Vec3(-1.3f, 1.3f, -1.3f), pPyramid);
		pPyramid4 = GameObjectManager::SpawnObject("Small Pyramid 4", Mesh::Name::Pyramid, ShaderObject::Name::ColorByVertex, Vec3(1.3f, 1.3f, -1.3f), pPyramid);

		pPyramid->SetRelativeScale(scale);
		pPyramid1->SetRelativeScale(scale * .25f);
		pPyramid2->SetRelativeScale(scale * .25f);
		pPyramid3->SetRelativeScale(scale * .25f);
		pPyramid4->SetRelativeScale(scale * .25f);
	}

	void Game::LoadColorObjects()
	{
		GameObjectManager::SpawnObject("Color Cross", Mesh::Name::Cross, ShaderObject::Name::ColorByVertex, Vec3(10.f, 0.f, 10.f))->SetRelativeScale(Vec3(.5f, .5f, .5f));
		GameObjectManager::SpawnObject("Color Cube", Mesh::Name::Cube, ShaderObject::Name::ColorByVertex, Vec3(14.f, -1.f, 10.f))->SetRelativeScale(Vec3(2.f, 2.f, 2.f));
		GameObjectManager::SpawnObject("Color Diamond", Mesh::Name::Diamond, ShaderObject::Name::ColorByVertex, Vec3(14.f, -0.0f, 14.f))->SetRelativeScale(Vec3(1.0f, 1.0f, 1.0f));
		GameObjectManager::SpawnObject("Color Pyramid", Mesh::Name::Pyramid, ShaderObject::Name::ColorByVertex, Vec3(10.f, -1.f, 14.f))->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
	}

	void Game::LoadLightObjects()
	{
		pLightBlock = GameObjectManager::SpawnObject("Light Block",
			new GOLightTexture(MeshManager::Find(Mesh::Name::Cube), ShaderObjectManager::Find(ShaderObject::Name::Default), TextureObjectManager::Find(TextureObject::Name::Brick)),
			Vec3(-10.f, 0.f, 14.f)
		);
		pLightBlock->SetRelativeScale(Vec3(2.f, 2.f, 2.f));

		pLightPyramid = GameObjectManager::SpawnObject("Light Pyramid",
			new GOLightTexture(MeshManager::Find(Mesh::Name::Pyramid), ShaderObjectManager::Find(ShaderObject::Name::Default), TextureObjectManager::Find(TextureObject::Name::Duckweed)),
			Vec3(-10.f, 0.f, 10.f)
		);
		pLightPyramid->SetRelativeScale(Vec3(1, 1, 1));

		pLightDiamond = GameObjectManager::SpawnObject("Light Diamond",
			new GOLightTexture(MeshManager::Find(Mesh::Name::Diamond), ShaderObjectManager::Find(ShaderObject::Name::Default), TextureObjectManager::Find(TextureObject::Name::Stone)),
			Vec3(-14.f, 0.f, 12.f)
		);
		pLightDiamond->SetRelativeScale(Vec3(1, 1, 1));
	}

	void Game::LoadMovingObjects()
	{
		pCube1 = GameObjectManager::SpawnObject("Small Cube 1", Mesh::Name::Cube, ShaderObject::Name::ColorByVertex, Vec3(3.f, 10.f, -30.f));
		pCube2 = GameObjectManager::SpawnObject("Small Cube 2", Mesh::Name::Cube, ShaderObject::Name::ColorByVertex, Vec3(0.f, 10.f, -30.f));
		pCube3 = GameObjectManager::SpawnObject("Small Cube 3", Mesh::Name::Cube, ShaderObject::Name::ColorByVertex, Vec3(-3.f, 10.f, -30.f));

		pCube1->SetRelativeScale(Vec3(.8f, .8f, .8f));
		pCube2->SetRelativeScale(Vec3(.8f, .8f, .8f));
		pCube3->SetRelativeScale(Vec3(.8f, .8f, .8f));

		pTexture1 = GameObjectManager::SpawnObject("Texture Block 1",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Diamond), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Rocks)),
			Vec3(-10.f, 0.f, -14.f)
		);
		pLightBlock->SetRelativeScale(Vec3(2.f, 2.f, 2.f));

		pTexture2 = GameObjectManager::SpawnObject("Texture Block 2",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Diamond), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Stone)),
			Vec3(-8.f, 0.f, -16.f)
		);
		pLightBlock->SetRelativeScale(Vec3(2.f, 2.f, 2.f));

		pTexture3 = GameObjectManager::SpawnObject("Texture Block 3",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Diamond), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Brick)),
			Vec3(-12.f, 0.f, -12.f)
		);
		pLightBlock->SetRelativeScale(Vec3(2.f, 2.f, 2.f));
	}

	void Game::LoadInstancedObjects()
	{
		GameObject* pInst1 = GameObjectManager::SpawnObject("Instance Cross 1",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Cross), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Rocks)),
			Vec3(-15.f, 0.f, 0.f)
		);
		pInst1->SetRelativeScale(Vec3(0.5f, 0.5f, 0.1f));

		GameObject* pInst2 = GameObjectManager::SpawnObject("Instance Cross 2",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Cross), ShaderObjectManager::Find(ShaderObject::Name::LightTexture), TextureObjectManager::Find(TextureObject::Name::Duckweed)),
			Vec3(-15.f, 0.f, 3.f)
		);
		pInst2->SetRelativeScale(Vec3(0.5f, 0.5f, 0.1f));

		GameObject* pInst3 = GameObjectManager::SpawnObject("Instance Cross 3",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Cross), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Brick)),
			Vec3(-15.f, 0.f, -3.f)
		);
		pInst3->SetRelativeScale(Vec3(0.5f, 0.25f, 0.1f));
	}

	void Game::UpdateDemo(float deltaTime)
	{
		static float rot1 = 0.f;
		static float scale1 = 0.2f;
		static float scaleSign = 1.f;
		static float trans1 = 1.05f;
		static float transSign = 1.f;
		static float texTrans1 = 1.05f;
		static float texTransSign = 1.f;

		//GOLightTexture* pLightTexture = (GOLightTexture*)pLightDiamond->GetGraphicsObject();

		//if (GetKeyState('V') & 0x8000)
		//{
		//	pLightTexture->pTex = TextureObjectManager::Find(TextureObject::Name::Brick);
		//}
		//else if (GetKeyState('B') & 0x8000)
		//{
		//	pLightTexture->pTex = TextureObjectManager::Find(TextureObject::Name::Stone);
		//}
		//else if (GetKeyState('N') & 0x8000)
		//{
		//	pLightTexture->pTex = TextureObjectManager::Find(TextureObject::Name::Duckweed);
		//}
		//else if (GetKeyState('M') & 0x8000)
		//{
		//	pLightTexture->pTex = TextureObjectManager::Find(TextureObject::Name::Rocks);
		//}

		//pCube->SetScale(Vec3(scale1, scale1, scale1));
		scale1 += 0.003f * scaleSign;

		if (scale1 > 0.5f)
		{
			scaleSign = -1.f;
		}
		else if (scale1 < .1f)
		{
			scaleSign = 1.f;
		}

		//pPyramid->SetRelativeRotation(Rot(Rot3::XYZ, rot1, rot1, rot1));
		//pPyramid1->SetRelativeRotation(Rot(Rot3::XYZ, rot1, 0.f, 0.f));
		//pPyramid2->SetRelativeRotation(Rot(Rot3::XYZ, rot1, 0.f, 0.f));
		//pPyramid3->SetRelativeRotation(Rot(Rot3::XYZ, rot1, 0.f, 0.f));
		//pPyramid4->SetRelativeRotation(Rot(Rot3::XYZ, rot1, 0.f, 0.f));
		rot1 += 1.f * deltaTime;

		//pCenter->SetRelativeRotation(Rot(Rot1::Z, rot1));

		//pCube1->SetRelativeLocation(pCube1->GetRelativeLocation() + Vec3(-trans1 * transSign * deltaTime, 0, 0));
		//pCube2->SetRelativeLocation(pCube2->GetRelativeLocation() + Vec3(0, trans1 * transSign * deltaTime, 0));
		//pCube3->SetRelativeLocation(pCube3->GetRelativeLocation() + Vec3(trans1 * transSign * deltaTime, 0, 0));

		//if (pCube2->GetWorldLocation()[y] < 0.f)
		//{
		//	transSign = 1.f;
		//}
		//else if (pCube2->GetWorldLocation()[y] > 10.f)
		//{
		//	transSign = -1.f;
		//}

		//pTexture1->SetRelativeLocation(pTexture1->GetRelativeLocation() + Vec3(texTrans1 * texTransSign * deltaTime, 0, 0));
		//pTexture2->SetRelativeLocation(pTexture2->GetRelativeLocation() + Vec3(texTrans1 * texTransSign * deltaTime, 0, 0));
		//pTexture3->SetRelativeLocation(pTexture3->GetRelativeLocation() + Vec3(texTrans1 * texTransSign * deltaTime, 0, 0));

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

		//if (GetKeyState('E') & 0x8000)
		//{
		//	lightPos[x] += 10 * deltaTime;
		//}
		//if (GetKeyState('T') & 0x8000)
		//{
		//	lightPos[x] -= 10 * deltaTime;
		//}

		//if (GetKeyState('4') & 0x8000)
		//{
		//	lightPos[z] += 10 * deltaTime;
		//}
		//if (GetKeyState('R') & 0x8000)
		//{
		//	lightPos[z] -= 10 * deltaTime;
		//}

	/*	LightSource->SetRelativeLocation(lightPos);
		LightSource1->SetRelativeLocation(lightPos + Vec3(5, 0, 0));
		LightSource2->SetRelativeLocation(lightPos + Vec3(-5, 0, 0));*/

		SODefault* pShader = (SODefault*)ShaderObjectManager::Find(ShaderObject::Name::Default);
		pShader->SetPointLightParameters(0, lightPos, 1500.f, 0.3f * Vec3(1.f, 1.f, 1.f), Vec3(0.3f, 0.3f, 0.3f), Vec3(0.8f, .8f, .6f), Vec3(0.8f, .8f, .6f));
		pShader->SetPointLightParameters(1, lightPos + Vec3(5, 0, 0), 1500.f, 0.3f * Vec3(1.f, 1.f, 1.f), Vec3(0.3f, 0.3f, 0.3f), Vec3(.8f, .1f, .1f), Vec3(0.8f, .8f, .6f));
		pShader->SetPointLightParameters(2, lightPos + Vec3(-5, 0, 0), 1500.f, 0.3f * Vec3(1.f, 1.f, 1.f), Vec3(0.3f, 0.3f, 0.3f), Vec3(.1f, .1f, .8f), Vec3(0.8f, .8f, .6f));

		//static float cameraTheta = 0.f;

		//if (GetKeyState('Z') & 0x8000)
		//{
		//	cameraTheta += 2 * deltaTime;
		//}
		//else if (GetKeyState('C') & 0x8000)
		//{
		//	cameraTheta -= 2 * deltaTime;
		//}

		//pAntiqueCamera->SetRelativeRotation(Rot(Rot1::Y, cameraTheta));

		//UpdateTimerDemo(deltaTime);
	}

	void Game::UpdateTimerDemo(float deltaTime)
	{
		static int i = 0;
		Trace::out("%d: %f\n", i++, deltaTime);

		AnimTime elapsedTime = intervalTimer.Toc();
		int timeInSeconds_ms = AnimTime::Quotient(elapsedTime, AnimTime(AnimTime::Duration::ONE_MILLISECOND));
		AnimTime timeInMs_remainder = AnimTime::Remainder(elapsedTime, AnimTime(AnimTime::Duration::ONE_MILLISECOND));
		int timeInSeconds_us_remainder = AnimTime::Quotient(timeInMs_remainder, AnimTime(AnimTime::Duration::ONE_MICROSECOND));

		Trace::out(" time in     ms : %d \n", timeInSeconds_ms);
		Trace::out(" remainder   us : %d \n", timeInSeconds_us_remainder);
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
	}

	void Game::LoadCameras()
	{
		Camera* pCamera = CameraManager::Add(Camera::Name::Default, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), Vec3(0, 0, 10), Vec3(0, 0, 0));
		pCamera->SetPerspective(65.0f, GetAspectRatio(), 0.1f, 1000.0f);

		pCamera = CameraManager::Add(Camera::Name::Player, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), Vec3(0, 0, 10), Vec3(0, 0, 0));
		pCamera->SetPerspective(85.0f, GetAspectRatio(), 0.1f, 1000.0f);

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
		TextureObjectManager::Add(TextureObject::Name::Hovered, new TextureProto("Hovered.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Disabled, new TextureProto("Disabled.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Birds, new TextureProto("Birds.proto.azul"));

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
		MeshManager::Add(Mesh::Name::AntiqueCameraTripod, new MeshProto("AntiqueCamera.proto.azul", 0));
		MeshManager::Add(Mesh::Name::AntiqueCamera, new MeshProto("AntiqueCamera.proto.azul", 1));
		MeshManager::Add(Mesh::Name::Dog, new MeshProto("dog.proto.azul"));
		MeshManager::Add(Mesh::Name::WesternTownHouse, new MeshProto("western_town_house.proto.azul", 0));
		MeshManager::Add(Mesh::Name::UnitSphere, new MeshProto("UnitSphere.proto.azul"));
		MeshManager::Add(Mesh::Name::UnitIcoSphere, new MeshProto("UnitIcoSphere.proto.azul"));
		MeshManager::Add(Mesh::Name::TranslateHandle, new MeshProto("TranslateHandle.proto.azul"));
		MeshManager::Add(Mesh::Name::ScaleHandle, new MeshProto("ScaleHandle.proto.azul"));
		MeshManager::Add(Mesh::Name::RotateHandle, new MeshProto("RotateHandle.proto.azul"));

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

		//MeshManager::Add(Mesh::Name::Bone, new MeshProto("Bone.proto.azul", 0));
		//MeshManager::Add(Mesh::Name::Bone_001, new MeshProto("Bone_001.proto.azul", 0));
		//MeshManager::Add(Mesh::Name::Bone_L, new MeshProto("Bone_L.proto.azul", 0));
		//MeshManager::Add(Mesh::Name::Bone_L_001, new MeshProto("Bone_L_001.proto.azul", 0));
		//MeshManager::Add(Mesh::Name::Bone_L_002, new MeshProto("Bone_L_002.proto.azul", 0));
		//MeshManager::Add(Mesh::Name::Bone_R, new MeshProto("Bone_R.proto.azul", 0));
		//MeshManager::Add(Mesh::Name::Bone_R_001, new MeshProto("Bone_R_001.proto.azul", 0));
		//MeshManager::Add(Mesh::Name::Bone_R_002, new MeshProto("Bone_R_002.proto.azul", 0));

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
	}
}