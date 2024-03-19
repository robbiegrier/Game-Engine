#include "GONull.h"
#include "ShaderObjectManager.h"

namespace Azul
{
	GONull::GONull()
		: GraphicsObject(nullptr, ShaderObjectManager::Find(ShaderObject::Name::Null))
	{
		name = Name::GraphicsObjectNull;
	}

	void GONull::ActivateGraphics()
	{
	}

	void GONull::Draw()
	{
	}

	GraphicsObject* GONull::Clone()
	{
		return new GONull();
	}
}