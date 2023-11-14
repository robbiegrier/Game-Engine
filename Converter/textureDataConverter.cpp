//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "textureDataConverter.h"

textureData::TEXTURE_COMPONENT textureDataConverter::GetComponent(int GLTF_TINY_VAL)
{
	textureData::TEXTURE_COMPONENT vboComponent;

	switch(GLTF_TINY_VAL)
	{

	case TINYGLTF_COMPONENT_TYPE_BYTE:
		vboComponent = textureData::TEXTURE_COMPONENT::BYTE;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
		vboComponent = textureData::TEXTURE_COMPONENT::BYTE;
		break;

	case TINYGLTF_COMPONENT_TYPE_SHORT:
		vboComponent = textureData::TEXTURE_COMPONENT::UNSIGNED_SHORT;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		vboComponent = textureData::TEXTURE_COMPONENT::UNSIGNED_SHORT;
		break;

	case TINYGLTF_COMPONENT_TYPE_INT:
		vboComponent = textureData::TEXTURE_COMPONENT::INT;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
		vboComponent = textureData::TEXTURE_COMPONENT::UNSIGNED_INT;
		break;

	case TINYGLTF_COMPONENT_TYPE_FLOAT:
		vboComponent = textureData::TEXTURE_COMPONENT::FLOAT;
		break;

	case TINYGLTF_COMPONENT_TYPE_DOUBLE:
		vboComponent = textureData::TEXTURE_COMPONENT::DOUBLE;
		break;

	default:
		assert(false);
		vboComponent = textureData::TEXTURE_COMPONENT::DEFAULT;
		break;

	}

	return vboComponent;
}

// --- End of File ---


