//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ManBase.h"

//----------------------------------------------------------------------
 // Constructor
 //----------------------------------------------------------------------
ManBase::ManBase(ListBase *_poActive, ListBase *_poReserve, int InitialNumReserved, int DeltaGrow)
{
	// Check now or pay later
	assert(poActive != nullptr);
	assert(poReserve != nullptr);
	assert(InitialNumReserved >= 0);
	assert(DeltaGrow > 0);

	// Initialize all variables
	this->mDeltaGrow = DeltaGrow;
	this->mNumReserved = 0;
	this->mNumActive = 0;
	this->mTotalNumNodes = 0;
	this->poActive = _poActive;
	this->poReserve = _poReserve;

	// Preload the reserve
	this->proFillReservedPool(InitialNumReserved);
}

ManBase::~ManBase()
{
	delete this->poActive;
	delete this->poReserve;

	this->poActive = nullptr;
	this->poReserve = nullptr;
}
//----------------------------------------------------------------------
// Base methods - called in Derived class but lives in Base
//----------------------------------------------------------------------
void ManBase::baseSetReserve(int reserveNum, int reserveGrow)
{
	this->mDeltaGrow = reserveGrow;

	if(reserveNum > this->mNumReserved)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum - this->mNumReserved);
	}
}

DLink *ManBase::baseAddToFront()
{
	Iterator *pIt = poReserve->GetIterator();
	assert(pIt != nullptr);

	// Are there any nodes on the Reserve list?
	if(pIt->First() == nullptr)
	{
		// refill the reserve list by the DeltaGrow
		this->proFillReservedPool(this->mDeltaGrow);
	}

	// Always take from the reserve list
	DLink *pNodeBase = poReserve->RemoveFromFront();
	assert(pNodeBase != nullptr);

	// Wash it
	pNodeBase->Wash();

	// Update stats
	this->mNumActive++;
	this->mNumReserved--;

	// copy to active
	poActive->AddToFront(pNodeBase);

	// YES - here's your new one (may its reused from reserved)
	return pNodeBase;
}

DLink *ManBase::baseAddToEnd()
{
	Iterator *pIt = poReserve->GetIterator();
	assert(pIt != nullptr);

	// Are there any nodes on the Reserve list?
	if(pIt->First() == nullptr)
	{
		// refill the reserve list by the DeltaGrow
		this->proFillReservedPool(this->mDeltaGrow);
	}

	// Always take from the reserve list
	DLink *pNodeBase = poReserve->RemoveFromFront();
	assert(pNodeBase != nullptr);

	// Wash it
	pNodeBase->Wash();

	// Update stats
	this->mNumActive++;
	this->mNumReserved--;

	// copy to active
	poActive->AddToEnd(pNodeBase);

	// YES - here's your new one (may its reused from reserved)
	return pNodeBase;
}

Iterator *ManBase::baseGetActiveIterator()
{
	return poActive->GetIterator();
}
Iterator *ManBase::baseGetReserveIterator()
{
	return poReserve->GetIterator();
}

DLink *ManBase::baseFind(DLink *pNodeTarget)
{	
	DLink *pNode = nullptr;

	// search the active list
	Iterator *pIt = poActive->GetIterator();

	for(pIt->First(); !pIt->IsDone(); pIt->Next())
	{
		pNode = pIt->Curr();
		if(pNode->Compare(pNodeTarget))
		{
			// found it
			break;
		}
	}

	return pNode;
}

void ManBase::baseRemove(DLink *pNodeBase)
{
	assert(pNodeBase != nullptr);

	// Don't do the work here... delegate it
	poActive->Remove(pNodeBase);

	// wash it before returning to reserve list
	pNodeBase->Wash();

	// add it to the return list
	poReserve->AddToFront(pNodeBase);

	// stats update
	this->mNumActive--;
	this->mNumReserved++;
}

void ManBase::baseDump()
{
    Trace::out("\n");
	Trace::out("   --- %s: Begin --- \n", STRING_ME(ManBase));
	Trace::out("\n");

	Trace::out("         mDeltaGrow: %d \n", mDeltaGrow);
	Trace::out("     mTotalNumNodes: %d \n", mTotalNumNodes);
	Trace::out("       mNumReserved: %d \n", mNumReserved);
	Trace::out("         mNumActive: %d \n", mNumActive);
	Trace::out("\n");

	Iterator *pItActive = poActive->GetIterator();
	assert(pItActive != nullptr);

	DLink *pNodeActive = pItActive->First();
	if(pNodeActive == nullptr)
	{
		Trace::out("    Active Head: null\n");
	}
	else
	{
		Trace::out("    Active Head: (%p)\n", pNodeActive);
	}

	Iterator *pItReserve = poReserve->GetIterator();
	assert(pItReserve != nullptr);

	DLink *pNodeReserve = pItReserve->First();
	if(pNodeReserve == nullptr)
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
	DLink *pData = nullptr;
	
	for(pItActive->First();!pItActive->IsDone();pItActive->Next())
	{
		Trace::out("   %d: -------------\n", i);
		pData = pItActive->Curr();
		pData->Dump();
		i++;
	}

	Trace::out("\n");
	Trace::out("   ------ Reserve List: ----------\n");

	i = 0;
	for(pItReserve->First();!pItReserve->IsDone();pItReserve->Next())
	{
		Trace::out("   %d: -------------\n", i);
		pData = pItReserve->Curr();
		pData->Dump();
		i++;
	}

	Trace::out("\n");
	Trace::out("   --- %s: End--- \n", STRING_ME(ManBase));
	Trace::out("\n");
}


//----------------------------------------------------------------------
// Private methods - helpers
//----------------------------------------------------------------------
void ManBase::proFillReservedPool(int count)
{
	// doesn't make sense if its not at least 1
	assert(count >= 0);

	this->mTotalNumNodes += count;
	this->mNumReserved += count;

	// Preload the reserve
	for(int i = 0; i < count; i++)
	{
		DLink *pNode = this->derivedCreateNode();
		assert(pNode != nullptr);

		poReserve->AddToFront(pNode);
	}
}

