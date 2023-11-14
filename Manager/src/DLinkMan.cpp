//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "DLinkMan.h"

DLinkMan::DLinkMan()
{
	this->poIterator = new DLinkIterator();
	this->poHead = nullptr;
}

DLinkMan::~DLinkMan()
{
	delete this->poIterator;
	this->poIterator = nullptr;

}

void DLinkMan::AddToFront(DLink *pNode)
{
	// add to front
	assert(pNode != nullptr);

	// add node
	if(poHead == nullptr)
	{
		// push to the front
		poHead = pNode;
		pNode->pNext = nullptr;
		pNode->pPrev = nullptr;
	}
	else
	{
		// push to front
		pNode->pPrev = nullptr;
		pNode->pNext = poHead;

		// update head
		poHead->pPrev = pNode;
		poHead = pNode;
	}

	// worst case, pHead was null initially, now we added a node so... this is true
	assert(poHead != nullptr);
}

void DLinkMan::AddToEnd(DLink *pNode)
{
	// add to front
	assert(pNode != nullptr);

	// add node
	if(poHead == nullptr)
	{
		// none on list... so add it
		poHead = pNode;
		pNode->pNext = nullptr;
		pNode->pPrev = nullptr;
	}
	else
	{
		// spin until end
		DLink *pTmp = poHead;
		DLink *pLast = pTmp;
		while(pTmp != nullptr)
		{
			pLast = pTmp;
			pTmp = pTmp->pNext;
		}

		// push to front
		pLast->pNext = pNode;
		pNode->pPrev = pLast;
		pNode->pNext = nullptr;

	}

	// worst case, pHead was null initially, now we added a node so... this is true
	assert(poHead != nullptr);
}

void DLinkMan::Remove(DLink *pNode)
{
	// There should always be something on list
	assert(poHead != nullptr);
	assert(pNode != nullptr);

	// four cases
	if(pNode->pPrev == nullptr && pNode->pNext == nullptr)
	{   // Only node
		poHead = nullptr;
	}
	else if(pNode->pPrev == nullptr && pNode->pNext != nullptr)
	{   // First node
		poHead = pNode->pNext;
		pNode->pNext->pPrev = pNode->pPrev;
	}
	else if(pNode->pPrev != nullptr && pNode->pNext == nullptr)
	{   // Last node
		pNode->pPrev->pNext = pNode->pNext;
	}
	else
	{   // Middle node
		pNode->pNext->pPrev = pNode->pPrev;
		pNode->pPrev->pNext = pNode->pNext;
	}

	// remove any lingering links
	// HUGELY important - otherwise its crossed linked 
	pNode->Clear();
}

DLink *DLinkMan::RemoveFromFront()
{
	// There should always be something on list
	assert(poHead != nullptr);

	// return node
	DLink *pNode = poHead;

	// Update head (OK if it points to NULL)
	poHead = poHead->pNext;
	if(poHead != nullptr)
	{
		poHead->pPrev = nullptr;
		// do not change pEnd
	}
	else
	{
		// only one on the list
		// pHead == null
	}

	// remove any lingering links
	// HUGELY important - otherwise its crossed linked 
	pNode->Clear();

	return pNode;
}

Iterator *DLinkMan::GetIterator()
{
	poIterator->Reset(this->poHead);
	return poIterator;
}


