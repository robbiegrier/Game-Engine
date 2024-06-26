#include "EngineStatePlay.h"
#include "Engine.h"
#include "EditorGui.h"
#include "CameraManager.h"
#include "GameObjectManager.h"
#include "AnimationSystem.h"

namespace Azul
{
	void EngineStatePlay::Update(float deltaTime)
	{
		Engine::NativeUpdate(deltaTime);
		CameraManager::Update(deltaTime);
		GameObjectManager::Tick(deltaTime);

		if (GetKeyState(VK_F1) & 0x8000)
		{
			Engine::SetEditorMode(true);
		}
	}

	void EngineStatePlay::Draw()
	{
		CameraManager::GetCurrentCamera()->SetAspectRatio((float)Engine::GetWindowWidth() / (float)Engine::GetWindowHeight());
		CameraManager::GetCurrentCamera2D()->SetViewport(0, 0, Engine::GetWindowWidth(), Engine::GetWindowHeight());
		CameraManager::GetCurrentCamera2D()->SetOrthographic(
			(float)-CameraManager::GetCurrentCamera2D()->GetScreenWidth() / 2.0f, (float)CameraManager::GetCurrentCamera2D()->GetScreenWidth() / 2.0f,
			(float)-CameraManager::GetCurrentCamera2D()->GetScreenHeight() / 2.0f, (float)CameraManager::GetCurrentCamera2D()->GetScreenHeight() / 2.0f,
			1.0f, 1000.0f
		);
		Engine::NativeRender();
	}

	void EngineStatePlay::Enter()
	{
		Trace::out("On Game Start\n");
		Engine::SetMouseVisibility(false);
		CameraManager::SetCurrentCamera(Camera::Name::Player);
		GameObjectManager::Start();
	}

	void EngineStatePlay::Exit()
	{
		Trace::out("On Game End\n");
	}

	void EngineStatePlay::TransitionToEditor()
	{
		Trace::out("Returning to Editor!\n");
	}
}