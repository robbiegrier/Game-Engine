//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "PCSTreeReverseIterator.h"

namespace Azul
{
	PCSTreeReverseIterator::PCSTreeReverseIterator(PCSNode* pInRoot)
		: pRoot(pInRoot), pCurr(nullptr), started(false)
	{
		First();
	}

	PCSNode* PCSTreeReverseIterator::First()
	{
		if (!pRoot)
		{
			return nullptr;
		}

		pCurr = pRoot->GetReverse();
		started = false;
		return Current();
	}

	PCSNode* PCSTreeReverseIterator::Next()
	{
		if (!pCurr)
		{
			return nullptr;
		}

		pCurr = pCurr->GetReverse();
		started = true;

		if (IsDone())
		{
			return nullptr;
		}

		return pCurr;
	}

	PCSNode* PCSTreeReverseIterator::Current()
	{
		return pCurr;
	}

	bool PCSTreeReverseIterator::IsDone()
	{
		return (started && pCurr == pRoot->GetReverse()) || !pCurr || !pRoot;
	}
}

// ---  End of File ---------------