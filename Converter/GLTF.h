//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GLTF_H
#define GLTF_H

#include "tiny_gltf.h"
using namespace tinygltf;

#include "meshData.h"
#include "GLTF_Helper.h"

struct GLB_header
{
	uint32_t magic;
	uint32_t version;
	uint32_t length;
};

struct CHUNK_header
{
	uint32_t chunkLength;
	uint32_t chunkType;
	unsigned char chunkData[];
};

class GLTF
{
public:

	static bool Load(Model& model, const char* const pFileName);

	static bool GetGLBRawBuffer(char*& pBuff, unsigned int& BuffSize, const char* const pFileName);
	static bool GetGLBHeader(GLB_header& header, const char* const pBuff, unsigned int BuffSize);
	static bool GetRawJSON(char*& pJSON, unsigned int& JsonSize, const char* const pBuff, unsigned int BuffSize);
	static bool GetBinaryBuffPtr(char*& pBinaryBuff, unsigned int& BinaryBuffSize, const char* const pBuff, unsigned int BuffSize);

	static bool SetVBO(Model& gltfModel, unsigned int index, vboData& vbo, char* pBinaryBuff);
	static bool SetVBO_pos(Model& gltfModel, unsigned int index, vboData& vbo, char* pBinaryBuff);
	static bool SetVBO_uv(Model& gltfModel, unsigned int index, vboData& vbo, char* pBinaryBuff);
	static bool SetVBO_index(Model& gltfModel, unsigned int index, vboData& vbo, char* pBinaryBuff);
	static bool SetTexture(Model& gltfModel, unsigned int index, textureData& text, char* pBinaryBuff);
	static bool SetExternalTexture(const char* const filename, unsigned int index, textureData& text);
};

#endif

// --- End of File ---
