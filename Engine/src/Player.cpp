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
	}

	void Player::Tick(float deltaTime)
	{
		CheckSwitchCameraInput();
		CheckMovement(deltaTime);
		CheckLookAt(deltaTime);
		ToggleLookAtMode();
		ToggleFramerateMode();

		Vec3 loc;
		CameraManager::GetCurrentCamera()->GetLocation(loc);
		SetRelativeLocation(loc);

		if (walkMode)
		{
			ProcessGravity(deltaTime);
		}
	}

	void Player::CheckSwitchCameraInput()
	{
		if (GetKeyState('Y') & 0x8000)
		{
			CameraManager::SetCurrentCamera(Camera::Name::Default);
		}
		else if (GetKeyState('U') & 0x8000)
		{
			CameraManager::SetCurrentCamera(Camera::Name::High);
		}
		else if (GetKeyState('I') & 0x8000)
		{
			CameraManager::SetCurrentCamera(Camera::Name::Low);
		}
		else if (GetKeyState('O') & 0x8000)
		{
			CameraManager::SetCurrentCamera(Camera::Name::Aux);
		}
	}

	void Player::CheckMovement(float deltaTime)
	{
		Vec3 cameraMove;
		float moveSpeed = 10.f;
		float sprintSpeed = 1.5f;
		float jumpStrength = 20.f;

		if (walkMode && GetKeyState(VK_SHIFT) & 0x8000)
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

		if (!walkMode)
		{
			if (GetKeyState(VK_SPACE) & 0x8000)
			{
				cameraMove[y] += moveSpeed;
			}
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				cameraMove[y] -= moveSpeed;
			}

			CameraManager::GetCurrentCamera()->AddLocationOffset(cameraMove * deltaTime);
		}
		else
		{
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

			CameraManager::GetCurrentCamera()->AddLocationOffset(cameraJump * deltaTime);
			CameraManager::GetCurrentCamera()->AddHorizontalLocationOffset(cameraMove * deltaTime);
		}
	}

	void Player::ProcessGravity(float deltaTime)
	{
		Vec3 cameraMove;
		float gravityStrength = 1.f;

		constexpr float earthGravityNormalizer = 10.f;
		gravityStrength *= earthGravityNormalizer;

		Vec3 cameraPos;
		CameraManager::GetCurrentCamera()->GetLocation(cameraPos);
		if (cameraPos[y] > 1.0f)
		{
			cameraMove[y] -= gravityStrength;
		}
		else
		{
			jumping = false;
		}

		CameraManager::GetCurrentCamera()->AddLocationOffset(cameraMove * deltaTime);
	}

	void Player::CheckLookAt(float deltaTime)
	{
		if (!useMouseLook)
		{
			Vec3 cameraLookAt;
			float lookSpeed = 3.5f;

			if (GetKeyState(VK_UP) & 0x8000)
			{
				cameraLookAt[y] += lookSpeed;
			}
			if (GetKeyState(VK_DOWN) & 0x8000)
			{
				cameraLookAt[y] -= lookSpeed;
			}
			if (GetKeyState(VK_LEFT) & 0x8000)
			{
				cameraLookAt[x] -= lookSpeed;
			}
			if (GetKeyState(VK_RIGHT) & 0x8000)
			{
				cameraLookAt[x] += lookSpeed;
			}

			CameraManager::GetCurrentCamera()->AddLookAtOffset(cameraLookAt * deltaTime);

			GetCursorPos(&prevPoint);
		}
		else
		{
			POINT p;
			GetCursorPos(&p);
			HWND hwnd{ 0 };
			ScreenToClient(hwnd, &p);
			{
				float offsetHorizontal = (float)p.x - (float)prevPoint.x;
				float offsetVertical = (float)p.y - (float)prevPoint.y;

				offsetHorizontal /= 1000.f;
				offsetVertical /= 1000.f;

				if (fabs(offsetHorizontal) > 0.005f || fabs(offsetVertical) > 0.005f)
				{
					constexpr float mouseLookSpeed = 1.25f;
					CameraManager::GetCurrentCamera()->AddLookAtOffset(Vec3(offsetHorizontal, -offsetVertical, 0.f) * mouseLookSpeed);

					prevPoint = p;
					SetCursorPos(Engine::GetWindowWidth() / 2, Engine::GetWindowHeight() / 2);
					prevPoint.x = (LONG)(Engine::GetWindowWidth() / 2) - (LONG)offsetHorizontal;
					prevPoint.y = (LONG)(Engine::GetWindowHeight() / 2) - (LONG)offsetVertical;
				}
			}
		}
	}

	void Player::ToggleLookAtMode()
	{
		if (GetKeyState('H') & 0x8000)
		{
			useMouseLook = true;
			walkMode = true;

			Engine::SetMouseVisibility(false);
		}
		else if (GetKeyState('G') & 0x8000)
		{
			useMouseLook = false;
			walkMode = false;

			Engine::SetMouseVisibility(true);
		}
	}

	void Player::ToggleFramerateMode()
	{
		if (GetKeyState('K') & 0x8000)
		{
			Engine::ToggleMaxFramerate(true);
		}
		else if (GetKeyState('L') & 0x8000)
		{
			Engine::ToggleMaxFramerate(false);
		}
	}
}