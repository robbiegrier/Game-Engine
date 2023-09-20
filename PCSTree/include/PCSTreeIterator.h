//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef PCSTREE_ITERATOR_H
#define PCSTREE_ITERATOR_H

#include "PCSNode.h"

namespace Azul
{
	class PCSTreeIterator
	{
	public:

		PCSTreeIterator() = default;
		PCSTreeIterator(const PCSTreeIterator &) = delete;
		PCSTreeIterator &operator = (const PCSTreeIterator &) = delete;
		virtual ~PCSTreeIterator() = default;

		virtual PCSNode *First() = 0;
		virtual PCSNode *Next() = 0;
		virtual bool IsDone() = 0;
		virtual PCSNode *Current() = 0;
	};
}

#endif

// ---  End of File ---------------
