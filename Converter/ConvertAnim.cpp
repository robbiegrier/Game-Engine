#include "GLTF.h"
#include "GLTF_Helper.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "ConvertAnim.h"
#include "MeshLayout.h"
#include "animClipData.h"
#include "skeletonData.h"
#include "ProtoAzul.h"

using namespace Azul;
using namespace tinygltf;
using json = nlohmann::json;

void ConvertAnim(const char* const pFileName)
{
	Trace::out("Converting Animation: %s\n", pFileName);

	bool status;
	tinygltf::Model gltfModel;

	meshData  runModel;

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

	//Trace::out("Time: \n");
	//unsigned char* pBuff = (unsigned char*)&gltfModel.buffers[0].data[0];
	//auto TimeAccessor = gltfModel.accessors[0];
	//auto TimeBuffView = gltfModel.bufferViews[(size_t)TimeAccessor.bufferView];

	//pBuff = pBuff + TimeBuffView.byteOffset;
	//assert(TimeAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	//assert(TimeAccessor.type == TINYGLTF_TYPE_SCALAR);
	//float* pFloatData = (float*)pBuff;
	//Trace::out("start: %p\n", pBuff);
	//for (size_t i = 0; i < TimeAccessor.count; i++)
	//{
	//	Trace::out("\t %d: %f \n", i, *pFloatData++);
	//}
	//size_t len = ((size_t)pFloatData - (size_t)pBuff);
	//Trace::out("end: %p  len:%d\n", pFloatData, len);
	//assert(len == TimeBuffView.byteLength);
	//Trace::out("\n");

	auto numFrames = gltfModel.accessors[0].count;

	auto& joints = gltfModel.skins[0].joints;

	animClipData animClip;
	animClip.numBones = joints.size();

	animFrameData* pPrevFrame = nullptr;

	for (size_t i = 0; i < numFrames; i++)
	{
		size_t FrameIndex = i;

		//Trace::out("// Frame: %d\n", i);

		//Trace::out("pTmpX = new FrameBucket(); \n\
		//pTmpX->prevBucket = pTmp; \n\
		//pTmpX->nextBucket = nullptr; \n\
		//pTmpX->KeyTime = %d * AnimTime(AnimTime::Duration::FILM_24_FRAME); \n\
		//pTmpX->poBone = new Bone[(unsigned int)this->numBones]; \n\
		//pTmp->nextBucket = pTmpX; \n\
		//pTmp = pTmpX; \n", FrameIndex);

		animFrameData* pFrameData = new animFrameData();
		pFrameData->frameNumber = FrameIndex;

		if (pPrevFrame)
		{
			pPrevFrame->pNext = pFrameData;
		}
		else
		{
			animClip.pFramesHead = pFrameData;
		}

		pPrevFrame = pFrameData;

		boneData* pPrev = nullptr;

		//for (int nodeIndex = 0; nodeIndex < 12; nodeIndex++)
		for (int j = 0; j < (int)joints.size(); j++)
		{
			int transChannel = -1;
			int rotChannel = -1;

			int nodeIndex = joints[j];

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

			//Trace::out("// %s (%d)\n", gltfModel.nodes[nodeIndex].name.c_str(), nodeIndex);
			auto& node = gltfModel.nodes[nodeIndex];

			boneData* pBone = new boneData();
			pBone->boneNumber = j;

			if (pPrev)
			{
				pPrev->pNext = pBone;
			}
			else
			{
				pFrameData->pBonesHead = pBone;
			}

			pPrev = pBone;

			if (transChannel >= 0)
			{
				GLTF::OutputTrans(gltfModel, transChannel + 1, nodeIndex, FrameIndex, pBone);
			}
			else
			{
				if (node.translation.size() > 2)
				{
					//Trace::out("pTmp->poBone[%d].T = Vec3(%ff,%ff,%ff);\n", nodeIndex, (float)node.translation[0], (float)node.translation[1], (float)node.translation[2]);

					pBone->translation[0] = static_cast<float>(node.translation[0]);
					pBone->translation[1] = static_cast<float>(node.translation[1]);
					pBone->translation[2] = static_cast<float>(node.translation[2]);
				}
				else
				{
					//Trace::out("pTmp->poBone[%d].T = Vec3(0.f, 0.f, 0.f);\n");
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
					//Trace::out("pTmp->poBone[%d].Q = Quat(%ff,%ff,%ff,%ff);\n", nodeIndex, (float)node.rotation[0], (float)node.rotation[1], (float)node.rotation[2], (float)node.rotation[3]);

					pBone->rotation[0] = static_cast<float>(node.rotation[0]);
					pBone->rotation[1] = static_cast<float>(node.rotation[1]);
					pBone->rotation[2] = static_cast<float>(node.rotation[2]);
					pBone->rotation[3] = static_cast<float>(node.rotation[3]);

					//Trace::out("++ pTmp->poBone[%d].Q = Quat(%ff,%ff,%ff,%ff);\n", nodeIndex, pBone->rotation[0], pBone->rotation[1], pBone->rotation[2], pBone->rotation[3]);
				}
				else
				{
					//Trace::out("pTmp->poBone[%d].Q = Quat(Special::Identity);\n");

					Quat identity = Quat(Special::Identity);
					pBone->rotation[0] = identity[x];
					pBone->rotation[1] = identity[y];
					pBone->rotation[2] = identity[z];
					pBone->rotation[3] = identity[w];
				}
			}

			//Trace::out("pTmp->poBone[%d].S = Vec3(1.000000f, 1.000000f, 1.000000f);\n", nodeIndex);

			pBone->scale[0] = 1.f;
			pBone->scale[1] = 1.f;
			pBone->scale[2] = 1.f;
		}

		//Trace::out("// =====================\n");
	}

	animClipData_proto acProto;
	animClip.Serialize(acProto);

	std::string animName = std::string(pFileName);
	const char* filename = animName.replace(animName.end() - 4, animName.end(), ".anim.proto.azul").c_str();
	ProtoAzul::WriteProtoFile(acProto, filename);
	//animClip.Print("ANIM CLIP");

	Trace::out("  => Created %s\n", filename);

	delete[] poBuff;
	delete[] poJSON;
}

void ConvertSkeleton(const char* const pFileName)
{
	Trace::out("Converting Skeleton: %s\n", pFileName);

	bool status;
	tinygltf::Model gltfModel;

	meshData  runModel;

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

	auto& joints = gltfModel.skins[0].joints;

	skeletonData skData(joints.size());

	int storageIndex = 0;

	for (int i = 0; i < (int)joints.size(); i++)
	{
		int joint = joints[i];
		auto& node = gltfModel.nodes[joint];

		auto jointName = node.name;
		auto jointIndex = i;
		auto jointParentIndex = -1;

		for (int j = 0; j < (int)gltfModel.nodes.size(); j++)
		{
			auto& n = gltfModel.nodes[j];

			for (auto c : n.children)
			{
				if (c == joint)
				{
					auto find = std::find(joints.begin(), joints.end(), j);
					if (find != joints.end())
					{
						jointParentIndex = find - joints.begin();
					}
					break;
				}
			}
		}

		//Trace::out("\t{ %d, %d, \"%s\" },\n", jointIndex, jointParentIndex, jointName.c_str());

		skData.jointIndicies[storageIndex] = jointIndex;
		skData.jointParentIndicies[storageIndex] = jointParentIndex;
		skData.SetJointName(storageIndex, jointName.c_str());
		storageIndex++;
	}

	skData.SortByParentTree();

	skeletonData_proto skData_proto;
	skData.Serialize(skData_proto);
	//skData.Print("SKELETON");
	std::string skeletonName = std::string(pFileName);
	const char* skFileName = skeletonName.replace(skeletonName.end() - 4, skeletonName.end(), ".skeleton.proto.azul").c_str();
	ProtoAzul::WriteProtoFile(skData_proto, skFileName);

	Trace::out("  => Created %s\n", skFileName);

	delete[] poBuff;
	delete[] poJSON;
}