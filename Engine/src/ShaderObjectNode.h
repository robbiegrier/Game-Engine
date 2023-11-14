//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_OBJECT_NODE_H
#define SHADER_OBJECT_NODE_H

#include "DLink.h"
#include "ShaderObject.h"

class ShaderObjectNode : public DLink
{
public:
	ShaderObjectNode();
	ShaderObjectNode(const ShaderObject &) = delete;
	ShaderObjectNode &operator = (const ShaderObject &) = delete;
	virtual ~ShaderObjectNode();

	void Set(ShaderObject *pShaderObject);
	ShaderObject *GetShaderObject();

	virtual char *GetName() override;
	virtual void Wash() override;
	virtual bool Compare(DLink *pTargetNode) override;
	virtual void Dump() override;

private:
	void privClear();

	//--------------------------------------------------
	// Data
	//--------------------------------------------------
	ShaderObject *poShaderObject;
};

#endif

// --- End of File ---
