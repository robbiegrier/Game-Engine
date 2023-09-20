#ifndef _DLinkedList
#define _DLinkedList

#include "ListBase.h"
#include "DLinkIterator.h"

namespace Azul
{
	class DLinkedList : public ListBase
	{
	public:
		DLinkedList();
		DLinkedList(const DLinkedList& other) = delete;
		DLinkedList& operator=(const DLinkedList& other) = delete;
		~DLinkedList();

	public:
		virtual void Add(DLink* pNode) override;
		virtual DLink* Remove(DLink* pNode) override;
		virtual void Push(DLink* pNode) override;
		virtual DLink* Pop() override;
		virtual void Enqueue(DLink* pNode) override;
		virtual DLink* Dequeue() override;
		virtual DLink* First() const override;
		virtual DLink* Last() const override;
		virtual bool IsEmpty() const override;
		virtual Iterator* GetIterator() const override;

	private:
		DLink* pHead = nullptr;
		DLink* pTail = nullptr;
		DLinkIterator* pIterator;
	};
}

#endif
