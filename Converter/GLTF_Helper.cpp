//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "tiny_gltf.h"
#include "GLTF_Helper.h"

StringThis::StringThis(int x)
{
	switch (x)
	{
	case TINYGLTF_TYPE_VEC2:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(VEC2));
		break;

	case TINYGLTF_TYPE_VEC3:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(VEC3));
		break;

	case TINYGLTF_TYPE_VEC4:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(VEC4));
		break;

	case TINYGLTF_TYPE_MAT2:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(MAT2));
		break;

	case TINYGLTF_TYPE_MAT3:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(MAT3));
		break;

	case TINYGLTF_TYPE_MAT4:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(MAT4));
		break;

	case TINYGLTF_TYPE_SCALAR:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(SCALAR));
		break;

	case TINYGLTF_TYPE_VECTOR:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(VECTOR));
		break;

	case TINYGLTF_TYPE_MATRIX:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(MATRIX));
		break;

	case TINYGLTF_TARGET_ARRAY_BUFFER:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ARRAY_BUFFER));
		break;

	case TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ELEMENT_ARRAY_BUFFER));
		break;

	case TINYGLTF_COMPONENT_TYPE_BYTE:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(BYTE));
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(UNSIGNED_BYTE));
		break;

	case TINYGLTF_COMPONENT_TYPE_SHORT:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(SHORT));
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(UNSIGNED_SHORT));
		break;

	case TINYGLTF_COMPONENT_TYPE_INT:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(INT));
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(UNSIGNED_INT));
		break;

	case TINYGLTF_COMPONENT_TYPE_FLOAT:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(FLOAT));
		break;

	case TINYGLTF_COMPONENT_TYPE_DOUBLE:
		strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(DOUBLE));
		break;

	default:
		assert(false);
	}
}

StringThis::operator char* ()
{
	return this->buffer;
}

// --- End of File ---