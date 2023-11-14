//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "TexNodeMan.h"
#include "TextureNull.h"
#include "ManBase.h"
#include "DLinkMan.h"

TexNodeMan *TexNodeMan::posInstance = nullptr;

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
TexNodeMan::TexNodeMan(int reserveNum, int reserveGrow)
	: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
{
	// Preload the reserve
	this->proFillReservedPool(reserveNum);

	// initialize derived data here
	TextureObject *pTextureObject = new TextureNull();
	assert(pTextureObject);
	this->poNodeCompare = new TexNode();
	assert(this->poNodeCompare);
	this->poNodeCompare->Set(TextureObject::Name::NullTexture, pTextureObject);
}

TexNodeMan::~TexNodeMan()
{
	delete this->poNodeCompare;
	this->poNodeCompare = nullptr;

	// iterate through the list and delete
	Iterator *pIt = this->baseGetActiveIterator();

	DLink *pNode = pIt->First();

	// Walk through the nodes
	while (!pIt->IsDone())
	{
		TexNode *pDeleteMe = (TexNode *)pIt->Curr();
		pNode = pIt->Next();
		delete pDeleteMe;
	}

	pIt = this->baseGetReserveIterator();

	pNode = pIt->First();

	// Walk through the nodes
	while (!pIt->IsDone())
	{
		TexNode *pDeleteMe = (TexNode *)pIt->Curr();
		pNode = pIt->Next();
		delete pDeleteMe;
	}
}

//----------------------------------------------------------------------
// Static Methods
//----------------------------------------------------------------------
void TexNodeMan::Create(int reserveNum, int reserveGrow)
{
	// make sure values are ressonable 
	assert(reserveNum >= 0);
	assert(reserveGrow > 0);

	// initialize the singleton here
	assert(posInstance == nullptr);

	// Do the initialization
	if (posInstance == nullptr)
	{
		posInstance = new TexNodeMan(reserveNum, reserveGrow);
	}
}

void TexNodeMan::Destroy()
{
	TexNodeMan *pMan = TexNodeMan::privGetInstance();
	assert(pMan != nullptr);
	AZUL_UNUSED_VAR(pMan);

	delete TexNodeMan::posInstance;
	TexNodeMan::posInstance = nullptr;
}

TexNode *TexNodeMan::Add(TextureObject::Name name, TextureObject *pTextureObject)
{
	TexNodeMan *pMan = TexNodeMan::privGetInstance();

	assert(pTextureObject);

	TexNode *pNode = (TexNode *)pMan->baseAddToFront();
	assert(pNode != nullptr);

	// Initialize the date
	pNode->Set(name, pTextureObject);

	return pNode;
}

TexNode *TexNodeMan::Find(TextureObject::Name _name)
{
	TexNodeMan *pMan = TexNodeMan::privGetInstance();
	assert(pMan != nullptr);

	// Compare functions only compares two Nodes

	// So:  Use the Compare Node - as a reference
	//      use in the Compare() function
	TextureObject *pTextureObject = pMan->poNodeCompare->GetTextureObject();
	assert(pTextureObject);

	pTextureObject->name = _name;

	TexNode *pData = (TexNode *)pMan->baseFind(pMan->poNodeCompare);
	return pData;
}

void TexNodeMan::Remove(TexNode *pNode)
{
	assert(pNode != nullptr);

	TexNodeMan *pMan = TexNodeMan::privGetInstance();
	assert(pMan != nullptr);

	pMan->baseRemove(pNode);
}

void TexNodeMan::Dump()
{
	TexNodeMan *pMan = TexNodeMan::privGetInstance();
	assert(pMan != nullptr);

	pMan->baseDump();
}

//----------------------------------------------------------------------
// Private methods
//----------------------------------------------------------------------
TexNodeMan *TexNodeMan::privGetInstance()
{
	// Safety - this forces users to call Create() first before using class
	assert(posInstance != nullptr);

	return posInstance;
}

//----------------------------------------------------------------------
// Override Abstract methods
//----------------------------------------------------------------------
DLink *TexNodeMan::derivedCreateNode()
{
	DLink *pNodeBase = new TexNode();
	assert(pNodeBase != nullptr);

	return pNodeBase;
}


// --- End of File ---




