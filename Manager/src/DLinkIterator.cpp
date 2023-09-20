#include "DLinkIterator.h"

namespace Azul
{
	DLinkIterator::DLinkIterator()
		: Iterator()
	{
		this->privClear();
	}

	void DLinkIterator::privClear()
	{
		this->pCurr = nullptr;
		this->pHead = nullptr;
		this->bDone = true;
	}

	DLink* DLinkIterator::Next()
	{
		DLink* pLink = this->pCurr;

		if (pLink != nullptr)
		{
			pLink = pLink->pNext;
		}

		this->pCurr = pLink;

		if (pLink == nullptr)
		{
			bDone = true;
		}
		else
		{
			bDone = false;
		}

		return pLink;
	}

	bool DLinkIterator::IsDone()
	{
		return bDone;
	}

	DLink* DLinkIterator::First()
	{
		if (this->pHead != nullptr)
		{
			this->bDone = false;
			this->pCurr = this->pHead;
		}
		else
		{
			this->privClear();
		}

		return pCurr;
	}

	DLink* DLinkIterator::Curr()
	{
		DLink* pNode = this->pCurr;
		return pNode;
	}

	void DLinkIterator::Reset(DLink* _pHead)
	{
		if (_pHead != nullptr)
		{
			this->bDone = false;
			this->pCurr = _pHead;
			this->pHead = _pHead;
		}
		else
		{
			this->privClear();
		}
	}
}