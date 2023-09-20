#ifndef _ManagerBase
#define _ManagerBase

#include "ListBase.h"
#include "Iterator.h"

namespace Azul
{
	class ManagerBase
	{
	protected:
		ManagerBase(ListBase* pActiveNodes, ListBase* pReserveNodes, int initialNumNodesReserved = 5, int reserveGrowthScale = 2);
		virtual ~ManagerBase();

		Iterator* GetActiveIterator();
		Iterator* GetReserveIterator();

		DLink* Find(DLink* pNodeTarget);
		void Remove(DLink* pNodeBase);
		DLink* Push();
		DLink* Add();

		void Dump();

		virtual DLink* CreateNode() = 0;

	private:
		DLink* NativeCreateNode();
		void GrowReserve();
		void GrowReserve(int count);
		void AddToReserve(DLink* pNode);
		DLink* PopReserveNode();
		void AddActiveNode(DLink* pNode);
		void PushActiveNode(DLink* pNode);

		ListBase* pActiveNodes;
		ListBase* pReserveNodes;
		int reserveGrowthScale;
		int totalNodeCount;
		int reserveNodeCount;
		int activeNodeCount;
	};
}

#endif