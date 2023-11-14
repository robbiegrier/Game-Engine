//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef VBO_DATA_CONVERTER_H
#define VBO_DATA_CONVERTER_H

#include "vboData.h"
#include "tiny_gltf.h"

class vboDataConverter
{
public:

	static vboData::VBO_TARGET GetTarget(int GLTF_TINY_VAL);
	static vboData::VBO_COMPONENT GetComponent(int GLTF_TINY_VAL);
	static vboData::VBO_TYPE GetType(int GLTF_TINY_VAL);

};

#endif

// --- End of File ---
