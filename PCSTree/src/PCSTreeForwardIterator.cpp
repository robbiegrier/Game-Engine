//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "PCSTreeForwardIterator.h"

namespace Azul
{
	PCSTreeForwardIterator::PCSTreeForwardIterator(PCSNode* pInRoot)
		: pRoot(pInRoot), pCurr(nullptr), started(false)
	{
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
		return (started && pCurr == pRoot) || !pCurr || !pRoot;
	}
}

// ---  End of File ---------------