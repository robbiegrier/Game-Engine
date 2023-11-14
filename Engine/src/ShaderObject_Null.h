//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_NULL_H
#define SHADER_OBJECT_NULL_H

#include "ShaderObject.h"

class ShaderObject_Null : public ShaderObject
{
public:

public:
	ShaderObject_Null() = delete;
	ShaderObject_Null(const ShaderObject_Null &) = delete;
	ShaderObject_Null &operator = (const ShaderObject_Null &) = delete;
	virtual ~ShaderObject_Null();

	ShaderObject_Null( ShaderObject_Null::Name _name);


	// ---------------------------------------------
	// 	   Data:
	// ---------------------------------------------


};

#endif

// --- End of File ---
