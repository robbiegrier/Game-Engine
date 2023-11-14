//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_OBJECT_NODE_MAN_H
#define SHADER_OBJECT_NODE_MAN_H 

#include "ManBase.h"
#include "ShaderObjectNode.h"
#include "ShaderObject.h"

class ShaderObjectNodeMan : public ManBase
{
	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
public:
	static void Create(int reserveNum = 0, int reserveGrow = 1);
	static void Destroy();

	static ShaderObjectNode *Add(ShaderObject *pShaderObject);
	static ShaderObjectNode *Find(ShaderObject::Name name);

	static void Remove(ShaderObjectNode *pNode);
	static void Dump();

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
private:
	static ShaderObjectNodeMan *privGetInstance();

	~ShaderObjectNodeMan();
	ShaderObjectNodeMan(int reserveNum, int reserveGrow);

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
protected:
	DLink *derivedCreateNode() override;

	//----------------------------------------------------------------------
	// Data: unique data for this manager 
	//----------------------------------------------------------------------
private:
	ShaderObjectNode *poNodeCompare;
	static ShaderObjectNodeMan *posInstance;

};

#endif

// --- End of File ---
