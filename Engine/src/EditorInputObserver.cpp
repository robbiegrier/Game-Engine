#include "EditorInputObserver.h"

namespace Azul
{
	void EditorInputObserver::Wash()
	{
	}

	bool EditorInputObserver::Compare(DLink* pTargetNode)
	{
		return this == pTargetNode;
	}
}