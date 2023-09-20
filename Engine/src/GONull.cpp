#include "GONull.h"
#include "ShaderObjectManager.h"

namespace Azul
{
	GONull::GONull()
		: GraphicsObject(nullptr, ShaderObjectManager::Find(ShaderObject::Name::Null))
	{
	}

	void GONull::ActivateGraphics()
	{
	}

	void GONull::Draw()
	{
	}
}