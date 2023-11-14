//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_CONST_COLOR_H
#define SHADER_OBJECT_CONST_COLOR_H

#include "ShaderObject.h"

class ShaderObject_ConstColor : public ShaderObject
{
public:

public:
	ShaderObject_ConstColor() = delete;
	ShaderObject_ConstColor(const ShaderObject_ConstColor &) = delete;
	ShaderObject_ConstColor &operator = (const ShaderObject_ConstColor &) = delete;
	~ShaderObject_ConstColor();

	ShaderObject_ConstColor( ShaderObject_ConstColor::Name _name);


	// ---------------------------------------------
	// 	   Data:
	// ---------------------------------------------


};

#endif

// --- End of File ---
