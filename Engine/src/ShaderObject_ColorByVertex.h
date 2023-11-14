//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_COLOR_BY_VERTEX_H
#define SHADER_OBJECT_COLOR_BY_VERTEX_H

#include "ShaderObject.h"

class ShaderObject_ColorByVertex : public ShaderObject
{
public:

public:
	ShaderObject_ColorByVertex() = delete;
	ShaderObject_ColorByVertex(const ShaderObject_ColorByVertex &) = delete;
	ShaderObject_ColorByVertex &operator = (const ShaderObject_ColorByVertex &) = delete;
	~ShaderObject_ColorByVertex();

	ShaderObject_ColorByVertex( ShaderObject_ColorByVertex::Name _name);


	// ---------------------------------------------
	// 	   Data:
	// ---------------------------------------------


};

#endif

// --- End of File ---
