#ifndef _GameObjectRigid
#define _GameObjectRigid

#include "GameObject.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"

namespace Azul
{
	class GameObjectRigid : public GameObject
	{
	public:
		// Big four
		GameObjectRigid(GraphicsObject* pGraphicsObject);
		GameObjectRigid() = delete;
		GameObjectRigid(const GameObjectRigid&) = delete;
		GameObjectRigid& operator=(GameObjectRigid&) = delete;
		virtual ~GameObjectRigid() = default;

		virtual void Update(AnimTime currTime);
	};
}

#endif

// --- End of File ---
