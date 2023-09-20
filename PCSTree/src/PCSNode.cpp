//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#include "PCSTree.h"
#include "PCSNode.h"

namespace Azul
{
	PCSNode::PCSNode()
		: pParent(nullptr), pChild(nullptr), pNextSibling(nullptr), pPrevSibling(nullptr), pForward(nullptr), pReverse(nullptr), pName()
	{
	}

	PCSNode::PCSNode(const PCSNode& in)
		: pParent(in.GetParent()), pChild(in.GetChild()), pNextSibling(in.GetNextSibling()), pPrevSibling(in.GetPrevSibling()),
		pForward(in.pForward), pReverse(in.pReverse), pName()
	{
		in.GetName(pName, NAME_SIZE);
	}

	PCSNode::PCSNode(PCSNode* const pInParent, PCSNode* const pInChild, PCSNode* const pInNextSibling, PCSNode* const pInPrevSibling, const char* const pInName)
		: pParent(pInParent), pChild(pInChild), pNextSibling(pInNextSibling), pPrevSibling(pInPrevSibling),
		pForward(nullptr), pReverse(nullptr), pName()
	{
		SetName(pInName);
	}

	PCSNode::PCSNode(const char* const pInName)
		: pParent(nullptr), pChild(nullptr), pNextSibling(nullptr), pPrevSibling(nullptr), pForward(nullptr), pReverse(nullptr)
	{
		SetName(pInName);
	}

	PCSNode::~PCSNode()
	{
		// No ownership
	}

	PCSNode& PCSNode::operator = (const PCSNode& in)
	{
		SetParent(in.GetParent());
		SetChild(in.GetChild());
		SetNextSibling(in.GetNextSibling());
		SetPrevSibling(in.GetPrevSibling());
		pForward = in.pForward;
		pReverse = in.pReverse;
		in.GetName(pName, NAME_SIZE);

		return *this;
	}

	void PCSNode::SetParent(PCSNode* const pIn)
	{
		pParent = pIn;
	}

	void PCSNode::SetChild(PCSNode* const pIn)
	{
		pChild = pIn;
	}

	void PCSNode::SetNextSibling(PCSNode* const pIn)
	{
		pNextSibling = pIn;
	}

	void PCSNode::SetPrevSibling(PCSNode* const pIn)
	{
		pPrevSibling = pIn;
	}

	PCSNode* PCSNode::GetParent(void) const
	{
		return pParent;
	}

	PCSNode* PCSNode::GetChild(void) const
	{
		return pChild;
	}

	PCSNode* PCSNode::GetNextSibling(void) const
	{
		return pNextSibling;
	}

	PCSNode* PCSNode::GetPrevSibling(void) const
	{
		return pPrevSibling;
	}

	PCSNode::Code PCSNode::SetName(const char* const pInName)
	{
		auto output = Code::SUCCESS;

		if (!pInName)
		{
			output = Code::FAIL_NULL_PTR;
		}
		else
		{
			memcpy(pName, pInName, NAME_SIZE);
		}

		return output;
	}

	PCSNode::Code PCSNode::GetName(char* const pOutBuffer, unsigned int sizeOutBuffer) const
	{
		auto output = Code::SUCCESS;

		if (!pName || !pOutBuffer)
		{
			output = Code::FAIL_NULL_PTR;
		}
		else
		{
			memcpy(pOutBuffer, pName, sizeOutBuffer - 1);
			pOutBuffer[sizeOutBuffer - 1] = '\0';
		}

		return output;
	}

	int PCSNode::GetNumSiblings() const
	{
		auto output = 0;

		if (pParent)
		{
			auto pCurr = GetParent()->GetChild();
			while (pCurr)
			{
				output++;
				pCurr = pCurr->GetNextSibling();
			}
		}
		else
		{
			output = 1;
		}

		return output;
	}

	int PCSNode::GetNumChildren() const
	{
		auto output = 0;

		auto pCurr = GetChild();

		while (pCurr)
		{
			output++;
			pCurr = pCurr->GetNextSibling();
		}

		return output;
	}

	int PCSNode::GetDepth() const
	{
		auto output = 0;

		auto pCurr = this;
		while (pCurr)
		{
			pCurr = pCurr->GetParent();
			output++;
		}

		return output;
	}

	void PCSNode::Nullify()
	{
		SetParent(nullptr);
		SetChild(nullptr);
		SetNextSibling(nullptr);
		SetPrevSibling(nullptr);
	}

	bool PCSNode::IsRoot() const
	{
		return pParent == nullptr;
	}

	void PCSNode::IteratorBind(PCSNode* pFirst, PCSNode* pSecond)
	{
		if (pFirst)
		{
			pFirst->pForward = pSecond;
		}

		if (pSecond)
		{
			pSecond->pReverse = pFirst;
		}
	}

	PCSNode* PCSNode::IteratorGetNextHorizontal()
	{
		auto pPositionVertical = this;

		while (!pPositionVertical->pNextSibling)
		{
			if (pPositionVertical->IsRoot())
			{
				return pPositionVertical;
			}

			pPositionVertical = pPositionVertical->pParent;
		}

		return pPositionVertical->pNextSibling;
	}

	PCSNode* PCSNode::GetForward() const
	{
		return pForward;
	}

	PCSNode* PCSNode::GetReverse() const
	{
		return pReverse;
	}

	void PCSNode::PrintNode() const
	{
		Trace::out("PCSNode %s\n", pName);
	}

	void PCSNode::PrintChildren() const
	{
		Trace::out("%s children: [ ", pName);

		auto pCurr = GetChild();

		while (pCurr)
		{
			Trace::out("%s, ", pCurr->pName);
			pCurr = pCurr->GetNextSibling();
		}

		Trace::out("]\n");
	}

	void PCSNode::PrintSiblings() const
	{
		Trace::out("%s siblings: [ ", pName);

		if (pParent)
		{
			auto pCurr = pParent->GetChild();

			while (pCurr)
			{
				Trace::out("%s, ", pCurr->pName);
				pCurr = pCurr->GetNextSibling();
			}
		}
		else
		{
			Trace::out("%s, ", pName);
		}

		Trace::out("]\n");
	}
}

// ---  End of File ---