//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_FLAT_TEXTURE_H
#define SHADER_OBJECT_FLAT_TEXTURE_H

#include "ShaderObject.h"

class ShaderObject_FlatTexture : public ShaderObject
{
public:

public:
	ShaderObject_FlatTexture() = delete;
	ShaderObject_FlatTexture(const ShaderObject_FlatTexture &) = delete;
	ShaderObject_FlatTexture &operator = (const ShaderObject_FlatTexture &) = delete;
	~ShaderObject_FlatTexture();

	ShaderObject_FlatTexture( ShaderObject_FlatTexture::Name _name);


	// ---------------------------------------------
	// 	   Data:
	// ---------------------------------------------


};

#endif

// --- End of File ---
