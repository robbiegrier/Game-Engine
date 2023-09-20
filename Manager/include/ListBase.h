#ifndef _ListBase
#define _ListBase

#include "DLink.h"
#include "Iterator.h"

namespace Azul
{
	class ListBase
	{
	public:
		// Add to the end
		virtual void Add(DLink* pNode) = 0;

		// Remove a node by pointer
		virtual DLink* Remove(DLink* pNode) = 0;

		// Push to the front
		virtual void Push(DLink* pNode) = 0;

		// Remove from the front
		virtual DLink* Pop() = 0;

		// Add as a fifo queue
		virtual void Enqueue(DLink* pNode) = 0;

		// Remove as a fifo queue
		virtual DLink* Dequeue() = 0;

		// Get the first element
		virtual DLink* First() const = 0;

		// Get the last element
		virtual DLink* Last() const = 0;

		// Check if the list is empty
		virtual bool IsEmpty() const = 0;

		// Get the iterator
		virtual Iterator* GetIterator() const = 0;

	public:
		// Virtual destructor
		virtual ~ListBase() = default;
	};
}

#endif
