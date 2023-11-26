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
		azulModel_proto aB_proto;
		aB_proto.ParseFromString(EngineUtils::FileToString("desert_rocks.proto.azul"));
		azulModel desertRocksData;
		desertRocksData.Deserialize(aB_proto);

		ShaderObjectManager::Create();
		ShaderObjectManager::Add(ShaderObject::Name::Null, new SONull());
		ShaderObjectManager::Add(ShaderObject::Name::ColorByVertex, new SOColorByVertex());
		ShaderObjectManager::Add(ShaderObject::Name::FlatTexture, new SOFlatTexture());
		ShaderObjectManager::Add(ShaderObject::Name::LightTexture, new SOLightTexture());
		ShaderObjectManager::Add(ShaderObject::Name::ConstColor, new SOConstColor());
		ShaderObjectManager::Add(ShaderObject::Name::Default, new SODefault());

		TextureObjectManager::Create();
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

		TextureObjectManager::Add(TextureObject::Name::Bone, new TextureProto("Bone.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Bone_001, new TextureProto("Bone_001.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Bone_L, new TextureProto("Bone_L.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Bone_L_001, new TextureProto("Bone_L_001.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Bone_L_002, new TextureProto("Bone_L_002.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Bone_R, new TextureProto("Bone_R.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Bone_R_001, new TextureProto("Bone_R_001.proto.azul", 0));
		TextureObjectManager::Add(TextureObject::Name::Bone_R_002, new TextureProto("Bone_R_002.proto.azul", 0));

		MeshManager::Create();
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

		MeshManager::Add(Mesh::Name::Bone, new MeshProto("Bone.proto.azul", 0));
		MeshManager::Add(Mesh::Name::Bone_001, new MeshProto("Bone_001.proto.azul", 0));
		MeshManager::Add(Mesh::Name::Bone_L, new MeshProto("Bone_L.proto.azul", 0));
		MeshManager::Add(Mesh::Name::Bone_L_001, new MeshProto("Bone_L_001.proto.azul", 0));
		MeshManager::Add(Mesh::Name::Bone_L_002, new MeshProto("Bone_L_002.proto.azul", 0));
		MeshManager::Add(Mesh::Name::Bone_R, new MeshProto("Bone_R.proto.azul", 0));
		MeshManager::Add(Mesh::Name::Bone_R_001, new MeshProto("Bone_R_001.proto.azul", 0));
		MeshManager::Add(Mesh::Name::Bone_R_002, new MeshProto("Bone_R_002.proto.azul", 0));

		Vec3 robotPos = Vec3(30, 3, 30);
		Vec3 animPos = Vec3(30, -1, -50);

		CameraManager::Create();

		Camera* pCamera = CameraManager::Add(Camera::Name::Default, new Camera());
		pCamera->SetOrientAndPosition(Vec3(0.f, 1.f, 0.f), animPos + (Vec3(0, 2, 0)), animPos + Vec3(-10, 2, 0));
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

		ClipManager::Create();
		ClipManager::Add(Clip::Name::Walk, new Clip(NUM_BONES, Clip::Name::Walk));
		ClipManager::Add(Clip::Name::SidestepRight, new Clip(NUM_BONES, Clip::Name::SidestepRight));
		ClipManager::Add(Clip::Name::HitFront, new Clip(NUM_BONES, Clip::Name::HitFront));
		ClipManager::Add(Clip::Name::Run, new Clip(NUM_BONES, Clip::Name::Run));
		ClipManager::Add(Clip::Name::ShotUp, new Clip(NUM_BONES, Clip::Name::ShotUp));

		AnimTime delta = 0.4f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		animDeltaTime = delta;

		pAnimController = new AnimController(animDeltaTime, Clip::Name::Run);

		pAnimController1 = new AnimController(delta, Clip::Name::HitFront);
		pAnimController1->GetObjectPivot()->SetTrans(Vec3(30, -1, -50) + Vec3(0, 4, 4));
		pAnimController1->GetObjectPivot()->SetScale(Vec3(50.f, 50.f, 50.f));

		pAnimController2 = new AnimController(delta, Clip::Name::SidestepRight);
		pAnimController2->GetObjectPivot()->SetTrans(Vec3(30, -1, -50) + Vec3(0, 0, 4));
		pAnimController2->GetObjectPivot()->SetScale(Vec3(50.f, 50.f, 50.f));

		pAnimController3 = new AnimController(delta, Clip::Name::ShotUp);
		pAnimController3->GetObjectPivot()->SetTrans(Vec3(30, -1, -50) + Vec3(0, 0, -4));
		pAnimController3->GetObjectPivot()->SetScale(Vec3(50.f, 50.f, 50.f));

		pAnimController4 = new AnimController(delta, Clip::Name::Walk);
		pAnimController4->GetObjectPivot()->SetTrans(Vec3(30, -1, -50) + Vec3(0, 4, -4));
		pAnimController4->GetObjectPivot()->SetScale(Vec3(50.f, 50.f, 50.f));

		//GameObject* pBone = GameObjectManager::SpawnObject(
		//	"Bone",
		//	Mesh::Name::Bone,
		//	TextureObject::Name::Bone,
		//	robotPos
		//);
		//pBone->SetRelativeScale(200.f);
		//pBone->SetRelativeRotation(Rot(Rot1::Z, MATH_PI2));

		//pBone = GameObjectManager::SpawnObject(
		//	"Bone_001",
		//	Mesh::Name::Bone_001,
		//	TextureObject::Name::Bone_001,
		//	robotPos + Vec3(0, 0, 3)
		//);
		//pBone->SetRelativeScale(200.f);
		//pBone->SetRelativeRotation(Rot(Rot1::Y, -MATH_PI2));

		//pBone = GameObjectManager::SpawnObject(
		//	"Bone_L",
		//	Mesh::Name::Bone_L,
		//	TextureObject::Name::Bone_L,
		//	robotPos + Vec3(-2, 0, 0)
		//);
		//pBone->SetRelativeScale(200.f);
		//pBone->SetRelativeRotation(Rot(Rot1::Y, -MATH_PI2));

		//pBone = GameObjectManager::SpawnObject(
		//	"Bone_R",
		//	Mesh::Name::Bone_R,
		//	TextureObject::Name::Bone_R,
		//	robotPos + Vec3(2, 0, 0)
		//);
		//pBone->SetRelativeScale(200.f);
		//pBone->SetRelativeRotation(Rot(Rot1::Y, -MATH_PI2));

		//pBone = GameObjectManager::SpawnObject(
		//	"Bone_L_001",
		//	Mesh::Name::Bone_L_001,
		//	TextureObject::Name::Bone_L_001,
		//	robotPos + Vec3(-2, 0, 5)
		//);
		//pBone->SetRelativeScale(200.f);
		//pBone->SetRelativeRotation(Rot(Rot1::Y, -MATH_PI2));

		//pBone = GameObjectManager::SpawnObject(
		//	"Bone_R_001",
		//	Mesh::Name::Bone_R_001,
		//	TextureObject::Name::Bone_R_001,
		//	robotPos + Vec3(2, 0, 5)
		//);
		//pBone->SetRelativeScale(200.f);
		//pBone->SetRelativeRotation(Rot(Rot1::Y, -MATH_PI2));

		//pBone = GameObjectManager::SpawnObject(
		//	"Bone_L_002",
		//	Mesh::Name::Bone_L_002,
		//	TextureObject::Name::Bone_L_002,
		//	robotPos + Vec3(-2, 0, 9)
		//);
		//pBone->SetRelativeScale(200.f);
		//pBone->SetRelativeRotation(Rot(Rot1::Y, -MATH_PI2));

		//pBone = GameObjectManager::SpawnObject(
		//	"Bone_R_002",
		//	Mesh::Name::Bone_R_002,
		//	TextureObject::Name::Bone_R_002,
		//	robotPos + Vec3(2, 0, 9)
		//);
		//pBone->SetRelativeScale(200.f);
		//pBone->SetRelativeRotation(Rot(Rot1::Y, -MATH_PI2));

		LightSource = GameObjectManager::SpawnObject(
			"Light Yellow",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Diamond),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				lightColor),
			lightPos);

		LightSource->SetRelativeScale(Vec3(.1f, .1f, .1f));

		LightSource1 = GameObjectManager::SpawnObject(
			"Light Red",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Diamond),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(1.f, 0.f, 0.f)),
			lightPos + Vec3(5, 0, 0));

		LightSource1->SetRelativeScale(Vec3(.1f, .1f, .1f));

		LightSource2 = GameObjectManager::SpawnObject(
			"Light Blue",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Diamond),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0.f, 0.f, 1.f)),
			lightPos + Vec3(-5, 0, 0));

		LightSource2->SetRelativeScale(Vec3(.1f, .1f, .1f));

		GameObject* pFish = GameObjectManager::SpawnObject(
			"Strange Fish",
			Mesh::Name::Fish,
			TextureObject::Name::Fish,
			Vec3(20.f, 3.f, 20.f)
		);

		pFish->SetRelativeScale(10.f);

		GameObject* pDogHouse = GameObjectManager::SpawnObject(
			"Dog House",
			Mesh::Name::DogHouse,
			TextureObject::Name::DogHouse,
			Vec3(0.f, -1.f, 30.f)
		);

		pDogHouse->SetRelativeScale(5.f);

		GameObjectManager::SpawnObject(
			"R2D2",
			Mesh::Name::R2D2,
			TextureObject::Name::R2D2,
			Vec3(60.f, .5f, -50.f)
		)->SetRelativeRotation(Rot(Rot1::Y, MATH_PI2 * .01f));

		GameObject* pDuck = GameObjectManager::SpawnObject(
			"Duck",
			Mesh::Name::Duck,
			TextureObject::Name::Duck,
			Vec3(-10.f, -.25f, -20.f)
		);

		pDuck->SetRelativeScale(0.05f);

		GameObject* pCorset = GameObjectManager::SpawnObject(
			"Corset",
			Mesh::Name::Corset,
			TextureObject::Name::Corset,
			Vec3(-20.f, 1.f, 20.f)
		);

		pCorset->SetRelativeScale(30.f);

		Vec3 cameraPos = Vec3(0.f, -1.f, -10.f);

		GameObject* pAntiqueCameraTripod = GameObjectManager::SpawnObject(
			"Antique Camera Tripod",
			Mesh::Name::AntiqueCameraTripod,
			TextureObject::Name::AntiqueCameraTripod,
			cameraPos
		);

		pAntiqueCameraTripod->SetRelativeScale(0.5f);

		pAntiqueCamera = GameObjectManager::SpawnObject(
			"Antique Camera",
			Mesh::Name::AntiqueCamera,
			TextureObject::Name::AntiqueCamera,
			Vec3(), pAntiqueCameraTripod
		);

		GameObjectManager::SpawnObject(
			"Dog",
			Mesh::Name::Dog,
			TextureObject::Name::Dog,
			Vec3(-2.f, -1.f, 10.f)
		);

		GameObject* pHouse = GameObjectManager::SpawnObject(
			"Western Town House",
			Mesh::Name::WesternTownHouse,
			TextureObject::Name::WesternTownHouse,
			Vec3(-40.f, -1.f, -40.f)
		);

		pHouse->SetRelativeScale(.3f);
		pHouse->SetRelativeRotation(Rot(Rot1::X, -MATH_PI / 2.f));

		GameObject* pDesertRock = GameObjectManager::SpawnObject(
			"Desert Rock",
			Mesh::Name::DesertRock12,
			TextureObject::Name::DesertRock12,
			Vec3(80.f, -1.f, -80.f)
		);
		pDesertRock->SetRelativeRotation(Rot(Rot3::XYZ, -MATH_PI2, -MATH_PI4, 0.f));
		pDesertRock->SetRelativeScale(3.f);

		pDesertRock = GameObjectManager::SpawnObject(
			"Desert Rock 11",
			Mesh::Name::DesertRock11,
			TextureObject::Name::DesertRock11,
			Vec3(58.f, 7.f, -90.f)
		);

		pDesertRock->SetRelativeRotation(Rot(Rot3::XYZ, -MATH_PI2, -MATH_PI4, 0.f));
		pDesertRock->SetRelativeScale(5.f);

		pDesertRock = GameObjectManager::SpawnObject(
			"Desert Rock 0",
			Mesh::Name::DesertRock0,
			TextureObject::Name::DesertRock0,
			Vec3(35.f, 3.f, -85.f)
		);

		pDesertRock->SetRelativeRotation(Rot(Rot3::XYZ, -MATH_PI2, MATH_PI / 2.f, 0.f));
		pDesertRock->SetRelativeScale(Vec3(8.f, 8.f, 8.f));

		GameObjectManager::SpawnObject(
			"Test Cross",
			Mesh::Name::Cross,
			TextureObject::Name::Crate,
			Vec3(50.f, 0.f, 0.f)
		);

		GameObjectManager::SpawnObject(
			"Wooden Crate",
			Mesh::Name::Crate,
			TextureObject::Name::Crate,
			Vec3(5.f, 0.f, 0.f)
		);

		GameObjectManager::SpawnObject(
			"Bracket",
			Mesh::Name::Bracket,
			TextureObject::Name::Brick,
			Vec3(-7.f, 0.f, 0.f)
		);

		GameObjectManager::SpawnObject(
			"Space Frigate",
			Mesh::Name::Frigate,
			TextureObject::Name::Frigate,
			Vec3(0.f, 10.f, 14.5f)
		)->SetRelativeScale(Vec3(0.25f, 0.25f, 0.25f));

		GameObjectManager::SpawnObject(
			"Floor Plane",
			Mesh::Name::Plane,
			TextureObject::Name::Desert,
			Vec3(0.f, -1.f, 0.f)
		);

		GameObjectManager::SpawnObject(
			"BIG Pyramid",
			new GOLightTexture(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::Default),
				TextureObjectManager::Find(TextureObject::Name::Desert)
			),
			Vec3(-50, 8, 50)
		)->SetRelativeScale(Vec3(10.f, 10.f, 10.f));

		GameObjectManager::SpawnObject(
			"BIG Pyramid",
			new GOLightTexture(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::Default),
				TextureObjectManager::Find(TextureObject::Name::Desert)
			),
			Vec3(-70, 6, 30)
		)->SetRelativeScale(Vec3(7.f, 7.f, 7.f));

		GameObjectManager::SpawnObject(
			"BIG Pyramid",
			new GOLightTexture(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::Default),
				TextureObjectManager::Find(TextureObject::Name::Desert)
			),
			Vec3(-30, 4, 70)
		)->SetRelativeScale(Vec3(5.f, 5.f, 5.f));

		constexpr static float markerLength = 20.f;
		GameObject* pXMarker = GameObjectManager::SpawnObject("X Marker",
			new GOConstColor(MeshManager::Find(Mesh::Name::Cube), ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(Azul::Colors::Blue)), Vec3(markerLength / 4.f, 0.f, 0.f)
		);

		pXMarker->SetRelativeScale(Vec3(markerLength, .001f, .001f));
		pXMarker->SetRenderShell(false);

		GameObject* pYMarker = GameObjectManager::SpawnObject("Y Marker",
			new GOConstColor(MeshManager::Find(Mesh::Name::Cube), ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(Azul::Colors::Yellow)), Vec3(0.f, markerLength / 4.f, 0.f)
		);

		pYMarker->SetRelativeScale(Vec3(0.001f, markerLength, .001f));
		pYMarker->SetRenderShell(false);

		GameObject* pZMarker = GameObjectManager::SpawnObject("Z Marker",
			new GOConstColor(MeshManager::Find(Mesh::Name::Cube), ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(Azul::Colors::Red)), Vec3(0.f, 0.f, markerLength / 4.f)
		);

		pZMarker->SetRelativeScale(Vec3(0.001f, 0.001f, markerLength));
		pZMarker->SetRenderShell(false);

		GameObjectManager::SpawnObject("Sphere",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::UnitSphere),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(Azul::Colors::Orange)), Vec3(0, 20, 0)
		);

		GameObjectManager::SpawnObject("Ico Sphere",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::UnitIcoSphere),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(Azul::Colors::Green)), Vec3(5, 20, 0)
		);

		//LoadFloor(13);
		LoadClock(2, Vec3(0.f, 30.f, 80.f));
		LoadPylon(1, Vec3(30.f, 7.f, 0.f));
		LoadColorObjects();
		LoadLightObjects();
		LoadMovingObjects();
		//LoadInstancedObjects();

		pPlayer = GameObjectManager::SpawnObject("Player", new Player(), Vec3(0.f, 2.f, -10.f));

		SODefault* pShader = (SODefault*)ShaderObjectManager::Find(ShaderObject::Name::Default);
		pShader->SetDirectionalLightParameters(Vec3(-1, -1, 1).getNorm(), .01f * Vec3(1, 1, 1), .5f * Vec3(1, 1, 1), Vec3(0.5f, 0.5f, 0.5f));

		GameObject::SetRenderShellGlobal(false);
		Engine::ToggleMaxFramerate(false);

		GameObjectManager::Dump();
		//ShaderObjectManager::Dump();
		//MeshManager::Dump();
		//CameraManager::Dump();
		//ClipManager::Dump();
		return true;
	}

	void Game::Update(float deltaTime)
	{
		UpdateDemo(deltaTime);

		pAnimController->Update();
		pAnimController1->Update();
		pAnimController2->Update();
		pAnimController3->Update();
		pAnimController4->Update();

		if (GetKeyState('5') & 0x8000)
		{
			pAnimController->SetClip(Clip::Name::Walk);
		}
		else if (GetKeyState('6') & 0x8000)
		{
			pAnimController->SetClip(Clip::Name::Run);
		}
		else if (GetKeyState('7') & 0x8000)
		{
			pAnimController->SetClip(Clip::Name::ShotUp);
		}
		else if (GetKeyState('8') & 0x8000)
		{
			pAnimController->SetClip(Clip::Name::HitFront);
		}
		else if (GetKeyState('9') & 0x8000)
		{
			pAnimController->SetClip(Clip::Name::SidestepRight);
		}
		else if (GetKeyState('1') & 0x8000)
		{
			animDeltaTime = animDeltaTime - (0.025f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}
		else if (GetKeyState('2') & 0x8000)
		{
			animDeltaTime = animDeltaTime + (0.025f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		static bool isReversePressed = false;
		if (GetKeyState('3') & 0x8000 && !isReversePressed)
		{
			isReversePressed = true;
			animDeltaTime = -animDeltaTime;
		}
		else if (!(GetKeyState('3') & 0x8000) && isReversePressed)
		{
			isReversePressed = false;
		}

		pAnimController->GetTimerController()->SetDeltaTime(animDeltaTime);

		CameraManager::Update(deltaTime);
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
		pPyr1->SetRelativeRotation(Rot(Rot1::Z, MATH_PI * 1.5f));

		GameObject* pPyr2 = GameObjectManager::SpawnObject("Clock Outer Pyramid",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(0.f, .5f, 0.f) * size, pC2);
		pPyr2->SetRelativeScale(scale * .1f);
		pPyr2->SetRelativeRotation(Rot(Rot1::Z, MATH_PI * 2.f));

		GameObject* pPyr3 = GameObjectManager::SpawnObject("Clock Outer Pyramid",
			new GOConstColor(
				MeshManager::Find(Mesh::Name::Pyramid),
				ShaderObjectManager::Find(ShaderObject::Name::ConstColor),
				Vec3(0, 0, 1)), Vec3(0.f, -.5f, 0.f) * size, pC3);
		pPyr3->SetRelativeScale(scale * .1f);
		pPyr3->SetRelativeRotation(Rot(Rot1::Z, MATH_PI * 1.f));

		GameObject* pPyr4 = GameObjectManager::SpawnObject("Clock Outer Pyramid",
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

		pPyramid->SetRelativeRotation(Rot(Rot3::XYZ, rot1, rot1, rot1));
		pPyramid1->SetRelativeRotation(Rot(Rot3::XYZ, rot1, 0.f, 0.f));
		pPyramid2->SetRelativeRotation(Rot(Rot3::XYZ, rot1, 0.f, 0.f));
		pPyramid3->SetRelativeRotation(Rot(Rot3::XYZ, rot1, 0.f, 0.f));
		pPyramid4->SetRelativeRotation(Rot(Rot3::XYZ, rot1, 0.f, 0.f));
		rot1 += 1.f * deltaTime;

		pCenter->SetRelativeRotation(Rot(Rot1::Z, rot1));

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
		LightSource1->SetRelativeLocation(lightPos + Vec3(5, 0, 0));
		LightSource2->SetRelativeLocation(lightPos + Vec3(-5, 0, 0));

		SODefault* pShader = (SODefault*)ShaderObjectManager::Find(ShaderObject::Name::Default);
		pShader->SetPointLightParameters(0, lightPos, 1500.f, 0.3f * Vec3(1.f, 1.f, 1.f), Vec3(0.3f, 0.3f, 0.3f), Vec3(0.8f, .8f, .6f), Vec3(0.8f, .8f, .6f));
		pShader->SetPointLightParameters(1, lightPos + Vec3(5, 0, 0), 1500.f, 0.3f * Vec3(1.f, 1.f, 1.f), Vec3(0.3f, 0.3f, 0.3f), Vec3(.8f, .1f, .1f), Vec3(0.8f, .8f, .6f));
		pShader->SetPointLightParameters(2, lightPos + Vec3(-5, 0, 0), 1500.f, 0.3f * Vec3(1.f, 1.f, 1.f), Vec3(0.3f, 0.3f, 0.3f), Vec3(.1f, .1f, .8f), Vec3(0.8f, .8f, .6f));

		static float cameraTheta = 0.f;

		if (GetKeyState('Z') & 0x8000)
		{
			cameraTheta += 2 * deltaTime;
		}
		else if (GetKeyState('C') & 0x8000)
		{
			cameraTheta -= 2 * deltaTime;
		}

		pAntiqueCamera->SetRelativeRotation(Rot(Rot1::Y, cameraTheta));

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
		const Vec4 ClearColor = Azul::Colors::SkyBlue;
#else
		const Vec4 ClearColor = Azul::Colors::DarkDarkGrey;
#endif
		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;
		pContext->ClearRenderTargetView(pRenderTargetView, (const FLOAT*)&ClearColor);
		pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, (FLOAT)clearDepth, (UINT8)clearStencil);
	}
}