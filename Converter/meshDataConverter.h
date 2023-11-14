//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef MESH_DATA_CONVERTER_H
#define MESH_DATA_CONVERTER_H

#include "meshData.h"
#include "tiny_gltf.h"

class meshDataConverter
{
public:

	static meshData::RENDER_MODE GetMode(int GLTF_TINY_VAL);

};

#endif

// --- End of File ---
