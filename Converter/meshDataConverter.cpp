//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "meshDataConverter.h"

meshData::RENDER_MODE meshDataConverter::GetMode(int GLTF_TINY_VAL)
{
	meshData::RENDER_MODE renderMode;

	switch(GLTF_TINY_VAL)
	{

	case TINYGLTF_MODE_POINTS:
		renderMode = meshData::RENDER_MODE::MODE_POINTS;
		break;

	case TINYGLTF_MODE_LINE:
		renderMode = meshData::RENDER_MODE::MODE_LINE;
		break;

	case TINYGLTF_MODE_LINE_LOOP:
		renderMode = meshData::RENDER_MODE::MODE_LINE_LOOP;
		break;

	case TINYGLTF_MODE_LINE_STRIP:
		renderMode = meshData::RENDER_MODE::MODE_LINE_STRIP;
		break;

	case TINYGLTF_MODE_TRIANGLES:
		renderMode = meshData::RENDER_MODE::MODE_TRIANGLES;
		break;

	case TINYGLTF_MODE_TRIANGLE_STRIP:
		renderMode = meshData::RENDER_MODE::MODE_TRIANGLE_STRIP;
		break;

	case TINYGLTF_MODE_TRIANGLE_FAN:
		renderMode = meshData::RENDER_MODE::MODE_TRIANGLE_FAN;
		break;


	default:
		assert(false);
		renderMode = meshData::RENDER_MODE::DEFAULT;
		break;

	}

	return renderMode;
}

// --- End of File ---
