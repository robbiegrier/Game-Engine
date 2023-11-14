//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef DLINK_MAN_H
#define DLINK_MAN_H

#include "ListBase.h"
#include "DLinkIterator.h"

class DLinkMan : public ListBase
{
public:
	DLinkMan();
	virtual ~DLinkMan();

	virtual void AddToFront(DLink *pNode) override;

	virtual void AddToEnd(DLink *pNode) override;

	virtual void Remove(DLink *pNode) override;

	virtual DLink *RemoveFromFront() override;

	virtual Iterator *GetIterator() override;


	DLinkIterator *poIterator;
	DLink *poHead;

};

#endif
