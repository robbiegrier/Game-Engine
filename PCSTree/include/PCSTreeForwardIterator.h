//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#ifndef PCSTREE_FORWARD_ITERATOR_H
#define PCSTREE_FORWARD_ITERATOR_H

#include "PCSTreeIterator.h"

namespace Azul
{
	class PCSTreeForwardIterator : public PCSTreeIterator
	{
	public:
		PCSTreeForwardIterator(PCSNode* rootNode);

		PCSTreeForwardIterator() = delete;
		PCSTreeForwardIterator(const PCSTreeForwardIterator&) = delete;
		PCSTreeForwardIterator& operator = (const PCSTreeForwardIterator&) = delete;
		virtual ~PCSTreeForwardIterator() = default;

		virtual PCSNode* First() override;
		virtual PCSNode* Next() override;
		virtual bool IsDone() override;
		virtual PCSNode* Current() override;

	private:
		PCSNode* pRoot;
		PCSNode* pLast;
		PCSNode* pCurr;
		bool started;
	};
}

#endif

// ---  End of File ---------------