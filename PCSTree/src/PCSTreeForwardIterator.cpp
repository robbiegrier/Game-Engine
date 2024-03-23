//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "PCSTreeForwardIterator.h"

namespace Azul
{
	PCSTreeForwardIterator::PCSTreeForwardIterator(PCSNode* pInRoot)
		: pRoot(pInRoot), pLast(nullptr), pCurr(nullptr), started(false)
	{
		pLast = pInRoot->GetLastChild();
		First();
	}

	PCSNode* PCSTreeForwardIterator::First()
	{
		if (!pRoot)
		{
			return nullptr;
		}

		pCurr = pRoot;
		started = false;
		return Current();
	}

	PCSNode* PCSTreeForwardIterator::Next()
	{
		if (!pCurr)
		{
			return nullptr;
		}

		pCurr = pCurr->GetForward();
		started = true;

		if (IsDone())
		{
			return nullptr;
		}

		return pCurr;
	}

	PCSNode* PCSTreeForwardIterator::Current()
	{
		return pCurr;
	}

	bool PCSTreeForwardIterator::IsDone()
	{
		const bool lastReached = started && (pCurr && pCurr->GetReverse() ? pCurr->GetReverse() == pLast : false);
		return (started && pCurr == pRoot) || !pCurr || !pRoot || lastReached;
	}
}

// ---  End of File ---------------