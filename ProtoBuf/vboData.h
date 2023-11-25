//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef VBO_DATA_H
#define VBO_DATA_H

#include "vboData.pb.h"

class vboData
{
public:
	enum class VBO_TARGET
	{
		ARRAY_BUFFER = 0,
		ELEMENT_ARRAY_BUFFER = 1,
		DEFAULT = ARRAY_BUFFER
	};

	enum class VBO_COMPONENT
	{
		BYTE = 0,
		UNSIGNED_BYTE = 1,
		SHORT = 2,
		UNSIGNED_SHORT = 3,
		INT = 4,
		UNSIGNED_INT = 5,
		FLOAT = 6,
		DOUBLE = 7,
		DEFAULT = FLOAT
	};

	enum class VBO_TYPE
	{
		VEC2 = 0,
		VEC3 = 1,
		VEC4 = 2,
		MAT2 = 3,
		MAT3 = 4,
		MAT4 = 5,
		SCALAR = 6,
		VECTOR = 7,
		MATRIX = 8,
		DEFAULT = VEC2
	};

public:
	vboData();
	vboData(const vboData&);
	vboData& operator = (const vboData&);
	~vboData();

	vboData(VBO_TARGET     targetType,
		VBO_COMPONENT  componetType,
		VBO_TYPE       vboType,
		unsigned int   attribIndex,
		unsigned int   count,
		unsigned int   dataSize,
		unsigned char* poData);

	// Serialization
	void Serialize(vboData_proto& out) const;
	void Deserialize(const vboData_proto& in);

	void Print(const char* const pName) const;

public:
	VBO_TARGET     targetType;
	VBO_COMPONENT  componentType;
	VBO_TYPE       vboType;
	unsigned int   attribIndex;
	unsigned int   count;
	unsigned int   dataSize;
	unsigned char* poData;
	bool enabled = true;
};

#endif

// --- End of File ---
