#include "SceneManager.h"
#include "json.hpp"
#include "TextureObjectManager.h"
#include "GameObjectManager.h"
#include "PCSTreeForwardIterator.h"
#include "ShaderObjectManager.h"
#include "MeshManager.h"
#include "GameObject.h"
#include "File.h"
#include "EditorHistory.h"
#include "GOColorByVertex.h"
#include "GOConstColor.h"
#include "GOFlatTexture.h"
#include "GOLightTexture.h"
#include "GONull.h"
#include "Player.h"
#include "EditorGui.h"
#include "Clipboard.h"

using json = nlohmann::json;

namespace Azul
{
	void SceneManager::SaveScene(const char* pSceneName)
	{
		Trace::out("Saving Scene: %s\n", pSceneName);

		const std::string pFileName = GetFilePathName(pSceneName);
		std::remove(pFileName.c_str());

		File::Handle fh;
		File::Error err;

		err = File::Open(fh, pFileName.c_str(), File::Mode::WRITE); assert(err == File::Error::SUCCESS);

		const json scene = Clipboard::SceneToJson((GameObject*)GameObjectManager::GetAllObjects().GetRoot());
		const std::string content = scene.dump(2);

		err = File::Write(fh, content.c_str(), content.length());

		if (err != File::Error::SUCCESS)
		{
			Trace::out("Failed to write to scene file. Did you checkout %s on Perforce?\n", pFileName.c_str());
		}
		else
		{
			Trace::out("Saved %s\n", pSceneName);
		}

		err = File::Close(fh); assert(err == File::Error::SUCCESS);
	}

	void SceneManager::ChangeScene(const char* pSceneName)
	{
		const std::string pFileName = GetFilePathName(pSceneName);
		Trace::out("Loading Scene: %s\n", pSceneName);

		GameObjectManager::ClearObjects();
		EditorHistory::ClearAllHistory();

		File::Handle fh;
		File::Error err;
		DWORD FileSize(0);

		err = File::Open(fh, pFileName.c_str(), File::Mode::READ); assert(err == File::Error::SUCCESS);
		err = File::Seek(fh, File::Position::END, 0); assert(err == File::Error::SUCCESS);
		err = File::Tell(fh, FileSize); assert(err == File::Error::SUCCESS);

		const unsigned int BuffSize = (unsigned int)FileSize + 1;
		char* pBuff = new char[BuffSize]();

		err = File::Seek(fh, File::Position::BEGIN, 0); assert(err == File::Error::SUCCESS);
		err = File::Read(fh, pBuff, BuffSize); assert(err == File::Error::SUCCESS);
		err = File::Close(fh); assert(err == File::Error::SUCCESS);

		const json scene = json::parse(pBuff);
		delete pBuff;

		for (const auto& objectMapping : scene.items())
		{
			GameObject* pGameObject = Clipboard::JsonToObject(objectMapping.value());
			GameObject* pGameObjectParent = Clipboard::GetParentFromJson(objectMapping.value());

			char protoName[GameObject::NAME_SIZE];
			pGameObject->GetName(protoName, GameObject::NAME_SIZE);

			GameObjectManager::SpawnObject(protoName, pGameObject, pGameObject->GetRelativeLocation(), pGameObjectParent);
		}

		Trace::out("Finished Loading Scene: %s\n", pSceneName);
	}

	std::string SceneManager::GetFilePathName(const char* pSceneName)
	{
		return std::string("..\\Scenes\\") + std::string(pSceneName) + std::string(".json");
	}

	SceneManager& SceneManager::GetInstance()
	{
		static SceneManager instance;
		return instance;
	}
}