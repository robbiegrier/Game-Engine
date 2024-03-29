//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GLTF.h"
#include "File.h"
#include "textureDataConverter.h"
#include "vboDataConverter.h"
#include "MathEngine.h"
#include "md5.h"
#include "stb_image.h"
#include "boneData.h"
#include "BoundingSphere.h"

using namespace Azul;

void GLTF::DumpGLBHeader(const char* poBuff, unsigned int BuffSize)
{
	GLB_header glbHeader;
	bool status = GLTF::GetGLBHeader(glbHeader, poBuff, BuffSize);
	assert(status);

	Trace::out("GLB Header:\n");
	Trace::out("\tmagic: 0x%x\n", glbHeader.magic);
	Trace::out("\tversion: 0x%x\n", glbHeader.version);
	Trace::out("\tlength: 0x%08x %d\n", glbHeader.length, glbHeader.length);
}

bool GLTF::Load(Model& model, const char* const pFileName)
{
	TinyGLTF loader;
	std::string err;
	std::string warn;

	bool status = loader.LoadBinaryFromFile(&model, &err, &warn, pFileName);

	if (!warn.empty())
	{
		Trace::out("Warn: %s\n", warn.c_str());
		assert(false);
	}

	if (!err.empty())
	{
		Trace::out("Err: %s\n", err.c_str());
		assert(false);
	}

	if (!status)
	{
		Trace::out2("Failed to parse glTF\n");
		assert(false);
	}

	return status;
}

bool GLTF::GetGLBRawBuffer(char*& pBuff, unsigned int& BuffSize, const char* const pFileName)
{
	File::Handle fh;
	File::Error err;

	err = File::Open(fh, pFileName, File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	err = File::Seek(fh, File::Position::END, 0);
	assert(err == File::Error::SUCCESS);

	DWORD FileSize(0);
	err = File::Tell(fh, FileSize);
	assert(err == File::Error::SUCCESS);

	// Cast it down to 32-bit size (limit 4GB)
	BuffSize = (unsigned int)FileSize;
	pBuff = new char[BuffSize]();
	assert(pBuff);

	err = File::Seek(fh, File::Position::BEGIN, 0);
	assert(err == File::Error::SUCCESS);

	err = File::Read(fh, pBuff, BuffSize);
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	assert(pBuff);
	assert(BuffSize > 0);

	return true;
}

bool GLTF::GetGLBHeader(GLB_header& header, const char* const pBuff, unsigned int BuffSize)
{
	GLB_header* pGlbHeader;

	pGlbHeader = (GLB_header*)&pBuff[0];
	assert(pGlbHeader);

	// boundary check
	assert((char*)(pGlbHeader + 1) < (pBuff + BuffSize));

	assert(pGlbHeader->version == 2);
	assert(pGlbHeader->magic == 0x46546C67);

	header = *pGlbHeader;

	return true;
}

bool GLTF::GetRawJSON(char*& pJSON, unsigned int& JsonSize, const char* const pBuff, unsigned int BuffSize)
{
	// BEGINNING...
	GLB_header* pGlbHeader;

	pGlbHeader = (GLB_header*)&pBuff[0];
	assert(pGlbHeader);

	// boundary check
	assert((char*)(pGlbHeader + 1) < (pBuff + BuffSize));

	// Now this next is CHUNK header
	CHUNK_header* pChunkHdr;
	pChunkHdr = (CHUNK_header*)(pGlbHeader + 1);

	assert(pChunkHdr->chunkType == 0x4E4F534A);
	JsonSize = pChunkHdr->chunkLength;
	pJSON = new char[JsonSize + 1]();
	memset(pJSON, 0, JsonSize + 1);
	memcpy(pJSON, (char*)pChunkHdr->chunkData, JsonSize);

	// boundary check - bottom of header
	assert((char*)(pChunkHdr + 1) < (pBuff + BuffSize));

	// boundary check - bottom of data
	assert(((char*)(pChunkHdr + 1) + pChunkHdr->chunkLength) < (pBuff + BuffSize));

	assert(pJSON);
	assert(JsonSize > 0);

	return true;
}

bool GLTF::GetBinaryBuffPtr(char*& pBinaryBuff, unsigned int& BinaryBuffSize, const char* const pBuff, unsigned int BuffSize)
{
	// BEGINNING...
	GLB_header* pGlbHeader;

	pGlbHeader = (GLB_header*)&pBuff[0];
	assert(pGlbHeader);

	// Now this next is CHUNK header - JSON
	CHUNK_header* pChunkHdr;
	pChunkHdr = (CHUNK_header*)(pGlbHeader + 1);

	assert(pChunkHdr);
	assert(pChunkHdr->chunkType == 0x4E4F534A);

	//Trace::out("CHUNK Header:\n");
	//Trace::out("\tType: 0x%08x \n", pChunkHdr->chunkType);
	//Trace::out("\tLength: 0x%x %d\n", pChunkHdr->chunkLength, pChunkHdr->chunkLength);

	// Now this next is CHUNK header - Binary
	pChunkHdr = (CHUNK_header*)((uint32_t)(pChunkHdr + 1) + pChunkHdr->chunkLength);

	assert(pChunkHdr->chunkType == 0x004E4942);

	//Trace::out("CHUNK Header:\n");
	//Trace::out("\tType: 0x%08x \n", pChunkHdr->chunkType);
	//Trace::out("\tLength: 0x%x %d\n", pChunkHdr->chunkLength, pChunkHdr->chunkLength);

	BinaryBuffSize = pChunkHdr->chunkLength;
	pBinaryBuff = (char*)pChunkHdr->chunkData;

	assert(((char*)(pChunkHdr + 1) + BinaryBuffSize) <= (pBuff + BuffSize));

	assert(pBinaryBuff);
	assert(BinaryBuffSize > 0);

	return true;
}

bool GLTF::SetVBO(Model& gltfModel, unsigned int index, vboData& vbo, char* pBinaryBuff)
{
	// Get the accessor, buffer view
	auto accessor = gltfModel.accessors[index];
	size_t buffIndex = (size_t)accessor.bufferView;
	auto bufferView = gltfModel.bufferViews[buffIndex];

	vbo.targetType = vboDataConverter::GetTarget(bufferView.target);
	vbo.componentType = vboDataConverter::GetComponent(accessor.componentType);
	vbo.vboType = vboDataConverter::GetType(accessor.type);

	vbo.attribIndex = index;
	vbo.count = accessor.count;
	vbo.dataSize = bufferView.byteLength;

	// poData
	assert(pBinaryBuff);
	char* pBuffStart(nullptr);

	// Start address
	pBuffStart = pBinaryBuff + bufferView.byteOffset + accessor.byteOffset;

	// in case there's data
	delete[] vbo.poData;

	vbo.poData = new unsigned char[vbo.dataSize]();
	assert(vbo.poData);
	memcpy_s(vbo.poData, vbo.dataSize, pBuffStart, vbo.dataSize);

	//float *p = (float *)vbo.poData;
	//Trace::out("%f %f %f\n", p[0], p[1], p[2]);
	return true;
}

bool GLTF::SetVBO_pos(Model& gltfModel, unsigned int index, vboData& vbo, char* pBinaryBuff)
{
	// Get the accessor, buffer view
	auto accessor = gltfModel.accessors[index];
	size_t buffIndex = (size_t)accessor.bufferView;
	auto bufferView = gltfModel.bufferViews[buffIndex];

	vbo.targetType = vboDataConverter::GetTarget(bufferView.target);
	vbo.componentType = vboDataConverter::GetComponent(accessor.componentType);
	vbo.vboType = vboDataConverter::GetType(accessor.type);

	vbo.attribIndex = index;
	vbo.count = accessor.count;
	vbo.dataSize = bufferView.byteLength;

	// poData
	assert(pBinaryBuff);
	char* pBuffStart(nullptr);

	// Start address
	pBuffStart = pBinaryBuff + bufferView.byteOffset + accessor.byteOffset;

	// in case there's data
	delete[] vbo.poData;

	vbo.poData = new unsigned char[vbo.dataSize]();
	assert(vbo.poData);
	memcpy_s(vbo.poData, vbo.dataSize, pBuffStart, vbo.dataSize);

	auto& mesh = gltfModel.meshes[0];

	Scale scale(1.f, 1.f, 1.f);
	Rot rotation(Special::Identity);
	Trans translation(0.f, 0.f, 0.f);

	tinygltf::Node& meshNode = gltfModel.nodes[0];

	for (const auto& node : gltfModel.nodes)
	{
		if (node.name == mesh.name)
		{
			meshNode = node;
			break;
		}
	}

	if (meshNode.rotation.size() > 3)
	{
		rotation = Rot(Quat((float)meshNode.rotation[0], (float)meshNode.rotation[1], (float)meshNode.rotation[2], (float)meshNode.rotation[3]));
	}

	if (meshNode.scale.size() > 2)
	{
		scale = Scale((float)meshNode.scale[0], (float)meshNode.scale[1], (float)meshNode.scale[2]);
	}

	if (meshNode.translation.size() > 2)
	{
		translation = Trans((float)meshNode.translation[0], (float)meshNode.translation[1], (float)meshNode.translation[2]);
	}

	Mat4 modelSpace = rotation * scale * translation;

	float* pSource = (float*)vbo.poData;
	for (unsigned int i = 0; i < accessor.count; i++)
	{
		Vec4 pSourceVec(*pSource, *(pSource + 1), *(pSource + 2), 1);

		pSourceVec *= modelSpace;

		*pSource = pSourceVec[x]; pSource++;
		*pSource = pSourceVec[y]; pSource++;
		*pSource = pSourceVec[z]; pSource++;

		//Trace::out("%f %f %f\n", pSourceVec[x], pSourceVec[y], pSourceVec[z]);
	}

	return true;
}

bool GLTF::SetVBO_uv(Model& gltfModel, unsigned int index, vboData& vbo, char* pBinaryBuff)
{
	// Get the accessor, buffer view
	auto accessor = gltfModel.accessors[index];
	size_t buffIndex = (size_t)accessor.bufferView;
	auto bufferView = gltfModel.bufferViews[buffIndex];
	vbo.targetType = vboDataConverter::GetTarget(bufferView.target);
	vbo.componentType = vboDataConverter::GetComponent(accessor.componentType);
	vbo.vboType = vboDataConverter::GetType(accessor.type);
	vbo.attribIndex = index;  // not used in index buffers
	vbo.count = accessor.count;
	vbo.dataSize = bufferView.byteLength;
	// poData
	assert(pBinaryBuff);
	char* pBuffStart(nullptr);
	// Start address
	pBuffStart = pBinaryBuff + bufferView.byteOffset + accessor.byteOffset;

	// in case there's data
	delete[] vbo.poData;

	struct TVec2 { float x; float y; };
	struct TVec4 { float x; float y; float z; float w; };

	if (vbo.vboType == vboData::VBO_TYPE::VEC4)
	{
		// Convert the unsigned short into unsigned int buffer
		// Engine will now be all unsigned int for index buffer
		TVec2* pData = new TVec2[vbo.count]();
		TVec2* pTmp = pData;
		TVec4* pVec4 = (TVec4*)pBuffStart;
		for (unsigned int i = 0; i < vbo.count; i++)
		{
			// convert it
			pTmp->x = pVec4->x;
			pTmp->y = pVec4->y;
			pTmp++;
			pVec4++;
		}
		// this is the output buffer
		vbo.poData = (unsigned char*)pData;
		// update the data
		vbo.dataSize = vbo.count * sizeof(TVec2);
		// update the component
		vbo.vboType = vboData::VBO_TYPE::VEC2;
	}
	else if (vbo.vboType == vboData::VBO_TYPE::VEC2)
	{
		vbo.poData = new unsigned char[vbo.dataSize]();
		assert(vbo.poData);
		memcpy_s(vbo.poData, vbo.dataSize, pBuffStart, vbo.dataSize);
	}
	else
	{
		assert(false);
	}
	return true;
}

bool GLTF::SetVBO_index(Model& gltfModel, unsigned int index, vboData& vbo, char* pBinaryBuff)
{
	auto accessor = gltfModel.accessors[index];

	tinygltf::BufferView bufferView;
	do {
		size_t buffIndex = (size_t)accessor.bufferView;
		bufferView = gltfModel.bufferViews[buffIndex];
		vbo.targetType = vboDataConverter::GetTarget(bufferView.target);
		vbo.componentType = vboDataConverter::GetComponent(accessor.componentType);
		vbo.vboType = vboDataConverter::GetType(accessor.type);
		vbo.attribIndex = index;  // not used in index buffers
		vbo.count = accessor.count;
		vbo.dataSize = bufferView.byteLength;
		index++;
	} while (vbo.vboType != vboData::VBO_TYPE::SCALAR);

	// poData
	assert(pBinaryBuff);
	char* pBuffStart(nullptr);

	// Start address
	pBuffStart = pBinaryBuff + bufferView.byteOffset + accessor.byteOffset;

	// in case there's data
	delete[] vbo.poData;

	if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_SHORT)
	{
		// Convert the unsigned short into unsigned int buffer
		// Engine will now be all unsigned int for index buffer
		unsigned int* pIndexData = new unsigned int[vbo.count]();
		unsigned int* pTmp = pIndexData;
		unsigned short* pShort = (unsigned short*)pBuffStart;

		for (unsigned int i = 0; i < vbo.count; i++)
		{
			*pTmp++ = *pShort++;
		}

		vbo.poData = (unsigned char*)pIndexData;
		vbo.dataSize = vbo.count * sizeof(unsigned int);
		vbo.componentType = vboData::VBO_COMPONENT::UNSIGNED_INT;
	}
	else if (vbo.componentType == vboData::VBO_COMPONENT::FLOAT)
	{
		unsigned int* pIndexData = new unsigned int[vbo.count]();
		unsigned int* pTmp = pIndexData;
		float* pFloat = (float*)pBuffStart;

		for (unsigned int i = 0; i < vbo.count; i++)
		{
			*pTmp++ = static_cast<unsigned int>(*pFloat++);
		}

		vbo.poData = (unsigned char*)pIndexData;
		vbo.dataSize = vbo.count * sizeof(unsigned int);
		vbo.componentType = vboData::VBO_COMPONENT::UNSIGNED_INT;
	}
	else if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_INT)
	{
		vbo.poData = new unsigned char[vbo.dataSize]();
		assert(vbo.poData);
		memcpy_s(vbo.poData, vbo.dataSize, pBuffStart, vbo.dataSize);
	}
	else
	{
		assert(false);
	}

	// Convert Vec2
	if (vbo.vboType == vboData::VBO_TYPE::VEC2)
	{
	}

	return true;
}

bool GLTF::SetTexture(Model& gltfModel, unsigned int index, textureData& text, char* pBinaryBuff)
{
	if (gltfModel.images.empty())
	{
		Trace::out("There are no images!\n");
		text.poData = new unsigned char[text.dataSize]();
		memset(text.poData, 0, text.dataSize);
		assert(text.poData);
		return false;
	}

	if (strcmp(gltfModel.images[index].mimeType.c_str(), "image/png") == 0)
	{
		//assert(gltfModel.textures[index].source == TINYGLTF_IMAGE_FORMAT_PNG);
		text.textType = textureData::TEXTURE_TYPE::PNG;
	}

	text.magFilter = textureData::TEXTURE_MAG_FILTER::DEFAULT;
	text.minFilter = textureData::TEXTURE_MIN_FILTER::DEFAULT;
	text.wrapS = textureData::TEXTURE_WRAP::DEFAULT;
	text.wrapT = textureData::TEXTURE_WRAP::DEFAULT;
	text.width = (unsigned int)gltfModel.images[index].width;
	text.height = (unsigned int)gltfModel.images[index].height;
	text.component = (unsigned int)gltfModel.images[index].component;
	text.bits = (unsigned int)gltfModel.images[index].bits;
	text.as_is = gltfModel.images[index].as_is;
	text.pixel_type = textureDataConverter::GetComponent(gltfModel.images[index].pixel_type);

	// pFileName
	unsigned int len = gltfModel.images[index].name.length();
	assert(len < text.FILE_NAME_SIZE);
	memcpy_s(text.pFileName,
		text.FILE_NAME_SIZE,
		gltfModel.images[index].name.c_str(),
		len);
	if (gltfModel.images[index].as_is == false)
	{
		// at this point the data is decompressed into a raw buffer gltfModel
		text.dataSize = gltfModel.images[index].image.size();
		//Trace::out("byteLength: %d \n", text.dataSize);
		// in case there's data
		delete[] text.poData;
		text.poData = new unsigned char[text.dataSize]();
		assert(text.poData);
		auto pStart = &gltfModel.images[index].image[0];
		memcpy_s(text.poData, text.dataSize, pStart, text.dataSize);
	}
	else
	{
		// export the texture file in compressed mode

		unsigned int buffViewIndex = (unsigned int)gltfModel.images[index].bufferView;
		auto buffView = gltfModel.bufferViews[buffViewIndex];

		text.dataSize = buffView.byteLength;

		char* pBuffStart = pBinaryBuff + buffView.byteOffset;

		Trace::out("byteLength: %d \n", text.dataSize);
		Trace::out("byteOffset: %d \n", buffView.byteOffset);

		// in case there's data
		delete[] text.poData;

		text.poData = new unsigned char[text.dataSize]();
		assert(text.poData);
		memcpy_s(text.poData, text.dataSize, pBuffStart, text.dataSize);
	}

	MD5BufferReduced(text.poData, text.dataSize, text.textureMD5Hash);

	return true;
}

bool GLTF::SetExternalTexture(const char* const filename, unsigned int index, textureData& text)
{
	static_cast<void>(index);

	int importImageWidth;
	int importImageHeight;
	int importImageComponents;
	constexpr static int importImageDesiredComponents = 4;

	stbi_uc* pImage = stbi_load(filename, &importImageWidth, &importImageHeight, &importImageComponents, importImageDesiredComponents);
	Trace::out("%s image:\n\n", filename);
	Trace::out("\tw: %d\n", importImageWidth);
	Trace::out("\th: %d\n", importImageHeight);
	Trace::out("\tc: %d\n", importImageComponents);
	Trace::out("\tdesiredC: %d\n\n", importImageDesiredComponents);

	text.textType = textureData::TEXTURE_TYPE::PNG;
	text.magFilter = textureData::TEXTURE_MAG_FILTER::DEFAULT;
	text.minFilter = textureData::TEXTURE_MIN_FILTER::DEFAULT;
	text.wrapS = textureData::TEXTURE_WRAP::DEFAULT;
	text.wrapT = textureData::TEXTURE_WRAP::DEFAULT;
	text.width = (unsigned int)importImageWidth;
	text.height = (unsigned int)importImageHeight;
	text.component = (unsigned int)importImageDesiredComponents;
	text.bits = (unsigned int)16;
	text.as_is = true;
	text.pixel_type = textureData::TEXTURE_COMPONENT::UNSIGNED_INT;

	memcpy_s(text.pFileName, text.FILE_NAME_SIZE, filename, strlen(filename));

	text.dataSize = text.component * text.width * text.height;
	Trace::out("byteLength: %u \n", text.dataSize);

	// in case there's data
	delete[] text.poData;
	text.poData = new unsigned char[text.dataSize]();
	assert(text.poData);
	auto pStart = (unsigned char*)pImage;
	memcpy_s(text.poData, text.dataSize, pStart, text.dataSize);

	MD5BufferReduced(text.poData, text.dataSize, text.textureMD5Hash);

	delete pImage;

	return true;
}

bool GLTF::SetVBO_BONE_index(Model& gltfModel, vboData& vbo, char* pBuffStart, unsigned int byteLength, unsigned int count)
{
	// Get the accessor, buffer view
	vbo.enabled = true;

	auto index = gltfModel.meshes[0].primitives[0].indices;
	assert(index > 0);

	auto accessor = gltfModel.accessors[(size_t)index];
	size_t buffIndex = (size_t)accessor.bufferView;
	auto bufferView = gltfModel.bufferViews[buffIndex];

	vbo.targetType = vboDataConverter::GetTarget(bufferView.target);

	// it was converted to unsigned int earlier...
	assert(vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_INT);
	vbo.componentType = vboData::VBO_COMPONENT::UNSIGNED_INT;

	vbo.vboType = vboDataConverter::GetType(accessor.type);

	vbo.count = count;
	vbo.dataSize = byteLength;

	assert(pBuffStart);

	// in case there's data
	delete[] vbo.poData;

	if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_SHORT)
	{
		// Convert the unsigned short into unsigned int buffer
		// Engine will now be all unsigned int for index buffer
		unsigned int* pIndexData = new unsigned int[vbo.count]();
		unsigned int* pTmp = pIndexData;
		unsigned short* pShort = (unsigned short*)pBuffStart;

		for (unsigned int i = 0; i < vbo.count; i++)
		{
			// convert it
			*pTmp++ = *pShort++;
		}

		// this is the output buffer
		vbo.poData = (unsigned char*)pIndexData;
		// update the data
		vbo.dataSize = vbo.count * sizeof(unsigned int);
		// update the component
		vbo.componentType = vboData::VBO_COMPONENT::UNSIGNED_INT;
	}
	else if (vbo.componentType == vboData::VBO_COMPONENT::UNSIGNED_INT)
	{
		vbo.poData = new unsigned char[vbo.dataSize]();
		assert(vbo.poData);
		memcpy_s(vbo.poData, vbo.dataSize, pBuffStart, vbo.dataSize);
	}
	else
	{
		assert(false);
	}

	return true;
}

bool GLTF::SetCustomVBO(vboData& vbo, void* pData, unsigned int sizeInBytes, unsigned int count, vboData::VBO_COMPONENT componentType, vboData::VBO_TYPE type, vboData::VBO_TARGET target)
{
	vbo.enabled = true;

	vbo.targetType = target;
	vbo.componentType = componentType;
	vbo.vboType = type;

	vbo.count = count;
	vbo.dataSize = sizeInBytes;

	vbo.poData = new unsigned char[vbo.dataSize]();
	assert(vbo.poData);
	memcpy_s(vbo.poData, vbo.dataSize, pData, vbo.dataSize);

	return true;
}

bool GLTF::SetVBO_BONE(Model& gltfModel, const char* pKey, vboData& vbo, char* pBuffStart, unsigned int byteLength, unsigned int count)
{
	// Get the accessor, buffer view
	vbo.enabled = false;

	assert(pKey);
	auto map = gltfModel.meshes[0].primitives[0].attributes;
	auto it = map.find(pKey);

	if (it != map.end())
	{
		// Get the accessor, buffer view
		vbo.enabled = true;

		auto index = it->second;

		auto accessor = gltfModel.accessors[(size_t)index];
		size_t buffIndex = (size_t)accessor.bufferView;
		auto bufferView = gltfModel.bufferViews[buffIndex];

		vbo.targetType = vboDataConverter::GetTarget(bufferView.target);
		vbo.componentType = vboDataConverter::GetComponent(accessor.componentType);
		vbo.vboType = vboDataConverter::GetType(accessor.type);

		vbo.count = count;
		vbo.dataSize = byteLength;

		assert(pBuffStart);

		// in case there's data
		delete[] vbo.poData;

		vbo.poData = new unsigned char[vbo.dataSize]();
		assert(vbo.poData);
		memcpy_s(vbo.poData, vbo.dataSize, pBuffStart, vbo.dataSize);
	}

	return vbo.enabled;
}

bool GLTF::LoadBinary(Model& model, const char* const pFileName)
{
	TinyGLTF loader;
	std::string err;
	std::string warn;

	bool status = loader.LoadBinaryFromFile(&model, &err, &warn, pFileName);

	if (!warn.empty())
	{
		Trace::out("Warn: %s\n", warn.c_str());
		assert(false);
	}

	if (!err.empty())
	{
		Trace::out("Err: %s\n", err.c_str());
		assert(false);
	}

	if (!status)
	{
		Trace::out2("Failed to parse glTF\n");
		assert(false);
	}

	return status;
}

bool GLTF::OutputQuat(Model& model, size_t AccessorIndex, size_t NodeIndex, size_t FrameIndex, boneData* pBone)
{
	static_cast<void>(NodeIndex);
	//Trace::out("Quat bone:%d Frame:%d \n", NodeIndex, FrameIndex);
	unsigned char* pBuff = (unsigned char*)&model.buffers[0].data[0];
	auto QuatAAccessor = model.accessors[AccessorIndex];
	auto QuatABuffView = model.bufferViews[(size_t)QuatAAccessor.bufferView];

	pBuff = pBuff + QuatABuffView.byteOffset;
	assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
	Vec4f* pVect4 = (Vec4f*)pBuff;
	for (size_t i = 0; i < QuatAAccessor.count; i++)
	{
		if (FrameIndex == i)
		{
			//Trace::out("pTmp->poBone[%d].Q = Quat(%ff,%ff,%ff,%ff);\n", NodeIndex,
			//	pVect4->x, pVect4->y, pVect4->z, pVect4->w);

			pBone->rotation[0] = pVect4->x;
			pBone->rotation[1] = pVect4->y;
			pBone->rotation[2] = pVect4->z;
			pBone->rotation[3] = pVect4->w;
		}
		pVect4++;
	}

	//Trace::out("\n");

	return true;
}

void GLTF::InsertBoundingSphereData(meshData& runModel)
{
	Sphere sphere;
	Vec3* pVerts = new Vec3[runModel.vertCount];

	for (unsigned int j = 0; j < runModel.vertCount; j++)
	{
		Vec3f* pVec3f = (Vec3f*)runModel.vbo_vert.poData;
		pVerts[j].set(pVec3f[j].x, pVec3f[j].y, pVec3f[j].z);
	}

	RitterSphere(sphere, pVerts, runModel.vertCount);

	runModel.boundingSphereRadius = sphere.rad;
	runModel.boundingSphereCenter[0] = sphere.cntr[x];
	runModel.boundingSphereCenter[1] = sphere.cntr[y];
	runModel.boundingSphereCenter[2] = sphere.cntr[z];

	delete[] pVerts;
}

bool GLTF::OutputTrans(Model& model, size_t AccessorIndex, size_t NodeIndex, size_t FrameIndex, boneData* pBone)
{
	static_cast<void>(NodeIndex);
	//Trace::out("Trans bone:%d Frame:%d \n", NodeIndex, FrameIndex);
	unsigned char* pBuff = (unsigned char*)&model.buffers[0].data[0];
	auto TransAccessor = model.accessors[AccessorIndex];
	auto TransBuffView = model.bufferViews[(size_t)TransAccessor.bufferView];

	pBuff = pBuff + TransBuffView.byteOffset;
	assert(TransAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(TransAccessor.type == TINYGLTF_TYPE_VEC3);
	Vec3f* pVect3 = (Vec3f*)pBuff;
	for (size_t i = 0; i < TransAccessor.count; i++)
	{
		if (FrameIndex == i)
		{
			//Trace::out("pTmp->poBone[%d].T = Vec3(%ff,%ff,%ff);  \n", NodeIndex,
			//	pVect3->x, pVect3->y, pVect3->z);

			pBone->translation[0] = pVect3->x;
			pBone->translation[1] = pVect3->y;
			pBone->translation[2] = pVect3->z;
		}
		pVect3++;
	}
	//Trace::out("\n");

	return true;
}