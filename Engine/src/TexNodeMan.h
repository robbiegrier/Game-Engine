//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEX_NODE_MAN_H
#define TEX_NODE_MAN_H 

#include "ManBase.h"
#include "TexNode.h"
#include "TextureObject.h"

class TexNodeMan : public ManBase
{
	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
public:
	static void Create(int reserveNum = 0, int reserveGrow = 1);
	static void Destroy();

	static TexNode *Add(TextureObject::Name name, TextureObject *pTextureObject);
	static TexNode *Find(TextureObject::Name name);

	static void Remove(TexNode *pNode);
	static void Dump();

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
private:
	static TexNodeMan *privGetInstance();

	~TexNodeMan();
	TexNodeMan(int reserveNum, int reserveGrow);

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
protected:
	DLink *derivedCreateNode() override;

	//----------------------------------------------------------------------
	// Data: unique data for this manager 
	//----------------------------------------------------------------------
private:
	TexNode *poNodeCompare;
	static TexNodeMan *posInstance;

};

#endif

// --- End of File ---
