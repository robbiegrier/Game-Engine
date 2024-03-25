#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnim.h"
#include "GraphicsObject.h"

namespace Azul
{
	GameObjectAnim::GameObjectAnim(GraphicsObject* pGraphicsObject)
		: GameObject(pGraphicsObject), index(-1)
	{
		SetIsSelectable(false);
	}

	void GameObjectAnim::Update(AnimTime currentTime)
	{
	}
}