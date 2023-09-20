#include "ListNode.h"

namespace Azul
{
	ListNode::ListNode(DLink* pInElement)
		: pElement(pInElement)
	{
	}

	ListNode::ListNode(const ListNode& other)
		: pElement(other.pElement)
	{
	}

	ListNode& ListNode::operator=(const ListNode& other)
	{
		pElement = other.pElement;
		return *this;
	}

	ListNode::~ListNode()
	{
	}

	void ListNode::Set(DLink* pInElement)
	{
		pElement = pInElement;
	}

	DLink* ListNode::Get() const
	{
		return pElement;
	}

	char* ListNode::GetName()
	{
		if (pElement)
		{
			return pElement->GetName();
		}

		return "null";
	}

	void ListNode::Wash()
	{
		if (pElement)
		{
			pElement->Wash();
			pElement = nullptr;
		}
	}

	bool ListNode::Compare(DLink* pTargetNode)
	{
		if (pElement)
		{
			auto pOtherElement = ((ListNode*)pTargetNode)->Get();
			return pElement->Compare(pOtherElement);
		}

		return false;
	}

	void ListNode::Dump()
	{
		if (pElement)
		{
			pElement->Dump();
		}
		else
		{
			Trace::out("null\n");
		}
	}

	OwningListNode::OwningListNode(DLink* pInElement)
		: ListNode(pInElement)
	{
	}

	OwningListNode::~OwningListNode()
	{
		if (pElement)
		{
			delete pElement;
		}
	}
}