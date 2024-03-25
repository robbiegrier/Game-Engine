#ifndef _GameObjectBasic
#define _GameObjectBasic

#include "GameObjectRigid.h"
#include "MathEngine.h"

namespace Azul
{
	class GraphicsObject;

	class GameObjectBasic : public GameObjectRigid
	{
	public:
		// Big four
		GameObjectBasic(GraphicsObject* graphicsObject);
		GameObjectBasic() = delete;
		GameObjectBasic(const GameObjectBasic&) = delete;
		GameObjectBasic& operator=(GameObjectBasic&) = delete;
		virtual ~GameObjectBasic();
	};
}

#endif
