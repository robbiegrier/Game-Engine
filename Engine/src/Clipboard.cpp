#include "Clipboard.h"
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
#include "GOSprite.h"
#include "Player.h"
#include "GameObjectSprite.h"
#include "EditorGui.h"
#include "EditorObjectReference.h"
#include "CameraManager.h"
#include "EditActionSpawn.h"
#include "EditActionDelete.h"
#include "PCSTreeReverseIterator.h"
#include "Image.h"
#include "ImageManager.h"

using json = nlohmann::json;

namespace Azul
{
	json Clipboard::SceneToJson(GameObject* pRootObject)
	{
		json scene;

		for (PCSTreeForwardIterator it(pRootObject); !it.IsDone(); it.Next())
		{
			GameObject* pObject = (GameObject*)it.Current();

			if (pObject->GetParentGameObject())
			{
				json objectJson = ObjectToJson(pObject);
				scene.push_back(objectJson);
			}
		}

		return scene;
	}

	json Clipboard::ObjectToJson(GameObject* pObject)
	{
		if (pObject->GetParentGameObject())
		{
			char pName[GameObject::NAME_SIZE];
			pObject->GetName(pName, GameObject::NAME_SIZE);

			char pParentName[GameObject::NAME_SIZE];
			pObject->GetParentGameObject()->GetName(pParentName, GameObject::NAME_SIZE);

			const Vec3& location = pObject->GetRelativeLocation();
			const Quat& rotation = pObject->GetRelativeRotation();
			const Vec3& scale = pObject->GetRelativeScale();

			ShaderObject::Name shaderEnum = ShaderObject::Name::None;
			Mesh::Name meshEnum = Mesh::Name::None;
			TextureObject::Name textureEnum = TextureObject::Name::None;
			Image::Name imageEnum = Image::Name::Null;

			if (pObject->GetGraphicsObject()->GetShader())	shaderEnum = pObject->GetGraphicsObject()->GetShader()->GetName();
			if (pObject->GetGraphicsObject()->GetModel())	meshEnum = pObject->GetGraphicsObject()->GetModel()->GetName();
			if (pObject->GetGraphicsObject()->GetTexture()) textureEnum = pObject->GetGraphicsObject()->GetTexture()->GetName();
			Vec4 color = pObject->GetGraphicsObject()->GetColor();
			GraphicsObject::Name graphicObjectEnum = pObject->GetGraphicsObject()->GetName();
			GameObject::Name gameObjectEnum = pObject->GetTypeName();

			if (pObject->GetTypeName() == GameObject::Name::GameObjectSprite)
			{
				imageEnum = ((GOSprite*)pObject->GetGraphicsObject())->pImage->GetName();
			}

			return json({
					{"Object Name", pName},
					{"Parent Name", pParentName},
					{"Location", {location[x], location[y], location[z]}},
					{"Rotation", {rotation[x], rotation[y], rotation[z], rotation[w]}},
					{"Scale", {scale[x], scale[y], scale[z]}},
					{"Shader", shaderEnum},
					{"Mesh", meshEnum},
					{"Texture", textureEnum},
					{"Image", imageEnum},
					{"Graphics Object", graphicObjectEnum},
					{"Game Object", gameObjectEnum},
					{"Color", {color[x], color[y], color[z], color[w]}},
					{"Render Shell", pObject->GetRenderShell()},
					{"Is Selectable", pObject->IsSelectable()},
				});
		}

		return json();
	}

	GameObject* Clipboard::GetParentFromJson(const json& item)
	{
		for (const auto& prop : item.items())
		{
			const char* propertyName = prop.key().c_str();
			if (strcmp(propertyName, "Parent Name") == 0)
			{
				return GameObjectManager::FindObject(prop.value().template get<std::string>().c_str());
			}
		}

		return nullptr;
	}

	GameObject* Clipboard::JsonToObject(const json& item)
	{
		char objectName[GameObject::NAME_SIZE];
		GraphicsObject::Name goName = GraphicsObject::Name::None;
		GameObject::Name typeName = GameObject::Name::None;
		Mesh::Name meshName = Mesh::Name::None;
		TextureObject::Name textureName = TextureObject::Name::None;
		ShaderObject::Name shaderName = ShaderObject::Name::None;
		Image::Name imageName = Image::Name::Null;
		Vec4 color;
		Vec3 location;
		Vec3 scale;
		Quat rotation;
		bool renderShell = true;
		bool isSelectable = true;

		for (const auto& prop : item.items())
		{
			const char* propertyName = prop.key().c_str();

			if (strcmp(propertyName, "Object Name") == 0)			memcpy_s(objectName, GameObject::NAME_SIZE, prop.value().template get<std::string>().c_str(), GameObject::NAME_SIZE);
			else if (strcmp(propertyName, "Parent Name") == 0) {}
			else if (strcmp(propertyName, "Graphics Object") == 0)	goName = (GraphicsObject::Name)prop.value().template get<unsigned int>();
			else if (strcmp(propertyName, "Game Object") == 0)		typeName = (GameObject::Name)prop.value().template get<unsigned int>();
			else if (strcmp(propertyName, "Shader") == 0)			shaderName = (ShaderObject::Name)prop.value().template get<unsigned int>();
			else if (strcmp(propertyName, "Texture") == 0)			textureName = (TextureObject::Name)prop.value().template get<unsigned int>();
			else if (strcmp(propertyName, "Mesh") == 0)				meshName = (Mesh::Name)prop.value().template get<unsigned int>();
			else if (strcmp(propertyName, "Image") == 0)			imageName = (Image::Name)prop.value().template get<unsigned int>();
			else if (strcmp(propertyName, "Render Shell") == 0)		renderShell = prop.value().template get<bool>();
			else if (strcmp(propertyName, "Is Selectable") == 0)	isSelectable = prop.value().template get<bool>();
			else if (strcmp(propertyName, "Copy Relative Location") == 0) {}
			else
			{
				const auto& list = prop.value().template get<std::vector<float>>();

				if (strcmp(propertyName, "Color") == 0)				color.set(list[0], list[1], list[2], list[3]);
				else if (strcmp(propertyName, "Location") == 0)		location.set(list[0], list[1], list[2]);
				else if (strcmp(propertyName, "Scale") == 0)		scale.set(list[0], list[1], list[2]);
				else if (strcmp(propertyName, "Rotation") == 0)		rotation.set(list[0], list[1], list[2], list[3]);
			}
		}

		GraphicsObject* pGo = nullptr;

		switch (goName)
		{
		case GraphicsObject::Name::GraphicsObjectColorByVertex:
			pGo = new GOColorByVertex(MeshManager::Find(meshName), ShaderObjectManager::Find(shaderName)); break;
		case GraphicsObject::Name::GraphicsObjectConstColor:
			pGo = new GOConstColor(MeshManager::Find(meshName), ShaderObjectManager::Find(shaderName), Vec3(color)); break;
		case GraphicsObject::Name::GraphicsObjectFlatTexture:
			pGo = new GOFlatTexture(MeshManager::Find(meshName), ShaderObjectManager::Find(shaderName), TextureObjectManager::Find(textureName)); break;
		case GraphicsObject::Name::GraphicsObjectLightTexture:
			pGo = new GOLightTexture(MeshManager::Find(meshName), ShaderObjectManager::Find(shaderName), TextureObjectManager::Find(textureName)); break;
		case GraphicsObject::Name::GraphicsObjectSprite:
			pGo = new GOSprite(MeshManager::Find(meshName), ShaderObjectManager::Find(shaderName), ImageManager::Find(imageName), Rect()); break;
		case GraphicsObject::Name::GraphicsObjectNull:
			pGo = new GONull(); break;
		}

		GameObject* pGameObject = nullptr;

		switch (typeName)
		{
		case GameObject::Name::GameObject:
			pGameObject = new GameObject(pGo);
			break;
		case GameObject::Name::Player:
			delete pGo;
			pGameObject = new Player();
			break;
		case GameObject::Name::GameObjectSprite:
			pGameObject = new GameObjectSprite(pGo);
			break;
		}

		pGameObject->SetRelativeScale(scale);
		pGameObject->SetRelativeLocation(location);
		pGameObject->SetRelativeRotation(rotation);
		pGameObject->SetRenderShell(renderShell);
		pGameObject->SetIsSelectable(isSelectable);
		pGameObject->SetName(objectName);

		return pGameObject;
	}

	void Clipboard::SpawnObjectsFromJson(const json& scene, const Vec3& inLocation)
	{
		for (const auto& objectMapping : scene.items())
		{
			GameObject* pGameObject = JsonToObject(objectMapping.value());
			GameObject* pGameObjectParent = GetParentFromJson(objectMapping.value());

			EditActionSpawn* pSpawnAction = new EditActionSpawn();
			pSpawnAction->SetSingleObject(pGameObject);
			pSpawnAction->SetTargetParent(pGameObjectParent);

			const bool parentIsRoot = pGameObjectParent == GameObjectManager::GetAllObjects().GetRoot();
			bool copyRelative = false;
			Vec3 spawnLocation = inLocation;

			for (const auto& prop : objectMapping.value().items())
			{
				const char* propertyName = prop.key().c_str();
				if (strcmp(propertyName, "Copy Relative Location") == 0)
				{
					copyRelative = prop.value().template get<bool>();
				}
			}

			if (!parentIsRoot)
			{
				spawnLocation = pGameObject->GetRelativeLocation();
			}

			if (copyRelative)
			{
				Mat4 posMat = Trans(inLocation) * Trans(pGameObject->GetRelativeLocation());
				spawnLocation = Vec3(posMat.get(Row4::i3));
			}

			pSpawnAction->SetLocation(spawnLocation);
			pSpawnAction->Execute();
			pSpawnAction->Commit();

			pGameObject->SetWorldLocation(spawnLocation);
		}
	}

	void Clipboard::Copy(const Selection& selection)
	{
		content.clear();

		for (Iterator& it = *selection.GetSelectedObjectReferences().GetIterator(); !it.IsDone(); it.Next())
		{
			GameObject* pCurr = (GameObject*)((EditorObjectReference*)it.Curr())->GetGameObject();

			json subtreeJson = SceneToJson(pCurr);

			// Preserve relative position to focus object
			if (pCurr->GetParentGameObject() == GameObjectManager::GetAllObjects().GetRoot() && pCurr != selection.GetFocusObject()->GetGameObject())
			{
				GameObject* pFocus = selection.GetFocusObject()->GetGameObject();
				Vec3 newRelativeLocation = Vec3(Vec4(pCurr->GetWorldLocation(), 1.f) * Trans(pFocus->GetWorldLocation()).getInv());

				json& rootJson = subtreeJson[0];
				rootJson["Location"] = { newRelativeLocation[x], newRelativeLocation[y], newRelativeLocation[z] };
				rootJson["Copy Relative Location"] = true;
			}

			for (const auto& objectJson : subtreeJson)
			{
				content += objectJson;
			}
		}

		std::string s = content.dump(2);
	}

	void Clipboard::Paste(const Vec3& location)
	{
		MakeContentUnique();
		SpawnObjectsFromJson(content, location);
	}

	void Clipboard::MakeContentUnique()
	{
		for (auto& objectJson : content)
		{
			std::string defaultName = objectJson["Object Name"].template get<std::string>();
			std::string str = GenerateUniqueObjectName(defaultName.c_str());

			if (str != defaultName)
			{
				objectJson["Object Name"] = str;

				for (auto& otherObjectJson : content)
				{
					std::string defaultParentName = otherObjectJson["Parent Name"].template get<std::string>();

					if (defaultParentName == defaultName)
					{
						otherObjectJson["Parent Name"] = str;
					}
				}
			}
		}
	}

	std::string Clipboard::GenerateUniqueObjectName(const char* pInName)
	{
		std::string output = pInName;

		bool uniqueReached = false;
		bool renamedOnce = false;

		if (*(output.end() - 3) == '(' && *(output.end() - 1) == ')')
		{
			renamedOnce = true;
		}

		int iterations = 1;

		while (!uniqueReached)
		{
			uniqueReached = true;

			for (PCSTreeReverseIterator it(GameObjectManager::GetAllObjects().GetRoot()); !it.IsDone(); it.Next())
			{
				GameObject* pCurrObject = (GameObject*)it.Current();
				const char* pCurrName = pCurrObject->GetNamePtr();

				if (strcmp(pCurrName, output.c_str()) == 0)
				{
					uniqueReached = false;

					if (!renamedOnce)
					{
						output += std::string(" (" + std::to_string(iterations) + ")");
						renamedOnce = true;
					}
					else
					{
						output = output.replace(output.end() - 4, output.end(), std::string(" (" + std::to_string(iterations) + ")"));
					}

					iterations++;
				}
			}

			for (const auto& otherObjectJson : content)
			{
				std::string otherObjectJsonName = otherObjectJson["Object Name"].template get<std::string>();

				if (strcmp(otherObjectJsonName.c_str(), output.c_str()) == 0)
				{
					uniqueReached = false;

					if (!renamedOnce)
					{
						output += std::string(" (" + std::to_string(iterations) + ")");
						renamedOnce = true;
					}
					else
					{
						output = output.replace(output.end() - 4, output.end(), std::string(" (" + std::to_string(iterations) + ")"));
					}

					iterations++;
				}
			}
		}

		return output;
	}
}