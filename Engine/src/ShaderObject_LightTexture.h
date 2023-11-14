//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef SHADER_OBJECT_LIGHT_TEXTURE_H
#define SHADER_OBJECT_LIGHT_TEXTURE_H

#include "ShaderObject.h"

class ShaderObject_LightTexture : public ShaderObject
{
public:

public:
	ShaderObject_LightTexture() = delete;
	ShaderObject_LightTexture(const ShaderObject_LightTexture &) = delete;
	ShaderObject_LightTexture &operator = (const ShaderObject_LightTexture &) = delete;
	~ShaderObject_LightTexture();

	ShaderObject_LightTexture( ShaderObject_LightTexture::Name _name);


	// ---------------------------------------------
	// 	   Data:
	// ---------------------------------------------


};

#endif

// --- End of File ---
