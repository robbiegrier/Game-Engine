//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

#ifndef PCSTREE_H
#define PCSTREE_H

namespace Azul
{
	// forward declare
	class PCSNode;

	// PCSTree class
	class PCSTree
	{
	public:
		struct Info
		{
			// Current number of nodes in the tree
			int currNumNodes = 0;

			// Peak number of nodes in the tree at any point
			int maxNumNodes = 0;

			// Current number of levels (depth) in the tree
			int currNumLevels = 0;

			// Max number of levels (depth) in the tree at any point
			int maxNumLevels = 0;

			inline void IncrementNodes();
			inline void IncrementLevels();
			inline void DecrementNodes();
			inline void DecrementLevels();

			void OnNodeInserted(const PCSNode* const pInNode);
		};

		// big four
		PCSTree();
		PCSTree(const PCSTree& in) = delete;
		PCSTree& operator = (const PCSTree& in);
		~PCSTree();

		// insert
		void Insert(PCSNode* const pInNode, PCSNode* const pParent);

		// inserts as the last child of the parent
		void InsertBack(PCSNode* const pInNode, PCSNode* const pParent);

		// remove
		void Remove(PCSNode* const pInNode);

		void DetachSubtree(PCSNode* const pInNode);

		void RelocateSubtree(PCSNode* const pInNode, PCSNode* const pParent, PCSNode* const pPrev = nullptr);

		// get the root of the tree
		PCSNode* GetRoot() const;

		// get the tree stats info struct
		void GetInfo(Info& outInfo);

		// print the tree
		void Print() const;

		PCSNode* GetLastChild(PCSNode* const pInNode);

	private:
		// Special insert cases for first vs another node
		void InsertFirstNode(PCSNode* const pInNode);
		void InsertAnotherNode(PCSNode* const pParent, PCSNode* const pInNode);

		// Returns true if there are no nodes in the tree
		bool IsEmpty() const;

		// Burn an entire subtree by nullifying all the pointers and derementing the counter
		void BurnSubtree(PCSNode* const pInNode);

		void BurnNode(PCSNode* const pBurner);

		// Recalculate the current depth of the tree
		void RecalculateCurrentDepth();

		// Recursion helper for printing
		void PrintHelper(PCSNode const* const pCurr, int depth) const;

		// Statistics struct
		Info info;

		// The root node (big deal)
		PCSNode* pRoot;
	};
}

#endif

// ---  End of File ---
