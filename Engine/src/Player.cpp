#include "Player.h"
#include "GONull.h"
#include "CameraManager.h"
#include "Engine.h"
#include <math.h>

namespace Azul
{
	Player::Player()
		: GameObject(new GONull())
	{
		typeName = Name::Player;
		alwaysRenderShell = true;

		SetShellColor(Vec4(0.f, .8f, .8f, 1.f));
	}

	Player::~Player()
	{
	}

	void Player::Start()
	{
		CameraManager::SetCurrentCamera(Camera::Name::Player);
		pPlayerCamera = CameraManager::GetCurrentCamera();
		pPlayerCamera->SetOrientAndPosition(Vec3(0, 1, 0), GetWorldLocation() + Vec3(GetWorld().get(Row4::i2)).getNorm(), GetWorldLocation());
		Trace::out("\tPlayer: Start!\n");
	}

	void Player::Tick(float deltaTime)
	{
		float moveScale = deltaTime;// toSeconds(deltaTime);
		CheckMovement(moveScale);
		CheckLookAt(moveScale);
		ProcessGravity(moveScale);
	}

	void Player::CheckMovement(float deltaTime)
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
		}

		AddRelativeLocationOffset(cameraJump * deltaTime);
		AddRelativeLocationOffset(cameraMove * deltaTime);

		pPlayerCamera->AddLocationOffset(cameraJump * deltaTime);
		pPlayerCamera->AddHorizontalLocationOffset(cameraMove * deltaTime);
	}

	void Player::ProcessGravity(float deltaTime)
	{
		Vec3 cameraMove;
		float gravityStrength = 1.f;

		constexpr float earthGravityNormalizer = 10.f;
		gravityStrength *= earthGravityNormalizer;

		Vec3 cameraPos;
		pPlayerCamera->GetLocation(cameraPos);

		if (cameraPos[y] > 3.5f)
		{
			cameraMove[y] -= gravityStrength;
		}
		else
		{
			jumping = false;
		}

		pPlayerCamera->AddLocationOffset(cameraMove * deltaTime);
	}

	void Player::CheckLookAt(float deltaTime)
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