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

		// Core update and draw
		static void Update(AnimTime deltaTime);
		static void Draw();

		// Spawn factory methods
		static GameObject* SpawnObject(const char* const pName, Mesh::Name model, ShaderObject::Name shader, const Vec3& location, GameObject* pParent = nullptr);
		static GameObject* SpawnObject(const char* const pName, Mesh* pModel, ShaderObject* pShader, const Vec3& location, GameObject* pParent = nullptr);
		static GameObject* SpawnObject(const char* const pName, Mesh* pModel, ShaderObject* pShader, TextureObject* pTexture, const Vec3& location, GameObject* pParent = nullptr);
		static GameObject* SpawnObject(const char* const pName, GraphicsObject* pGraphicsObject, const Vec3& location, GameObject* pParent = nullptr);
		static GameObject* SpawnObject(const char* const pName, GameObject* pGameObject, const Vec3& location, GameObject* pParent = nullptr);

		static GameObject* SpawnObject(const char* const pName, Mesh::Name model, TextureObject::Name texture, const Vec3& location, GameObject* pParent = nullptr);

		static PCSTree& GetAllObjects() { return GetInstance().objects; }

		static GameObject* FindObject(const char* const pName);

	private:
		// Singleton
		static GameObjectManager& GetInstance();

		// Object Heirarchy
		PCSTree objects;
	};
}

#endif