#include "EngineStateEditor.h"
#include "EditorGui.h"
#include "EditorInput.h"
#include "Engine.h"
#include "CameraManager.h"
#include "SceneManager.h"

namespace Azul
{
	void EngineStateEditor::Update(float deltaTime)
	{
		EditorGui::NewFrame();
		EditorInput::Update();
		EditorGui::Controller().Update();
		Engine::NativeUpdate(0.0f);
		//Engine::NativeUpdate(deltaTime);
		CameraManager::Update(deltaTime);

		if (GetKeyState(VK_F2) & 0x8000)
		{
			Engine::SetEditorMode(false);
		}
	}

	void EngineStateEditor::Draw()
	{
		EditorGui::Draw();
	}

	void EngineStateEditor::Enter()
	{
		CameraManager::SetCurrentCamera(Camera::Name::Default);
		Engine::SetMouseVisibility(true);

		SceneManager::ChangeScene("AzulScene");
	}

	void EngineStateEditor::Exit()
	{
	}

	void EngineStateEditor::TransitionToPlay()
	{
		Trace::out("Started Play Mode!\n");
	}
}