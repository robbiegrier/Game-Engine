//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GLTF_H
#define GLTF_H

#include "tiny_gltf.h"
#include "MeshLayout.h"
#include "meshData.h"
#include "GLTF_Helper.h"
#include "MathEngine.h"

using namespace tinygltf;
using namespace Azul;

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

struct skin_weight_joint_index
{
	skin_weight_joint_index() = default;
	~skin_weight_joint_index() = default;
	size_t	index;
	Vec4f	skinWeight;
	Vec4si	jointIndex;
};

struct LocalToGlobalIndex
{
	LocalToGlobalIndex(size_t _local, size_t _global, size_t _joint)
		: localIndex(_local), globalIndex(_global), JointIndex(_joint)
	{}

	size_t localIndex;   // new model
	size_t globalIndex;  // original model
	size_t JointIndex;   // Here for verification...should be same value for every index
};

struct BoneMesh
{
	std::string boneName;
	size_t      vertCount;
	size_t      jointIndex;
	Mat4        inverseMatrix;  // single - Inverse Matrix based on JointIndex
	bool        corrected;      // pos/norms corrected
	std::vector< LocalToGlobalIndex > l2gIndex;   // x
	std::vector<Vec3f> pos;
	std::vector<Vec3f> norms;
	std::vector<Vec2f> text_coord;
	std::vector<Vec3ui> index;                 // x
};

struct SkinMesh
{
	std::vector< Mat4 > invBone;
	std::vector< Vec4ui > joints;  // orig unsigned short--> unsigned int
	std::vector< Vec4f > weights;
};

class boneData;

class GLTF
{
public:

	static void DumpGLBHeader(const char* poBuff, unsigned int BuffSize);

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

	// In Chicken Bot Rigid Skeleton Converter
	static bool SetVBO_BONE(Model& gltfModel, const char* pKey, vboData& vbo, char* pBuffStart, unsigned int byteLength, unsigned int count);
	static bool SetVBO_BONE_index(Model& gltfModel, vboData& vbo, char* pBuffStart, unsigned int byteLength, unsigned int count);

	static bool SetCustomVBO(vboData& vbo, void* data, unsigned int sizeInBytes, unsigned int count, vboData::VBO_COMPONENT componentType, vboData::VBO_TYPE type, vboData::VBO_TARGET target);

	static bool LoadBinary(Model& model, const char* const pFileName);
	static bool OutputTrans(Model& model, size_t AccessorIndex, size_t NodeIndex, size_t FrameIndex, boneData* pBone);
	static bool OutputQuat(Model& model, size_t AccessorIndex, size_t NodeIndex, size_t FrameIndex, boneData* pBone);
	static void InsertBoundingSphereData(meshData& runModel);
	static void InsertBoundingBoxData(meshData& runModel);
};

#endif
