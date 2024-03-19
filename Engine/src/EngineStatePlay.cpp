#include "EngineStatePlay.h"
#include "Engine.h"
#include "EditorGui.h"
#include "CameraManager.h"
#include "GameObjectManager.h"

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