#include "Game.h"
#include "MathEngine.h"
#include "GameObjectBasic.h"
#include "GraphicsObject.h"
#include "Mesh.h"

namespace Azul
{
	GameObjectBasic::GameObjectBasic(GraphicsObject* pGraphicsObject)
		: GameObjectRigid(pGraphicsObject)
	{
	}

	GameObjectBasic::~GameObjectBasic()
	{
	}
}