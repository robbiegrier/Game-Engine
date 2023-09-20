#include "DLinkedList.h"

namespace Azul
{
	DLinkedList::DLinkedList()
	{
		pIterator = new DLinkIterator();
		pHead = nullptr;
	}

	DLinkedList::~DLinkedList()
	{
		auto pCurr = pHead;

		while (pCurr)
		{
			auto pTmp = pCurr;
			pCurr = pCurr->pNext;
			delete pTmp;
		}

		delete pIterator;
	}

	void DLinkedList::Push(DLink* pNode)
	{
		assert(pNode);

		pNode->pPrev = nullptr;
		pNode->pNext = pHead;

		if (pHead)
		{
			pHead->pPrev = pNode;
		}

		if (!pTail)
		{
			pTail = pNode;
		}

		pHead = pNode;
	}

	void DLinkedList::Add(DLink* pNode)
	{
		assert(pNode);

		pNode->pNext = nullptr;
		pNode->pPrev = pTail;

		if (pTail)
		{
			pTail->pNext = pNode;
		}

		if (!pHead)
		{
			pHead = pNode;
		}

		pTail = pNode;
	}

	DLink* DLinkedList::Remove(DLink* pNode)
	{
		assert(pNode);

		if (pNode == pHead)
		{
			pHead = pNode->pNext;
		}

		if (pNode == pTail)
		{
			pTail = pNode->pPrev;
		}

		if (pNode->pPrev)
		{
			pNode->pPrev->pNext = pNode->pNext;
		}

		if (pNode->pNext)
		{
			pNode->pNext->pPrev = pNode->pPrev;
		}

		pNode->pPrev = nullptr;
		pNode->pNext = nullptr;

		return pNode;
	}

	DLink* DLinkedList::Pop()
	{
		return Remove(pHead);
	}

	void DLinkedList::Enqueue(DLink* pNode)
	{
		Add(pNode);
	}

	DLink* DLinkedList::Dequeue()
	{
		return Pop();
	}

	DLink* DLinkedList::First() const
	{
		return pHead;
	}

	DLink* DLinkedList::Last() const
	{
		return pTail;
	}

	bool DLinkedList::IsEmpty() const
	{
		return pHead == nullptr;
	}

	Iterator* DLinkedList::GetIterator() const
	{
		pIterator->Reset(pHead);
		return pIterator;
	}
}