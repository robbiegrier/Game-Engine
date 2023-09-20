#include "ManagerBase.h"

namespace Azul
{
	ManagerBase::ManagerBase(ListBase* pInActiveNodes, ListBase* pInReserveNodes, int inInitialNumNodesReserved, int inReserveGrowthScale)
		: pActiveNodes(pInActiveNodes), pReserveNodes(pInReserveNodes), reserveGrowthScale(inReserveGrowthScale), totalNodeCount(0), reserveNodeCount(0), activeNodeCount(0)
	{
		assert(pActiveNodes != nullptr);
		assert(pReserveNodes != nullptr);
		assert(inInitialNumNodesReserved >= 0);
		assert(inReserveGrowthScale > 0);

		GrowReserve(inInitialNumNodesReserved);
	}

	ManagerBase::~ManagerBase()
	{
		delete pActiveNodes;
		delete pReserveNodes;
	}

	DLink* ManagerBase::Push()
	{
		DLink* pNode = PopReserveNode();
		PushActiveNode(pNode);
		return pNode;
	}

	DLink* ManagerBase::Add()
	{
		DLink* pNode = PopReserveNode();
		AddActiveNode(pNode);
		return pNode;
	}

	Iterator* ManagerBase::GetActiveIterator()
	{
		return pActiveNodes->GetIterator();
	}

	Iterator* ManagerBase::GetReserveIterator()
	{
		return pReserveNodes->GetIterator();
	}

	DLink* ManagerBase::Find(DLink* pNodeTarget)
	{
		DLink* pNode = nullptr;

		Iterator* pIt = pActiveNodes->GetIterator();

		for (pIt->First(); !pIt->IsDone(); pIt->Next())
		{
			pNode = pIt->Curr();
			if (pNode->Compare(pNodeTarget))
			{
				return pNode;
			}
		}

		return nullptr;
	}

	void ManagerBase::Remove(DLink* pNodeBase)
	{
		assert(pNodeBase != nullptr);

		pActiveNodes->Remove(pNodeBase);
		activeNodeCount--;

		pNodeBase->Wash();

		pReserveNodes->Push(pNodeBase);
		reserveNodeCount++;
	}

	void ManagerBase::Dump()
	{
		Trace::out("   --- %s: Begin --- \n", STRING_ME(ManagerBase));

		Trace::out("         mDeltaGrow: %d \n", reserveGrowthScale);
		Trace::out("     mTotalNumNodes: %d \n", totalNodeCount);
		Trace::out("       mNumReserved: %d \n", reserveNodeCount);
		Trace::out("         mNumActive: %d \n", activeNodeCount);
		Trace::out("\n");

		Iterator* pItActive = pActiveNodes->GetIterator();
		assert(pItActive != nullptr);

		DLink* pNodeActive = pItActive->First();
		if (pNodeActive == nullptr)
		{
			Trace::out("    Active Head: null\n");
		}
		else
		{
			Trace::out("    Active Head: (%p)\n", pNodeActive);
		}

		Iterator* pItReserve = pReserveNodes->GetIterator();
		assert(pItReserve != nullptr);

		DLink* pNodeReserve = pItReserve->First();
		if (pNodeReserve == nullptr)
		{
			Trace::out("   Reserve Head: null\n");
		}
		else
		{
			Trace::out("   Reserve Head: (%p)\n", pNodeReserve);
		}
		Trace::out("\n");

		Trace::out("   ------ Active List: -----------\n");

		int i = 0;
		DLink* pData = nullptr;

		for (pItActive->First(); !pItActive->IsDone(); pItActive->Next())
		{
			Trace::out("   %d: -------------\n", i);
			pData = pItActive->Curr();
			pData->Dump();
			i++;
		}

		Trace::out("\n");
		Trace::out("   ------ Reserve List: ----------\n");

		i = 0;
		for (pItReserve->First(); !pItReserve->IsDone(); pItReserve->Next())
		{
			Trace::out("   %d: -------------\n", i);
			pData = pItReserve->Curr();
			pData->Dump();
			i++;
		}

		Trace::out("\n");
		Trace::out("   --- %s: End--- \n", STRING_ME(ManagerBase));
		Trace::out("\n");
	}

	void ManagerBase::GrowReserve(int count)
	{
		for (auto i = 0; i < count; i++)
		{
			AddToReserve(NativeCreateNode());
		}
	}

	DLink* ManagerBase::NativeCreateNode()
	{
		totalNodeCount++;
		return CreateNode();
	}

	void ManagerBase::GrowReserve()
	{
		GrowReserve(reserveGrowthScale);
	}

	void ManagerBase::AddToReserve(DLink* pNode)
	{
		reserveNodeCount++;
		pReserveNodes->Push(pNode);
	}

	DLink* ManagerBase::PopReserveNode()
	{
		if (pReserveNodes->IsEmpty())
		{
			GrowReserve();
		}

		DLink* pNode = pReserveNodes->Pop();
		reserveNodeCount--;
		pNode->Wash();

		return pNode;
	}

	void ManagerBase::AddActiveNode(DLink* pNode)
	{
		activeNodeCount++;
		pActiveNodes->Add(pNode);
	}

	void ManagerBase::PushActiveNode(DLink* pNode)
	{
		activeNodeCount++;
		pActiveNodes->Push(pNode);
	}
}