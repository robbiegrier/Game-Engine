#include "GameObjectSprite.h"
#include "GOSprite.h"

namespace Azul
{
	GameObjectSprite::GameObjectSprite(GraphicsObject* pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject);

		GOSprite* pGraphicsObjectSprite = (GOSprite*)pGraphicsObject;

		SetRelativeScale(Vec3(pGraphicsObjectSprite->origWidth, pGraphicsObjectSprite->origHeight, 1.f));
		SetRelativeLocation(Vec3(pGraphicsObjectSprite->origPosX, pGraphicsObjectSprite->origPosY, 0.f));
	}

	GameObjectSprite::~GameObjectSprite()
	{
	}
}