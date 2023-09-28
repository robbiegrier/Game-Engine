#include <d3d11.h>
#include <d3dcompiler.h>
#include "Game.h"
#include "Engine.h"
#include "MathEngine.h"
#include "Camera.h"
#include "Colors.h"
#include "GameObject.h"
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
#include "GOConstColor.h"
#include "SOConstColor.h"
#include "GOFlatTexture.h"
#include "SOFlatTexture.h"
#include "Player.h"
#include "GONull.h"
#include "SONull.h"
#include "SODefault.h"

namespace Azul
{
	Game* Game::pInstance = nullptr;

	ShaderObject* poShaderColorByVertex;
	Mesh* pModelPyramid;
	Mesh* pModelCube;

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
	GameObject* pPlayer;

	Vec3 lightColor = Vec3(Azul::Colors::LightYellow);
	Vec3 lightPos = Vec3(0, 5, 0);

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
	}

	Game::~Game()
	{
	}

	bool Game::LoadContent()
	{
		ShaderObjectManager::Create();
		ShaderObjectManager::Add(ShaderObject::Name::Null, new SONull());
		ShaderObjectManager::Add(ShaderObject::Name::ColorByVertex, new SOColorByVertex());
		ShaderObjectManager::Add(ShaderObject::Name::FlatTexture, new SOFlatTexture());
		ShaderObjectManager::Add(ShaderObject::Name::LightTexture, new SOLightTexture());
		ShaderObjectManager::Add(ShaderObject::Name::ConstColor, new SOConstColor());
		ShaderObjectManager::Add(ShaderObject::Name::Default, new SODefault());

		TextureObjectManager::Create();
		TextureObjectManager::Add(TextureObject::Name::Brick, new TextureObject(TextureObject::Name::Brick, L"RedBrick.tga"));
		TextureObjectManager::Add(TextureObject::Name::Rocks, new TextureObject(TextureObject::Name::Rocks, L"Rocks.tga"));
		TextureObjectManager::Add(TextureObject::Name::Duckweed, new TextureObject(TextureObject::Name::Duckweed, L"Duckweed.tga"));
		TextureObjectManager::Add(TextureObject::Name::Stone, new TextureObject(TextureObject::Name::Stone, L"Stone.tga"));

		MeshManager::Create();
		MeshManager::Add(Mesh::Name::Cube, new CubeMesh());
		MeshManager::Add(Mesh::Name::Pyramid, new PyramidMesh());
		MeshManager::Add(Mesh::Name::Diamond, new DiamondMesh());
		MeshManager::Add(Mesh::Name::Cross, new CrossMesh());
		MeshManager::Add(Mesh::Name::Plane, new PlaneMesh());

		CameraManager::Create();

		Camera* pCamera = CameraManager::Add(Camera::Name::Default, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), Vec3(), Vec3(0.f, 2.f, -10.f));
		pCamera->SetPerspective(50.0f, GetAspectRatio(), 0.1f, 1000.0f);

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

		GameObjectManager::Create();

		LightSource = GameObjectManager::SpawnObject("Light", new GOConstColor(MeshManager::Find(Mesh::Name::Diamond), ShaderObjectManager::Find(ShaderObject::Name::ConstColor), lightColor), lightPos);
		LightSource->SetRelativeScale(Vec3(.1f, .1f, .1f));

		ShaderObject* pLightTestShader = ShaderObjectManager::Find(ShaderObject::Name::Default);

		GameObjectManager::SpawnObject("PX Cube",
			new GOLightTexture(MeshManager::Find(Mesh::Name::Cube), pLightTestShader, TextureObjectManager::Find(TextureObject::Name::Brick)), Vec3(5, 0, 0)
		);
		GameObjectManager::SpawnObject("PX Diamond",
			new GOLightTexture(MeshManager::Find(Mesh::Name::Diamond), pLightTestShader, TextureObjectManager::Find(TextureObject::Name::Duckweed)), Vec3(-7, 0, 0)
		);
		GameObjectManager::SpawnObject("PX Pyramid",
			new GOLightTexture(MeshManager::Find(Mesh::Name::Pyramid), pLightTestShader, TextureObjectManager::Find(TextureObject::Name::Stone)), Vec3(0, 0, 4.5)
		);
		GameObjectManager::SpawnObject("Floor Plane",
			new GOLightTexture(MeshManager::Find(Mesh::Name::Plane), pLightTestShader, TextureObjectManager::Find(TextureObject::Name::Brick)), Vec3(0, -1.f, 0)
		);

		constexpr static float markerLength = 6.f;
		GameObjectManager::SpawnObject("X Marker",
			new GOConstColor(MeshManager::Find(Mesh::Name::Cube), ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(Azul::Colors::Blue)), Vec3(markerLength / 4.f, 0.f, 0.f)
		)->SetRelativeScale(Vec3(markerLength, .001f, .001f));

		GameObjectManager::SpawnObject("Y Marker",
			new GOConstColor(MeshManager::Find(Mesh::Name::Cube), ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(Azul::Colors::Yellow)), Vec3(0.f, markerLength / 4.f, 0.f)
		)->SetRelativeScale(Vec3(0.001f, markerLength, .001f));

		GameObjectManager::SpawnObject("Z Marker",
			new GOConstColor(MeshManager::Find(Mesh::Name::Cube), ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(Azul::Colors::Red)), Vec3(0.f, 0.f, markerLength / 4.f)
		)->SetRelativeScale(Vec3(0.001f, 0.001f, markerLength));

		//LoadFloor(13);
		LoadClock(2, Vec3(0.f, 30.f, 80.f));
		LoadPylon(1, Vec3(30.f, 7.f, 0.f));
		LoadColorObjects();
		LoadLightObjects();
		LoadMovingObjects();
		LoadInstancedObjects();

		pPlayer = GameObjectManager::SpawnObject("Player", new Player(), Vec3(0.f, 2.f, -10.f));

		GameObjectManager::Dump();
		//ShaderObjectManager::Dump();
		//MeshManager::Dump();
		//CameraManager::Dump();
		return true;
	}

	void Game::Update(float deltaTime)
	{
		UpdateDemo(deltaTime);

		if (GetKeyState('F') & 0x8000)
		{
			lightPos[y] += 10 * deltaTime;
		}
		if (GetKeyState('V') & 0x8000)
		{
			lightPos[y] -= 10 * deltaTime;
		}

		if (GetKeyState('E') & 0x8000)
		{
			lightPos[x] += 10 * deltaTime;
		}
		if (GetKeyState('T') & 0x8000)
		{
			lightPos[x] -= 10 * deltaTime;
		}

		if (GetKeyState('4') & 0x8000)
		{
			lightPos[z] += 10 * deltaTime;
		}
		if (GetKeyState('R') & 0x8000)
		{
			lightPos[z] -= 10 * deltaTime;
		}

		LightSource->SetRelativeLocation(lightPos);

		SODefault* pShader = (SODefault*)ShaderObjectManager::Find(ShaderObject::Name::Default);
		pShader->SetPointLightParameters(lightPos, 1500.f, 0.3f * Vec3(1.f, 1.f, 1.f), Vec3(0.3f, 0.3f, 0.3f), Vec3(0.8f, .8f, .6f), Vec3(0.8f, .8f, .6f));

		CameraManager::Update(deltaTime);
		GameObjectManager::Update(deltaTime);
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

		GameObject* pC1 = GameObjectManager::SpawnObject("Clock Inner Cube",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cube),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 1)), Vec3(3.f, 0.f, 0.f) * size, pCenter);
		pC1->SetRelativeScale(scale * 2);

		GameObject* pC2 = GameObjectManager::SpawnObject("Clock Inner Cube",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cube),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 1)), Vec3(0.f, 3.f, 0.f) * size, pCenter);
		pC2->SetRelativeScale(scale * 2);

		GameObject* pC3 = GameObjectManager::SpawnObject("Clock Inner Cube",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cube),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 1)), Vec3(0.f, -3.f, 0.f) * size, pCenter);
		pC3->SetRelativeScale(scale * 2);

		GameObject* pC4 = GameObjectManager::SpawnObject("Clock Inner Cube",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Cube),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1, 0, 1)), Vec3(-3.f, 0.f, 0.f) * size, pCenter);
		pC4->SetRelativeScale(scale * 2);

		GameObject* pPyr1 = GameObjectManager::SpawnObject("Clock Outer Pyramid",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(.5f, 0.f, 0.f) * size, pC1);
		pPyr1->SetRelativeScale(scale * .1f);
		pPyr1->SetRelativeRotation(Mat4(Mat4::Rot1::Z, MATH_PI * 1.5f));

		GameObject* pPyr2 = GameObjectManager::SpawnObject("Clock Outer Pyramid",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(0.f, .5f, 0.f) * size, pC2);
		pPyr2->SetRelativeScale(scale * .1f);
		pPyr2->SetRelativeRotation(Mat4(Mat4::Rot1::Z, MATH_PI * 2.f));

		GameObject* pPyr3 = GameObjectManager::SpawnObject("Clock Outer Pyramid",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(0.f, -.5f, 0.f) * size, pC3);
		pPyr3->SetRelativeScale(scale * .1f);
		pPyr3->SetRelativeRotation(Mat4(Mat4::Rot1::Z, MATH_PI * 1.f));

		GameObject* pPyr4 = GameObjectManager::SpawnObject("Clock Outer Pyramid",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(-.5f, 0.f, 0.f) * size, pC4);
		pPyr4->SetRelativeScale(scale * .1f);
		pPyr4->SetRelativeRotation(Mat4(Mat4::Rot1::Z, MATH_PI * 2.5f));
	}

	void Game::LoadPylon(float size, const Vec3& pos)
	{
		static_cast<void>(lightPos);
		static_cast<void>(lightColor);

		Vec3 scale = Vec3(size, size, size);

		pPyramid = GameObjectManager::SpawnObject("Pyramid", Mesh::Name::Diamond, ShaderObject::Name::ColorByVertex, pos);
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
		pCube2 = GameObjectManager::SpawnObject("Small Cube 1", Mesh::Name::Cube, ShaderObject::Name::ColorByVertex, Vec3(0.f, 10.f, -30.f));
		pCube3 = GameObjectManager::SpawnObject("Small Cube 1", Mesh::Name::Cube, ShaderObject::Name::ColorByVertex, Vec3(-3.f, 10.f, -30.f));

		pCube1->SetRelativeScale(Vec3(.8f, .8f, .8f));
		pCube2->SetRelativeScale(Vec3(.8f, .8f, .8f));
		pCube3->SetRelativeScale(Vec3(.8f, .8f, .8f));

		pTexture1 = GameObjectManager::SpawnObject("Texture Block",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Diamond), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Rocks)),
			Vec3(-10.f, 0.f, -14.f)
		);
		pLightBlock->SetRelativeScale(Vec3(2.f, 2.f, 2.f));

		pTexture2 = GameObjectManager::SpawnObject("Texture Block",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Diamond), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Stone)),
			Vec3(-8.f, 0.f, -16.f)
		);
		pLightBlock->SetRelativeScale(Vec3(2.f, 2.f, 2.f));

		pTexture3 = GameObjectManager::SpawnObject("Texture Block",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Diamond), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Brick)),
			Vec3(-12.f, 0.f, -12.f)
		);
		pLightBlock->SetRelativeScale(Vec3(2.f, 2.f, 2.f));
	}

	void Game::LoadInstancedObjects()
	{
		GameObject* pInst1 = GameObjectManager::SpawnObject("Instance Cross",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Cross), ShaderObjectManager::Find(ShaderObject::Name::FlatTexture), TextureObjectManager::Find(TextureObject::Name::Rocks)),
			Vec3(-15.f, 0.f, 0.f)
		);
		pInst1->SetRelativeScale(Vec3(0.5f, 0.5f, 0.1f));

		GameObject* pInst2 = GameObjectManager::SpawnObject("Instance Cross",
			new GOFlatTexture(MeshManager::Find(Mesh::Name::Cross), ShaderObjectManager::Find(ShaderObject::Name::LightTexture), TextureObjectManager::Find(TextureObject::Name::Duckweed)),
			Vec3(-15.f, 0.f, 3.f)
		);
		pInst2->SetRelativeScale(Vec3(0.5f, 0.5f, 0.1f));

		GameObject* pInst3 = GameObjectManager::SpawnObject("Instance Cross",
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

		GOLightTexture* pLightTexture = (GOLightTexture*)pLightDiamond->GetGraphicsObject();

		if (GetKeyState('V') & 0x8000)
		{
			pLightTexture->pTex = TextureObjectManager::Find(TextureObject::Name::Brick);
		}
		else if (GetKeyState('B') & 0x8000)
		{
			pLightTexture->pTex = TextureObjectManager::Find(TextureObject::Name::Stone);
		}
		else if (GetKeyState('N') & 0x8000)
		{
			pLightTexture->pTex = TextureObjectManager::Find(TextureObject::Name::Duckweed);
		}
		else if (GetKeyState('M') & 0x8000)
		{
			pLightTexture->pTex = TextureObjectManager::Find(TextureObject::Name::Rocks);
		}

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

		pPyramid->SetRelativeRotation(Mat4(Mat4::Rot3::XYZ, rot1, rot1, rot1));
		pPyramid1->SetRelativeRotation(Mat4(Mat4::Rot3::XYZ, rot1, 0.f, 0.f));
		pPyramid2->SetRelativeRotation(Mat4(Mat4::Rot3::XYZ, rot1, 0.f, 0.f));
		pPyramid3->SetRelativeRotation(Mat4(Mat4::Rot3::XYZ, rot1, 0.f, 0.f));
		pPyramid4->SetRelativeRotation(Mat4(Mat4::Rot3::XYZ, rot1, 0.f, 0.f));
		rot1 += 1.f * deltaTime;

		pCenter->SetRelativeRotation(Mat4(Mat4::Rot1::Z, rot1));

		pCube1->SetRelativeLocation(pCube1->GetRelativeLocation() + Vec3(-trans1 * transSign * deltaTime, 0, 0));
		pCube2->SetRelativeLocation(pCube2->GetRelativeLocation() + Vec3(0, trans1 * transSign * deltaTime, 0));
		pCube3->SetRelativeLocation(pCube3->GetRelativeLocation() + Vec3(trans1 * transSign * deltaTime, 0, 0));

		if (pCube2->GetLocation()[y] < 0.f)
		{
			transSign = 1.f;
		}
		else if (pCube2->GetLocation()[y] > 10.f)
		{
			transSign = -1.f;
		}

		pTexture1->SetRelativeLocation(pTexture1->GetRelativeLocation() + Vec3(texTrans1 * texTransSign * deltaTime, 0, 0));
		pTexture2->SetRelativeLocation(pTexture2->GetRelativeLocation() + Vec3(texTrans1 * texTransSign * deltaTime, 0, 0));
		pTexture3->SetRelativeLocation(pTexture3->GetRelativeLocation() + Vec3(texTrans1 * texTransSign * deltaTime, 0, 0));

		if (pTexture1->GetLocation()[x] < -15.f)
		{
			texTransSign = 1.f;
		}
		else if (pTexture1->GetLocation()[x] > 10.f)
		{
			texTransSign = -1.f;
		}
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

	void Game::ClearDepthStencilBuffer()
	{
#ifdef _DEBUG
		const Vec4 ClearColor = Azul::Colors::Black;
#else
		const Vec4 ClearColor = Azul::Colors::DarkDarkGrey;
#endif
		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;
		pContext->ClearRenderTargetView(pRenderTargetView, (const FLOAT*)&ClearColor);
		pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, (FLOAT)clearDepth, (UINT8)clearStencil);
	}
}