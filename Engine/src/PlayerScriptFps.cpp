#include "PlayerScriptFps.h"
#include "Engine.h"
#include "Terrain.h"
#include "TerrainSystem.h"
#include "CameraManager.h"

namespace Azul
{
	void PlayerScriptFps::Start()
	{
		PlayerScript::Start();
		Trace::out("\tPlayer FPS: Start!\n");

		CameraManager::SetCurrentCamera(Camera::Name::Player);
		pPlayerCamera = CameraManager::GetCurrentCamera();
	}

	void PlayerScriptFps::Update(float deltaTime)
	{
		PlayerScript::Update(deltaTime);

		CheckMovement(deltaTime);
		CheckLookAt(deltaTime);
		ProcessGravity(deltaTime);
	}

	void PlayerScriptFps::CheckMovement(float deltaTime)
	{
		Vec3 cameraMove;
		float moveSpeed = 10.f;
		float sprintSpeed = 1.5f;
		float jumpStrength = 20.f;

		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			moveSpeed *= sprintSpeed;
		}

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

		GetPlayer()->AddRelativeLocationOffset(cameraMove * deltaTime);
		pPlayerCamera->AddHorizontalLocationOffset(cameraMove * deltaTime);

		Vec3 cameraJump;

		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			if (!jumping)
			{
				jumping = true;
				jumpImpulse = jumpStrength;
			}
		}

		if (jumping)
		{
			cameraJump[y] += jumpImpulse;
			jumpImpulse = (jumpImpulse - (20.0f * deltaTime));
			cameraJump *= deltaTime;

			Vec3 loc = pPlayerCamera->GetLocation();
			Vec3 newLoc = Vec3(loc.x(), loc.y() + cameraJump[y], loc.z());
			Vec3 delta = newLoc - loc;
			pPlayerCamera->AddLocationOffset(delta);

			GetPlayer()->SetWorldLocation(pPlayerCamera->GetLocation());
		}
		else
		{
			Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
			float groundHeightAtPos = pTerrain->ComputeBarycentricHeight(pPlayerCamera->GetLocation());

			Vec3 loc = pPlayerCamera->GetLocation();
			Vec3 newLoc = Vec3(loc.x(), groundHeightAtPos + 4.f, loc.z());
			Vec3 delta = newLoc - loc;
			pPlayerCamera->AddLocationOffset(delta);

			GetPlayer()->SetWorldLocation(pPlayerCamera->GetLocation());
		}
	}

	void PlayerScriptFps::ProcessGravity(float deltaTime)
	{
		Vec3 cameraMove;
		float gravityStrength = 1.f;

		constexpr float earthGravityNormalizer = 10.f;
		gravityStrength *= earthGravityNormalizer;

		Vec3 cameraPos;
		pPlayerCamera->GetLocation(cameraPos);

		Terrain* pTerrain = TerrainSystem::GetCurrentTerrain();
		float groundHeightAtPos = pTerrain->ComputeBarycentricHeight(pPlayerCamera->GetLocation());

		if (cameraPos[y] > 4.f + groundHeightAtPos)
		{
			cameraMove[y] -= gravityStrength;
		}
		else
		{
			jumping = false;
		}

		pPlayerCamera->AddLocationOffset(cameraMove * deltaTime);
	}

	void PlayerScriptFps::CheckLookAt(float deltaTime)
	{
		static_cast<void>(deltaTime);
		Engine::SetMouseVisibility(false);

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
			constexpr float mouseLookSpeed = 1.25f;
			pPlayerCamera->AddLookAtOffset(Vec3(offsetHorizontal, -offsetVertical, 0.f) * mouseLookSpeed);

			prevPoint = p;
			SetCursorPos(Engine::GetWindowWidth() / 2, Engine::GetWindowHeight() / 2);
			prevPoint.x = (LONG)(Engine::GetWindowWidth() / 2) - (LONG)offsetHorizontal;
			prevPoint.y = (LONG)(Engine::GetWindowHeight() / 2) - (LONG)offsetVertical;
		}
	}
}