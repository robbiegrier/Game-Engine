//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Verify.h"
#include "meshData.h"
#include "EngineUtils.h"

#define CREATE_MESH_DATA meshData_proto mB_proto;\
	mB_proto.ParseFromString(Azul::EngineUtils::FileToString("space_frigate.proto.azul"));\
	meshData mB;\
	mB.Deserialize(mB_proto);

void Verify::GetVertsCopy(unsigned char*& p, unsigned int& numBytes)
{
	CREATE_MESH_DATA;
	unsigned char* pBuff = new unsigned char[mB.vbo_vert.dataSize]();
	memcpy(pBuff, mB.vbo_vert.poData, mB.vbo_vert.dataSize);
	p = pBuff;
	numBytes = mB.vbo_vert.dataSize;
}

void Verify::GetNormsCopy(unsigned char*& p, unsigned int& numBytes)
{
	CREATE_MESH_DATA;
	unsigned char* pBuff = new unsigned char[mB.vbo_norm.dataSize]();
	memcpy(pBuff, mB.vbo_norm.poData, mB.vbo_norm.dataSize);
	p = pBuff;
	numBytes = mB.vbo_norm.dataSize;
}

void Verify::GetUVsCopy(unsigned char*& p, unsigned int& numBytes)
{
	CREATE_MESH_DATA;
	unsigned char* pBuff = new unsigned char[mB.vbo_uv.dataSize]();
	memcpy(pBuff, mB.vbo_uv.poData, mB.vbo_uv.dataSize);
	p = pBuff;
	numBytes = mB.vbo_uv.dataSize;
}

void Verify::GetIndexCopy(unsigned char*& p, unsigned int& numBytes)
{
	CREATE_MESH_DATA;
	unsigned char* pBuff = new unsigned char[mB.vbo_index.dataSize]();
	memcpy(pBuff, mB.vbo_index.poData, mB.vbo_index.dataSize);
	p = pBuff;
	numBytes = mB.vbo_index.dataSize;
}

void Verify::GetTextureBufferCopy(unsigned char*& p, unsigned int& numBytes)
{
	CREATE_MESH_DATA;
	unsigned char* pBuff = new unsigned char[mB.text_color.dataSize]();
	memcpy(pBuff, mB.text_color.poData, mB.text_color.dataSize);
	p = pBuff;
	numBytes = mB.text_color.dataSize;
}

Verify::Model_stats Verify::GetModelStats()
{
	Model_stats stats;
	stats = { 0 };

	CREATE_MESH_DATA;

	stats.numTris = mB.triCount;
	stats.numVerts = mB.vertCount;
	stats.TextureHeight = mB.text_color.height;
	stats.TextureWidth = mB.text_color.width;

	return stats;
}

// --- End of File ---