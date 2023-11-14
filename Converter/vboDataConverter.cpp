//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "vboDataConverter.h"

vboData::VBO_TARGET vboDataConverter::GetTarget(int GLTF_TINY_VAL)
{
	vboData::VBO_TARGET vboTarget;

	switch(GLTF_TINY_VAL)
	{
	case TINYGLTF_TARGET_ARRAY_BUFFER:
		vboTarget = vboData::VBO_TARGET::ARRAY_BUFFER;
		break;

	case TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER:
		vboTarget = vboData::VBO_TARGET::ELEMENT_ARRAY_BUFFER;
		break;

	default:
		assert(false);
		vboTarget = vboData::VBO_TARGET::DEFAULT;
		break;

	}

	return vboTarget;
}


vboData::VBO_COMPONENT vboDataConverter::GetComponent(int GLTF_TINY_VAL)
{
	vboData::VBO_COMPONENT vboComponent;

	switch(GLTF_TINY_VAL)
	{

	case TINYGLTF_COMPONENT_TYPE_BYTE:
		vboComponent = vboData::VBO_COMPONENT::BYTE;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
		vboComponent = vboData::VBO_COMPONENT::BYTE;
		break;

	case TINYGLTF_COMPONENT_TYPE_SHORT:
		vboComponent = vboData::VBO_COMPONENT::UNSIGNED_SHORT;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		vboComponent = vboData::VBO_COMPONENT::UNSIGNED_SHORT;
		break;

	case TINYGLTF_COMPONENT_TYPE_INT:
		vboComponent = vboData::VBO_COMPONENT::INT;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
		vboComponent = vboData::VBO_COMPONENT::UNSIGNED_INT;
		break;

	case TINYGLTF_COMPONENT_TYPE_FLOAT:
		vboComponent = vboData::VBO_COMPONENT::FLOAT;
		break;

	case TINYGLTF_COMPONENT_TYPE_DOUBLE:
		vboComponent = vboData::VBO_COMPONENT::DOUBLE;
		break;

	default:
		assert(false);
		vboComponent = vboData::VBO_COMPONENT::DEFAULT;
		break;

	}

	return vboComponent;
}

vboData::VBO_TYPE vboDataConverter::GetType(int GLTF_TINY_VAL)
{
	vboData::VBO_TYPE vboType;

	switch(GLTF_TINY_VAL)
	{
	case TINYGLTF_TYPE_VEC2:
		vboType = vboData::VBO_TYPE::VEC2;
		break;

	case TINYGLTF_TYPE_VEC3:
		vboType = vboData::VBO_TYPE::VEC3;
		break;

	case TINYGLTF_TYPE_VEC4:
		vboType = vboData::VBO_TYPE::VEC4;
		break;

	case TINYGLTF_TYPE_MAT2:
		vboType = vboData::VBO_TYPE::MAT2;
		break;

	case TINYGLTF_TYPE_MAT3:
		vboType = vboData::VBO_TYPE::MAT3;
		break;

	case TINYGLTF_TYPE_MAT4:
		vboType = vboData::VBO_TYPE::MAT4;
		break;

	case TINYGLTF_TYPE_SCALAR:
		vboType = vboData::VBO_TYPE::SCALAR;
		break;

	case TINYGLTF_TYPE_VECTOR:
		vboType = vboData::VBO_TYPE::VECTOR;
		break;

	case TINYGLTF_TYPE_MATRIX:
		vboType = vboData::VBO_TYPE::MATRIX;
		break;

	default:
		assert(false);
		vboType = vboData::VBO_TYPE::DEFAULT;
		break;
	}

	return vboType;

}

// --- End of File ---


