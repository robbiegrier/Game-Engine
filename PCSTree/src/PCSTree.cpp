//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "PCSTree.h"
#include "PCSNode.h"
#include "PCSTreeForwardIterator.h"

namespace Azul
{
	PCSTree::PCSTree()
	{
		pRoot = nullptr;
	};

	PCSTree& PCSTree::operator=(const PCSTree& in)
	{
		pRoot = in.pRoot;
		info = in.info;
		return *this;
	}

	PCSTree::~PCSTree()
	{
		// nothing
	};

	void PCSTree::Insert(PCSNode* const pInNode, PCSNode* const pParent)
	{
		assert(pInNode);

		if (IsEmpty())
		{
			InsertFirstNode(pInNode);
		}
		else
		{
			InsertAnotherNode(pParent, pInNode);
		}

		info.OnNodeInserted(pInNode);
	}

	void PCSTree::InsertBack(PCSNode* const pInNode, PCSNode* const pParent)
	{
		assert(pInNode);

		if (IsEmpty())
		{
			InsertFirstNode(pInNode);
		}
		else if (!pParent->GetChild())
		{
			InsertAnotherNode(pParent, pInNode);
		}
		else
		{
			PCSNode* pPrev = pParent->GetChild();

			while (pPrev->GetNextSibling())
			{
				pPrev = pPrev->GetNextSibling();
			}

			PCSNode* pLastChildPrev = GetLastChild(pPrev);

			pPrev->SetNextSibling(pInNode);
			pInNode->SetPrevSibling(pPrev);
			pInNode->SetParent(pParent);

			//Trace::out("InsertBack Rebinding Iterator: %s => %s \n", pInNode->GetNamePtr(), pLastChildPrev->GetForward()->GetNamePtr());
			PCSNode::IteratorBind(GetLastChild(pInNode), pLastChildPrev->GetForward());
			//Trace::out("InsertBack Rebinding Iterator: %s => %s \n", pLastChildPrev->GetNamePtr(), pInNode->GetNamePtr());
			PCSNode::IteratorBind(pLastChildPrev, pInNode);
		}

		info.OnNodeInserted(pInNode);
	}

	void PCSTree::Remove(PCSNode* const pInNode)
	{
		assert(pInNode);
		BurnSubtree(pInNode);
		RecalculateCurrentDepth();
	}

	void PCSTree::DetachSubtree(PCSNode* const pInNode)
	{
		//Trace::out("Rebinding Iterator: %s => %s \n", pInNode->GetReverse()->GetNamePtr(), GetLastChild(pInNode)->GetForward()->GetNamePtr());

		PCSNode::IteratorBind(pInNode->GetReverse(), GetLastChild(pInNode)->GetForward());

		if (pInNode->GetParent())
		{
			if (pInNode->GetParent()->GetChild() == pInNode)
			{
				pInNode->GetParent()->SetChild(pInNode->GetNextSibling());
			}
		}

		if (pInNode->GetNextSibling())
		{
			pInNode->GetNextSibling()->SetPrevSibling(pInNode->GetPrevSibling());
		}

		if (pInNode->GetPrevSibling())
		{
			pInNode->GetPrevSibling()->SetNextSibling(pInNode->GetNextSibling());
		}

		pInNode->SetNextSibling(nullptr);
		pInNode->SetPrevSibling(nullptr);
		pInNode->SetParent(nullptr);
		PCSNode::IteratorBind(nullptr, pInNode);
		PCSNode::IteratorBind(pInNode->GetLastChild(), nullptr);

		RecalculateCurrentDepth();
	}

	void PCSTree::RelocateSubtree(PCSNode* const pInNode, PCSNode* const pParent, PCSNode* const pPrev)
	{
		assert(pParent);

		DetachSubtree(pInNode);
		PCSNode* pLastChild = GetLastChild(pInNode);

		if (pPrev)
		{
			pInNode->SetParent(pParent);
			pInNode->SetNextSibling(pPrev->GetNextSibling());

			if (pPrev->GetNextSibling())
			{
				pPrev->GetNextSibling()->SetPrevSibling(pInNode);
			}

			pInNode->SetPrevSibling(pPrev);
			pPrev->SetNextSibling(pInNode);

			PCSNode* pLastChildPrev = GetLastChild(pPrev);

			//Trace::out("Rebinding Iterator: %s => %s \n", pLastChildPrev->GetNamePtr(), pInNode->GetNamePtr());
			PCSNode::IteratorBind(pLastChildPrev, pInNode);

			//Trace::out("Rebinding Iterator: %s => %s \n", pLastChild->GetNamePtr(), pInNode->IteratorGetNextHorizontal()->GetNamePtr());
			PCSNode::IteratorBind(pLastChild, pInNode->IteratorGetNextHorizontal());
		}
		else
		{
			PCSNode* pExistingChild = pParent->GetChild();
			pParent->SetChild(pInNode);
			pInNode->SetParent(pParent);
			pInNode->SetNextSibling(pExistingChild);
			pInNode->SetPrevSibling(nullptr);

			//Trace::out("Rebinding Iterator: %s => %s \n", pParent->GetNamePtr(), pInNode->GetNamePtr());
			PCSNode::IteratorBind(pParent, pInNode);

			//Trace::out("Rebinding Iterator: %s => %s \n", pLastChild->GetNamePtr(), pInNode->IteratorGetNextHorizontal()->GetNamePtr());
			PCSNode::IteratorBind(pLastChild, pInNode->IteratorGetNextHorizontal());

			if (pExistingChild)
			{
				pExistingChild->SetPrevSibling(pInNode);
			}
		}

		RecalculateCurrentDepth();
	}

	PCSNode* PCSTree::GetLastChild(PCSNode* const pInNode)
	{
		return pInNode->GetLastChild();
	}

	void PCSTree::BurnSubtree(PCSNode* const pInNode)
	{
		auto pCurr = pInNode;

		while (pCurr->GetChild())
		{
			pCurr = pCurr->GetChild();
		}

		bool justWentUp = false;

		while (pCurr != pInNode)
		{
			if (!justWentUp)
			{
				while (pCurr->GetChild())
				{
					pCurr = pCurr->GetChild();
				}
			}

			auto pBurner = pCurr;

			if (!pCurr->GetNextSibling())
			{
				pCurr = pCurr->GetParent();
				justWentUp = true;
			}
			else
			{
				pCurr = pCurr->GetNextSibling();
				justWentUp = false;
			}

			BurnNode(pBurner);
		}

		BurnNode(pInNode);
	}

	void PCSTree::BurnNode(PCSNode* const pBurner)
	{
		PCSNode::IteratorBind(pBurner->GetReverse(), pBurner->GetForward());

		if (pBurner->GetParent())
		{
			if (pBurner->GetParent()->GetChild() == pBurner)
			{
				pBurner->GetParent()->SetChild(pBurner->GetNextSibling());
			}
		}

		if (pBurner->GetNextSibling())
		{
			pBurner->GetNextSibling()->SetPrevSibling(pBurner->GetPrevSibling());
		}

		if (pBurner->GetPrevSibling())
		{
			pBurner->GetPrevSibling()->SetNextSibling(pBurner->GetNextSibling());
		}

		if (pBurner == pRoot)
		{
			pRoot = nullptr;
		}

		info.DecrementNodes();
		pBurner->Nullify();
	}

	void PCSTree::RecalculateCurrentDepth()
	{
		if (!pRoot)
		{
			info.currNumLevels = 0;
		}
		else
		{
			auto depth = 1;
			auto maxDepth = 1;

			auto pCurr = pRoot;

			while (pCurr->GetChild())
			{
				maxDepth = std::max(maxDepth, ++depth);
				pCurr = pCurr->GetChild();
			}

			bool justWentUp = false;

			while (pCurr != pRoot)
			{
				if (!justWentUp)
				{
					while (pCurr->GetChild())
					{
						maxDepth = std::max(maxDepth, ++depth);
						pCurr = pCurr->GetChild();
					}
				}

				if (!pCurr->GetNextSibling())
				{
					depth--;
					pCurr = pCurr->GetParent();
					justWentUp = true;
				}
				else
				{
					pCurr = pCurr->GetNextSibling();
					justWentUp = false;
				}
			}

			info.currNumLevels = maxDepth;
		}
	}

	PCSNode* PCSTree::GetRoot() const
	{
		return pRoot;
	}

	void PCSTree::GetInfo(Info& outInfo)
	{
		outInfo = info;
	}

	void PCSTree::Print() const
	{
		//PrintHelper(pRoot, 0);

		PCSTreeForwardIterator it(pRoot);

		Trace::out("\n------------------\n");
		for (it.First(); !it.IsDone(); it.Next())
		{
			const PCSNode* pCurr = it.Current();

			char pName[PCSNode::NAME_SIZE];
			PCSNode::Code res;
			res = pCurr->GetName(pName, PCSNode::NAME_SIZE);
			assert(res == PCSNode::Code::SUCCESS);

			for (auto i = 0; i < pCurr->GetDepth() - 1; i++)
			{
				Trace::out("    ");
			}

			Trace::out("[%s]\n", pName);
		}
		Trace::out("------------------\n");
	}

	void PCSTree::PrintHelper(PCSNode const* const pCurr, int depth) const
	{
		char pName[PCSNode::NAME_SIZE];
		PCSNode::Code res;
		res = pCurr->GetName(pName, PCSNode::NAME_SIZE);
		assert(res == PCSNode::Code::SUCCESS);

		// indent
		for (auto i = 0; i < depth; i++)
		{
			Trace::out("    ");
		}

		Trace::out("[%s]\n", pName);

		if (pCurr->GetChild())
		{
			PrintHelper(pCurr->GetChild(), depth + 1);
		}

		if (pCurr->GetNextSibling())
		{
			PrintHelper(pCurr->GetNextSibling(), depth);
		}
	}

	void PCSTree::InsertFirstNode(PCSNode* const pInNode)
	{
		assert(!pRoot);
		assert(info.currNumNodes == 0);

		pInNode->SetParent(nullptr);
		pInNode->SetNextSibling(nullptr);
		pInNode->SetPrevSibling(nullptr);
		pInNode->SetChild(nullptr);

		PCSNode::IteratorBind(nullptr, pInNode);
		PCSNode::IteratorBind(pInNode, pInNode->IteratorGetNextHorizontal());

		pRoot = pInNode;
	}

	void PCSTree::InsertAnotherNode(PCSNode* const pParent, PCSNode* const pInNode)
	{
		assert(pParent);

		auto pExistingChild = pParent->GetChild();
		pParent->SetChild(pInNode);
		pInNode->SetParent(pParent);
		pInNode->SetNextSibling(pExistingChild);
		pInNode->SetPrevSibling(nullptr);

		PCSNode::IteratorBind(pParent, pInNode);
		PCSNode::IteratorBind(GetLastChild(pInNode), pInNode->IteratorGetNextHorizontal());

		if (pExistingChild)
		{
			pExistingChild->SetPrevSibling(pInNode);
		}
	}

	bool PCSTree::IsEmpty() const
	{
		return !pRoot;
	}

	void PCSTree::Info::IncrementNodes()
	{
		maxNumNodes = std::max(maxNumNodes, ++currNumNodes);
	}

	void PCSTree::Info::IncrementLevels()
	{
		maxNumLevels = std::max(maxNumLevels, ++currNumLevels);
	}

	void PCSTree::Info::DecrementNodes()
	{
		currNumNodes--;
	}

	void PCSTree::Info::DecrementLevels()
	{
		currNumLevels--;
	}

	void PCSTree::Info::OnNodeInserted(const PCSNode* const pInNode)
	{
		if (pInNode->GetDepth() > currNumLevels)
		{
			IncrementLevels();
		}

		IncrementNodes();
	}
}

// ---  End of File ---