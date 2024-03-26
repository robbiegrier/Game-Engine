#include "Component.h"

namespace Azul
{
	void Component::Wash()
	{
	}

	bool Component::Compare(DLink* pTargetNode)
	{
		return this == (Component*)pTargetNode;
	}
}