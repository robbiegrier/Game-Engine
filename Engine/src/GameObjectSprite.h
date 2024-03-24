#ifndef _GameObjectSprite
#define _GameObjectSprite

#include "GameObject.h"

namespace Azul
{
	class GameObjectSprite : public GameObject
	{
	public:
		// Big four
		GameObjectSprite(GraphicsObject* graphicsObject);
		GameObjectSprite() = delete;
		GameObjectSprite(const GameObjectSprite&) = delete;
		GameObjectSprite& operator=(GameObjectSprite&) = delete;
		virtual ~GameObjectSprite();
	};
}

#endif
