#ifndef _GameObjectAnimSkin
#define _GameObjectAnimSkin

#include "GameObject.h"
#include "MathEngine.h"

namespace Azul
{
	class AnimController;

	class GameObjectAnimSkin : public GameObject
	{
	public:
		// Big four
		GameObjectAnimSkin(GraphicsObject* pGraphicsObject, AnimController* pInController);
		GameObjectAnimSkin() = delete;
		GameObjectAnimSkin(const GameObjectAnimSkin&) = delete;
		GameObjectAnimSkin& operator=(GameObjectAnimSkin&) = delete;
		virtual ~GameObjectAnimSkin();

		virtual void Update(AnimTime currTime);

	public:
		Mat4* poBoneWorld;
		AnimController* pController;

	private:
		void UpdateSkinGPU();
	};
}

#endif
