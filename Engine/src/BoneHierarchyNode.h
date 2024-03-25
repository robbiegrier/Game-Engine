#ifndef _BoneHierarchyNode
#define _BoneHierarchyNode

namespace Azul
{
	class BoneHierarchyNode
	{
	public:
		int  index;
		int  parentIndex;
		char name[32];
	};
}

#endif