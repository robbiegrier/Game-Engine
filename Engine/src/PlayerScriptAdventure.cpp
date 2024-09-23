#include "PlayerScriptAdventure.h"
#include "Engine.h"
#include "Terrain.h"
#include "TerrainSystem.h"
#include "CameraManager.h"
#include "QuadTree.h"
#include "GOSurfaceShape.h"
#include "MeshManager.h"
#include "ShaderObjectManager.h"
#include "TextureObjectManager.h"
#include "GameObjectManager.h"
#include "Animator.h"

namespace Azul
{
	PlayerScriptAdventure::PlayerScriptAdventure()
	{
		pGoMoveIndicator = new GOSurfaceShape(
			MeshManager::Find(Mesh::Name::Cylinder),
			ShaderObjectManager::Find(ShaderObject::Name::SurfaceShape),
			TextureObjectManager::Find(TextureObject::Name::Yellow)
		);

		pGoCharacterIndicator = new GOSurfaceShape(
			MeshManager::Find(Mesh::Name::Cylinder),
			ShaderObjectManager::Find(ShaderObject::Name::SurfaceShape),
			TextureObjectManager::Find(TextureObject::Name::Yellow)
		);
	}

	PlayerScriptAdventure::~PlayerScriptAdventure()
	{
		delete pGoMoveIndicator;
		delete pGoCharacterIndicator;
	}

	void PlayerScriptAdventure::Start()
	{
		PlayerScript::Start();
		Trace::out("\tPlayer Adventure: Start!\n");

		pCharacter = GameObjectManager::FindObject("Paladin");

		CameraManager::SetCurrentCamera(Camera::Name::Player);
		pPlayerCamera = CameraManager::GetCurrentCamera();

		Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
		float groundHeightAtPos = pTerrain->ComputeBarycentricHeight(pPlayerCamera->GetLocation());

		startViewOffset = Vec3(0.f, 50.f, 50.f);

		Vec3 terrainPos = Vec3(0.f, groundHeightAtPos, 0.f);
		Vec3 startPos = terrainPos + startViewOffset;

		pPlayerCamera->SetOrientAndPosition(pPlayerCamera->GetUp(), terrainPos, startPos);
		GetPlayer()->SetRelativeLocation(pPlayerCamera->GetLocation());

		for (Iterator& it = *pCharacter->GetComponents().GetIterator(); !it.IsDone(); it.Next())
		{
			Animator* pAnimator = (Animator*)it.Curr();
			pAnimator->SetActiveClip(Clip::Name::PaladinIdle);
			pAnimator->SetAnimationSpeed(1.f);
		}

		Engine::SetMouseVisibility(true);
	}

	void PlayerScriptAdventure::Update(float deltaTime)
	{
		PlayerScript::Update(deltaTime);

		CheckMovement(deltaTime);
		CheckRotate(deltaTime);

		static bool lbutDown = false;
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!lbutDown)
			{
				lbutDown = true;
				Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();

				POINT p;
				GetCursorPos(&p);
				HWND hwnd{ 0 };
				ScreenToClient(hwnd, &p);

				//Trace::out("point: %d %d\n", p.x, p.y);

				float mouseX = 2.f * -(((float)Engine::GetWindowWidth() / 2.f) - (float)p.x) / (float)Engine::GetWindowWidth();
				float mouseY = 2.f * (((float)Engine::GetWindowHeight() / 2.f) - (float)p.y) / (float)Engine::GetWindowHeight();
				//Trace::out("pointf: %f %f\n", mouseX, mouseY);

				Vec3 rayDir = pPlayerCamera->GetRay(mouseX, mouseY);

				Vec3 raycast;
				if (pTerrain->GetQuadTree()->Raycast(pPlayerCamera->GetLocation(), rayDir, raycast))
				{
					pGoMoveIndicator->SetWorld(Scale(1.75f, 1.75f, 1.75f) * Trans(raycast));
					drawMoveIndicator = true;

					//Trace::out("Draw move ind \n");
				}
			}
		}
		else
		{
			lbutDown = false;
		}

		float characterSpeed = 15.f;

		if (drawMoveIndicator)
		{
			Vec3 target2D = Vec3(pGoMoveIndicator->GetWorld().get(Row4::i3));
			target2D[y] = 0.f;

			Vec3 loc2D = pCharacter->GetWorldLocation();
			loc2D[y] = 0.f;

			Vec3 toTarget2D = target2D - loc2D;

			if (toTarget2D.len() < 0.1f)
			{
				drawMoveIndicator = false;

				for (Iterator& it = *pCharacter->GetComponents().GetIterator(); !it.IsDone(); it.Next())
				{
					Animator* pAnimator = (Animator*)it.Curr();
					pAnimator->SetActiveClip(Clip::Name::PaladinIdle);
					pAnimator->SetAnimationSpeed(1.f);
				}
			}
			else
			{
				pCharacter->AddWorldLocationOffset(toTarget2D.getNorm() * characterSpeed * deltaTime);

				Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
				float groundHeightAtPos = pTerrain->ComputeBarycentricHeight(pCharacter->GetWorldLocation());
				Vec3 loc = pCharacter->GetWorldLocation();
				Vec3 newLoc = Vec3(loc.x(), groundHeightAtPos, loc.z());
				Vec3 delta = newLoc - loc;
				pCharacter->AddWorldLocationOffset(delta);

				pCharacter->SetWorldRotation(Quat(Rot(Orient::LocalToWorld, toTarget2D, Vec3(0, 1, 0))));
				pGoCharacterIndicator->SetWorld(Scale(1.5f, 1.5f, 1.5f) * Trans(pCharacter->GetWorldLocation()));

				for (Iterator& it = *pCharacter->GetComponents().GetIterator(); !it.IsDone(); it.Next())
				{
					Animator* pAnimator = (Animator*)it.Curr();
					pAnimator->SetActiveClip(Clip::Name::PaladinRun);
					pAnimator->SetAnimationSpeed(characterSpeed / 8.f);
				}
			}
		}

		//Vec3 lightPos = pCharacter->GetWorldLocation();
		//lightPos[y] += 8.f;

		//GameObject* pLight = GameObjectManager::FindObject("Light Yellow");
		//pLight->SetWorldLocation(lightPos);
		//pLight->SetRelativeScale(0.f);
	}

	void PlayerScriptAdventure::Draw()
	{
		if (drawMoveIndicator)
		{
			pGoMoveIndicator->Render();
		}

		//pGoCharacterIndicator->Render();
	}

	void PlayerScriptAdventure::CheckMovement(float deltaTime)
	{
		Vec3 cameraMove;
		float moveSpeed = 3.5f;
		float scrollSpeed = 0.15f;
		float scrollFalloff = 4.f;
		float moveFalloff = 5.f;

		if (GetKeyState('W') & 0x8000)
		{
			cameraMove[z] += moveSpeed;
		}
		if (GetKeyState('A') & 0x8000)
		{
			cameraMove[x] -= moveSpeed;
		}
		if (GetKeyState('D') & 0x8000)
		{
			cameraMove[x] += moveSpeed;
		}
		if (GetKeyState('S') & 0x8000)
		{
			cameraMove[z] -= moveSpeed;
		}

		scrollImpulse += (float)Engine::GetMouseWheelDelta() * scrollSpeed;
		pPlayerCamera->AddLocationOffset(Vec3(0.f, 0.f, 1.f) * scrollImpulse * deltaTime);

		if (Util::isNonZero(scrollImpulse, MATH_TOLERANCE))
		{
			scrollImpulse = EngineUtils::Lerp(scrollImpulse, 0.f, scrollFalloff * deltaTime);
		}

		Mat4 view = pPlayerCamera->GetViewMatrix();
		view.set(Row4::i3, Vec4(0.f, 0.f, 0.f, 1.f));

		moveImpulse += cameraMove;

		pPlayerCamera->AddHorizontalLocationOffset(moveImpulse * deltaTime);

		GetPlayer()->SetRelativeLocation(pPlayerCamera->GetLocation());

		if (Util::isNonZero(moveImpulse.len(), MATH_TOLERANCE))
		{
			moveImpulse = EngineUtils::Lerp(moveImpulse, Vec3(), moveFalloff * deltaTime);
		}
	}

	void PlayerScriptAdventure::CheckRotate(float deltaTime)
	{
		bool rotationRequired = false;
		float rotationSpeed = 1.25f;
		Vec3 rotator;

		if (GetKeyState('Q') & 0x8000)
		{
			rotationRequired = true;
			rotator[y] -= rotationSpeed;
		}
		if (GetKeyState('E') & 0x8000)
		{
			rotationRequired = true;
			rotator[y] += rotationSpeed;
		}
		if (GetKeyState(VK_MBUTTON) & 0x8000)
		{
			if (!rotOngoing)
			{
				Trace::out("Rotating!\n");

				rotOngoing = true;
			}

			POINT p;
			GetCursorPos(&p);
			HWND hwnd{ 0 };
			ScreenToClient(hwnd, &p);
			float offsetHorizontal = (float)p.x - (float)prevPoint.x;
			float offsetVertical = (float)p.y - (float)prevPoint.y;

			offsetHorizontal /= 1000.f;
			offsetVertical /= 1000.f;

			if (fabs(offsetHorizontal) > 0.005f || fabs(offsetVertical) > 0.005f)
			{
				constexpr float mouseLookSpeed = 75.f;

				Vec3 lookOffset = Vec3(offsetHorizontal, -offsetVertical, 0.f) * mouseLookSpeed;
				pPlayerCamera->AddLookAtOffset(lookOffset * deltaTime);

				prevPoint = p;
				SetCursorPos(Engine::GetWindowWidth() / 2, Engine::GetWindowHeight() / 2);
				prevPoint.x = (LONG)(Engine::GetWindowWidth() / 2) - (LONG)offsetHorizontal;
				prevPoint.y = (LONG)(Engine::GetWindowHeight() / 2) - (LONG)offsetVertical;
			}
		}
		else
		{
			GetCursorPos(&prevPoint);
			rotOngoing = false;

			if (rotationRequired)
			{
				if (!rotOngoing)
				{
					//Trace::out("Rotating!\n");

					rotOngoing = true;
				}

				Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();

				Vec3 raycast;
				pTerrain->GetQuadTree()->Raycast(pPlayerCamera->GetLocation(), -pPlayerCamera->GetDirection(), raycast);
				pivot = raycast;

				//Vec3 pivot = pPlayerCamera->GetLocation() + -pPlayerCamera->GetDirection() * 100.f;
				Vec3 delta = (pivot - pPlayerCamera->GetLocation()).cross(Vec3(0, 1, 0));

				pPlayerCamera->RotateAround(pivot, delta * rotator[y] * deltaTime);
			}
			else
			{
				rotOngoing = false;
			}
		}

		Engine::SetMouseVisibility(!rotOngoing);
	}
}