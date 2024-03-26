#ifndef _GameObjectManager
#define _GameObjectManager

#include "PCSTree.h"
#include "MathEngine.h"
#include "Mesh.h"
#include "ShaderObject.h"
#include "TextureObject.h"
#include "AnimTime.h"

namespace Azul
{
	class GameObject;
	class GraphicsObject;

	class GameObjectManager
	{
	public:
		// Manager
		static void Create();
		static void Destroy();
		static void Dump();

		// Scene Events
		static void Start();
		static void Tick(float deltaTime);

		// Frame Events
		static void Update(float deltaTime);
		static void Draw();

		// Spawn factory methods
		static GameObject* SpawnObject(const char* const pName, Mesh::Name model, ShaderObject::Name shader, const Vec3& location, GameObject* pParent = nullptr);
		static GameObject* SpawnObject(const char* const pName, Mesh* pModel, ShaderObject* pShader, const Vec3& location, GameObject* pParent = nullptr);
		static GameObject* SpawnObject(const char* const pName, Mesh* pModel, ShaderObject* pShader, TextureObject* pTexture, const Vec3& location, GameObject* pParent = nullptr);
		static GameObject* SpawnObject(const char* const pName, GraphicsObject* pGraphicsObject, const Vec3& location, GameObject* pParent = nullptr);
		static GameObject* SpawnObject(const char* const pName, GameObject* pGameObject, const Vec3& location, GameObject* pParent = nullptr);

		static GameObject* SpawnObject(const char* const pName, Mesh::Name model, TextureObject::Name texture, const Vec3& location, GameObject* pParent = nullptr);

		static GameObject* ReparentObject(GameObject* pObject, GameObject* pParent, GameObject* pPrev, bool preserveWorld = true);

		static PCSTree& GetAllObjects() { return GetInstance().objects; }

		static GameObject* FindObject(const char* const pName);

		// todo: destroy children
		static void DestroyObject(const char* const pName);

		static GameObject* DespawnObject(const char* const pName);
		static GameObject* DespawnObject(GameObject* pObject);

		static void ClearObjects();

	private:
		// Singleton
		static GameObjectManager& GetInstance();

		// Object Heirarchy
		PCSTree objects;
	};
}

#endif