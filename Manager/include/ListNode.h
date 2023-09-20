#ifndef _ListNode
#define _ListNode

#include "DLink.h"

namespace Azul
{
	class ListNode : public DLink
	{
	public:
		ListNode() = default;
		ListNode(DLink* pElement);
		ListNode(const ListNode& other);
		ListNode& operator=(const ListNode& other);
		virtual ~ListNode();

		void Set(DLink* pElement);
		DLink* Get() const;

		virtual char* GetName() override;
		virtual void Wash() override;
		virtual bool Compare(DLink* pTargetNode) override;
		virtual void Dump() override;

	protected:
		DLink* pElement = nullptr;
	};

	class OwningListNode : public ListNode
	{
	public:
		OwningListNode() = default;
		OwningListNode(DLink* pElement);
		OwningListNode(const OwningListNode& other) = delete;
		OwningListNode& operator=(const OwningListNode& other) = delete;
		virtual ~OwningListNode();
	};
}

#endif