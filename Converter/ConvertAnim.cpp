#include "GLTF.h"
#include "GLTF_Helper.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "ConvertAnim.h"
#include "MeshLayout.h"
#include "animClipData.h"

using namespace Azul;
using namespace tinygltf;
using json = nlohmann::json;

void WriteAnimClipDataToFile(const animClipData_proto& source, const char* const filename)
{
	File::Handle fh;

	File::Error err = File::Open(fh, filename, File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	source.SerializeToString(&strOut);

	File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);
}

void ConvertAnim(const char* const pFileName)
{
	bool status;
	tinygltf::Model gltfModel;

	meshData  runModel;

	Trace::out("%12s ", pFileName);

	unsigned int len2 = strlen(pFileName);
	char* pTmp = new char[len2 + 1]();
	memset(pTmp, 0, len2);
	memcpy(pTmp, pFileName, len2 - strlen(".glb"));

	std::string BaseName = pTmp;
	delete[] pTmp;

	status = GLTF::LoadBinary(gltfModel, pFileName);
	assert(status);

	char* poBuff = nullptr;
	unsigned int BuffSize(0);

	status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pFileName);
	assert(status);

	GLB_header glbHeader;
	status = GLTF::GetGLBHeader(glbHeader, poBuff, BuffSize);
	assert(status);

	char* poJSON = nullptr;
	unsigned int JsonSize(0);
	status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize);
	assert(status);

	char* pBinaryBuff = nullptr;
	unsigned int BinaryBuffSize = 0;
	status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize);
	assert(status);

	{
		Trace::out("Time: \n");
		unsigned char* pBuff = (unsigned char*)&gltfModel.buffers[0].data[0];
		auto TimeAccessor = gltfModel.accessors[0];
		auto TimeBuffView = gltfModel.bufferViews[(size_t)TimeAccessor.bufferView];

		pBuff = pBuff + TimeBuffView.byteOffset;
		assert(TimeAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(TimeAccessor.type == TINYGLTF_TYPE_SCALAR);
		float* pFloatData = (float*)pBuff;
		Trace::out("start: %p\n", pBuff);
		for (size_t i = 0; i < TimeAccessor.count; i++)
		{
			Trace::out("\t %d: %f \n", i, *pFloatData++);
		}
		size_t len = ((size_t)pFloatData - (size_t)pBuff);
		Trace::out("end: %p  len:%d\n", pFloatData, len);
		assert(len == TimeBuffView.byteLength);
		Trace::out("\n");
	}

	auto numFrames = gltfModel.accessors[0].count;

	animClipData animClip;

	for (size_t i = 0; i < numFrames; i++)
	{
		size_t FrameIndex = i;

		Trace::out("// Frame: %d\n", i);

		Trace::out("pTmpX = new FrameBucket(); \n\
		pTmpX->prevBucket = pTmp; \n\
		pTmpX->nextBucket = nullptr; \n\
		pTmpX->KeyTime = %d * AnimTime(AnimTime::Duration::FILM_24_FRAME); \n\
		pTmpX->poBone = new Bone[(unsigned int)this->numBones]; \n\
		pTmp->nextBucket = pTmpX; \n\
		pTmp = pTmpX; \n", FrameIndex);

		animFrameData* pFrameData = new animFrameData();
		pFrameData->frameNumber = FrameIndex;

		for (int nodeIndex = 0; nodeIndex < 12; nodeIndex++)
		{
			int transChannel = -1;
			int rotChannel = -1;

			for (size_t c = 0; c < gltfModel.animations[0].channels.size(); c++)
			{
				auto& channel = gltfModel.animations[0].channels[c];
				if (channel.target_node == nodeIndex)
				{
					if (channel.target_path == "translation")
					{
						//Trace::out("Node %d's T-channel is channel -> %d\n", nodeIndex, c);
						transChannel = c;
					}
					else if (channel.target_path == "rotation")
					{
						//Trace::out("Node %d's Q-channel is channel -> %d\n", nodeIndex, c);
						rotChannel = c;
					}
				}
			}

			Trace::out("// %s (%d)\n", gltfModel.nodes[nodeIndex].name.c_str(), nodeIndex);
			auto& node = gltfModel.nodes[nodeIndex];

			boneData* pBone = new boneData();
			pBone->boneNumber = nodeIndex;

			if (transChannel >= 0)
			{
				GLTF::OutputTrans(gltfModel, transChannel + 1, nodeIndex, FrameIndex, pBone);
			}
			else
			{
				if (node.translation.size() > 2)
				{
					Trace::out("pTmp->poBone[%d].T = Vec3(%ff,%ff,%ff);\n", nodeIndex, (float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);

					pBone->translation[0] = static_cast<float>(node.translation[0]);
					pBone->translation[1] = static_cast<float>(node.translation[1]);
					pBone->translation[2] = static_cast<float>(node.translation[2]);
				}
				else
				{
					Trace::out("pTmp->poBone[%d].T = Vec3(0.f, 0.f, 0.f);\n");
					pBone->translation[0] = 0.f;
					pBone->translation[1] = 0.f;
					pBone->translation[2] = 0.f;
				}
			}

			if (rotChannel >= 0)
			{
				GLTF::OutputQuat(gltfModel, rotChannel + 1, nodeIndex, FrameIndex, pBone);
			}
			else
			{
				if (node.rotation.size() > 3)
				{
					Trace::out("pTmp->poBone[%d].Q = Quat(%ff,%ff,%ff,%ff);\n", nodeIndex, (float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);

					pBone->rotation[0] = static_cast<float>(node.rotation[0]);
					pBone->rotation[1] = static_cast<float>(node.rotation[1]);
					pBone->rotation[2] = static_cast<float>(node.rotation[2]);
					pBone->rotation[3] = static_cast<float>(node.rotation[3]);

					Trace::out("++ pTmp->poBone[%d].Q = Quat(%ff,%ff,%ff,%ff);\n", nodeIndex, pBone->rotation[0], pBone->rotation[1], pBone->rotation[2], pBone->rotation[3]);
				}
				else
				{
					Trace::out("pTmp->poBone[%d].Q = Quat(Special::Identity);\n");

					Quat identity = Quat(Special::Identity);
					pBone->rotation[0] = identity[x];
					pBone->rotation[1] = identity[y];
					pBone->rotation[2] = identity[z];
					pBone->rotation[3] = identity[w];
				}
			}

			Trace::out("pTmp->poBone[%d].S = Vec3(1.000000f, 1.000000f, 1.000000f);\n", nodeIndex);

			pBone->scale[0] = 1.f;
			pBone->scale[1] = 1.f;
			pBone->scale[2] = 1.f;

			pFrameData->bones.push_back(pBone);
		}

		animClip.frames.push_back(pFrameData);

		Trace::out("// =====================\n");
	}

	animClipData_proto acProto;
	animClip.Serialize(acProto);

	std::string animName = std::string(pFileName);
	const char* filename = animName.replace(animName.end() - 4, animName.end(), ".anim.proto.azul").c_str();
	WriteAnimClipDataToFile(acProto, filename);

	animClip.Print("shot up");

	Trace::out("\n\nSkeleton Data:\n");

	auto& joints = gltfModel.skins[0].joints;

	for (int joint : joints)
	{
		auto& node = gltfModel.nodes[joint];

		auto jointName = node.name;
		auto jointIndex = joint;
		auto jointParentIndex = -1;

		for (int i = 0; i < (int)gltfModel.nodes.size(); i++)
		{
			auto& n = gltfModel.nodes[i];

			for (auto c : n.children)
			{
				if (c == joint)
				{
					if (std::find(joints.begin(), joints.end(), i) != joints.end())
					{
						jointParentIndex = i;
					}
					break;
				}
			}
		}

		Trace::out("\t{ %d, %d, \"%s\" },\n", jointIndex, jointParentIndex, jointName.c_str());
	}

	delete[] poBuff;
	delete[] poJSON;
}