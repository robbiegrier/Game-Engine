//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ShaderObjectNode.h"
#include "StringThis.h"

ShaderObjectNode::ShaderObjectNode()
    : DLink(),
    poShaderObject(nullptr)
{
    this->privClear();
}

ShaderObjectNode::~ShaderObjectNode()
{
    this->privClear();
}

void ShaderObjectNode::Set(ShaderObject *pShaderObject)
{
   assert(pShaderObject);
   this->poShaderObject = pShaderObject;

   this->poShaderObject->SetName(pShaderObject->name);
}

ShaderObject * ShaderObjectNode::GetShaderObject()
{
    return this->poShaderObject;
}

void ShaderObjectNode::privClear()
{
    if(this->poShaderObject)
    {
        delete this->poShaderObject;
    }
    this->poShaderObject = nullptr;
}

char * ShaderObjectNode::GetName()
{
    char *pName = nullptr;
    if(this->poShaderObject)
    {
        pName = this->poShaderObject->GetName();
    }

    return pName;
}

void ShaderObjectNode::Wash()
{
    this->privClear();
}

bool ShaderObjectNode::Compare(DLink *pTargetNode)
{
    // This is used in ManBase.Find() 
    assert(pTargetNode);

    ShaderObjectNode *pDataB = (ShaderObjectNode *) pTargetNode;

    bool status = false;

    assert(pDataB->poShaderObject);
    assert(this->poShaderObject);

    if(this->poShaderObject->name == pDataB->poShaderObject->name)
    {
        status = true;
    }

    return status;
}

void ShaderObjectNode::Dump()
{
    Trace::out("      ShaderObjectNode(%p)\n", this);

    // Data:
    if(this->poShaderObject)
    {
        Trace::out("      ShaderObject(%p) \n", this->poShaderObject);
        Trace::out("      Name: %s \n", StringMe(this->poShaderObject->name));
    }
    else
    {
        Trace::out("      Name: %s \n", "null");
    }

    DLink::Dump();
}


// --- End of File ---
