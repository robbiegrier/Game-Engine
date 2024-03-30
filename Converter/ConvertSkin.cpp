#include "GLTF.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "MathEngine.h"
#include "ChickenBot.h"
#include "ConvertSkin.h"
#include "azulModel.h"
#include "BoundingSphere.h"
#include "ProtoAzul.h"

using namespace tinygltf;
using json = nlohmann::json;

namespace Azul
{
	void WriteAndVerifyRuntimeModel(azulModel& azulRunModel, const char* const filenameIn)
	{
		std::string meshName = std::string(filenameIn);
		const char* filename = meshName.replace(meshName.end() - 4, meshName.end(), ".skin.proto.azul").c_str();

		azulModel_proto mA_proto;
		azulRunModel.Serialize(mA_proto);
		ProtoAzul::WriteProtoFile(mA_proto, filename);

		Trace::out("  => Created %s\n", filename);
	}

	void ConvertSkin(const char* const pName)
	{
		Trace::out("Converting Skin Mesh: %s\n", pName);

		bool status;
		char* poBuff = nullptr;
		unsigned int BuffSize(0);
		char* pBinaryBuff = nullptr;
		unsigned int BinaryBuffSize = 0;
		char* poJSON = nullptr;
		unsigned int JsonSize = 0u;

		tinygltf::Model gltfModel;
		azulModel azulRunModel;
		azulRunModel.converterVersion = 3;

		status = GLTF::LoadBinary(gltfModel, pName); assert(status);
		status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pName); assert(status);
		status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize); assert(status);
		status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize);

		for (int meshIndex = 0; meshIndex < (int)gltfModel.meshes.size(); meshIndex++)
		{
			SkinMesh Skin0;
			meshData& runModel = azulRunModel.meshes[meshIndex];
			tinygltf::Mesh& gltfMesh = gltfModel.meshes[meshIndex];
			tinygltf::Primitive& primitive = gltfMesh.primitives[0];
			tinygltf::Material& gltfMaterial = gltfModel.materials[primitive.material];
			auto& attributes = primitive.attributes;

			if (primitive.material < 0)
			{
				tinygltf::Material mat;
				mat.name = pName;
				gltfModel.materials.push_back(mat);
				primitive.material = 0;
			}

			memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, gltfMaterial.name.c_str(), gltfMaterial.name.length());

			const auto& PosAccessor = gltfModel.accessors[attributes["POSITION"]];
			const auto& JointAccessor = gltfModel.accessors[attributes["JOINTS_0"]];
			const auto& WeightAccessor = gltfModel.accessors[attributes["WEIGHTS_0"]];
			const auto& InverseAccessor = gltfModel.accessors[gltfModel.skins[0].inverseBindMatrices];

			unsigned char* pBuff = (unsigned char*)&gltfModel.buffers[0].data[0];
			unsigned char* pJointBuff = pBuff + gltfModel.bufferViews[JointAccessor.bufferView].byteOffset;
			unsigned char* pWeightBuff = pBuff + gltfModel.bufferViews[WeightAccessor.bufferView].byteOffset;
			unsigned char* pInverseMatrixBuff = pBuff + gltfModel.bufferViews[InverseAccessor.bufferView].byteOffset;

			assert(JointAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT);
			assert(WeightAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			assert(InverseAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			assert(JointAccessor.type == TINYGLTF_TYPE_VEC4);
			assert(WeightAccessor.type == TINYGLTF_TYPE_VEC4);
			assert(InverseAccessor.type == TINYGLTF_TYPE_MAT4);
			assert(WeightAccessor.count == JointAccessor.count);

			Mat4* pMat4 = (Mat4*)pInverseMatrixBuff;
			for (size_t i = 0; i < InverseAccessor.count; i++)
			{
				assert(pMat4);
				Skin0.invBone.push_back(pMat4[i]);
			}

			Vec4si* pJointVect4 = (Vec4si*)pJointBuff;
			Vec4f* pWeightVect4 = (Vec4f*)pWeightBuff;

			for (size_t i = 0; i < PosAccessor.count; i++)
			{
				Vec4ui jointVec((unsigned int)pJointVect4[i].x, (unsigned int)pJointVect4[i].y, (unsigned int)pJointVect4[i].z, (unsigned int)pJointVect4[i].w);
				Skin0.joints.push_back(jointVec);
				Skin0.weights.push_back(pWeightVect4[i]);
			}

			GLTF::SetVBO_pos(gltfModel, attributes["POSITION"], runModel.vbo_vert, pBinaryBuff);
			GLTF::SetVBO(gltfModel, attributes["NORMAL"], runModel.vbo_norm, pBinaryBuff);
			GLTF::SetVBO_uv(gltfModel, attributes["TEXCOORD_0"], runModel.vbo_uv, pBinaryBuff);
			GLTF::SetVBO_index(gltfModel, primitive.indices, runModel.vbo_index, pBinaryBuff);

			GLTF::SetVBO(gltfModel, attributes["WEIGHTS_0"], runModel.vbo_weights, pBinaryBuff);
			GLTF::SetVBO(gltfModel, attributes["JOINTS_0"], runModel.vbo_joints, pBinaryBuff);
			GLTF::SetVBO(gltfModel, gltfModel.skins[0].inverseBindMatrices, runModel.vbo_invBind, pBinaryBuff);

			const int textureIndexInt = gltfMaterial.pbrMetallicRoughness.baseColorTexture.index;
			unsigned int colorIndex;

			if (textureIndexInt > 0)
			{
				colorIndex = (unsigned int)textureIndexInt;
			}
			else
			{
				colorIndex = 0u;
			}

			GLTF::SetTexture(gltfModel, colorIndex, runModel.text_color, pBinaryBuff);

			bool printSkinData = false;

			if (printSkinData)
			{
				for (int i = 0; i < (int)runModel.vbo_joints.count * 4; i = i + 4)
				{
					Vec4 joint;
					joint[x] = (float)((unsigned short*)runModel.vbo_joints.poData)[i];
					joint[y] = (float)((unsigned short*)runModel.vbo_joints.poData)[i + 1];
					joint[z] = (float)((unsigned short*)runModel.vbo_joints.poData)[i + 2];
					joint[w] = (float)((unsigned short*)runModel.vbo_joints.poData)[i + 3];

					Trace::out("joint: %f, %f, %f, %f\n", joint.x(), joint.y(), joint.z(), joint.w());
				}

				for (int i = 0; i < (int)runModel.vbo_weights.count * 4; i = i + 4)
				{
					Vec4 weight;
					weight[x] = ((float*)runModel.vbo_weights.poData)[i];
					weight[y] = ((float*)runModel.vbo_weights.poData)[i + 1];
					weight[z] = ((float*)runModel.vbo_weights.poData)[i + 2];
					weight[w] = ((float*)runModel.vbo_weights.poData)[i + 3];

					Trace::out("weight: %f, %f, %f, %f\n", weight.x(), weight.y(), weight.z(), weight.w());
				}

				for (int i = 0; i < (int)runModel.vbo_invBind.count * 16; i = i + 16)
				{
					Mat4 invBind;
					invBind[m0] = ((float*)runModel.vbo_invBind.poData)[i];
					invBind[m1] = ((float*)runModel.vbo_invBind.poData)[i + 1];
					invBind[m2] = ((float*)runModel.vbo_invBind.poData)[i + 2];
					invBind[m3] = ((float*)runModel.vbo_invBind.poData)[i + 3];
					invBind[m4] = ((float*)runModel.vbo_invBind.poData)[i + 4];
					invBind[m5] = ((float*)runModel.vbo_invBind.poData)[i + 5];
					invBind[m6] = ((float*)runModel.vbo_invBind.poData)[i + 6];
					invBind[m7] = ((float*)runModel.vbo_invBind.poData)[i + 7];
					invBind[m8] = ((float*)runModel.vbo_invBind.poData)[i + 8];
					invBind[m9] = ((float*)runModel.vbo_invBind.poData)[i + 9];
					invBind[m10] = ((float*)runModel.vbo_invBind.poData)[i + 10];
					invBind[m11] = ((float*)runModel.vbo_invBind.poData)[i + 11];
					invBind[m12] = ((float*)runModel.vbo_invBind.poData)[i + 12];
					invBind[m13] = ((float*)runModel.vbo_invBind.poData)[i + 13];
					invBind[m14] = ((float*)runModel.vbo_invBind.poData)[i + 14];
					invBind[m15] = ((float*)runModel.vbo_invBind.poData)[i + 15];

					Trace::out("invBind 0: %f, %f, %f, %f\n", invBind.get(Row4::i0).x(), invBind.get(Row4::i0).y(), invBind.get(Row4::i0).z(), invBind.get(Row4::i0).w());
					Trace::out("invBind 1: %f, %f, %f, %f\n", invBind.get(Row4::i1).x(), invBind.get(Row4::i1).y(), invBind.get(Row4::i1).z(), invBind.get(Row4::i1).w());
					Trace::out("invBind 2: %f, %f, %f, %f\n", invBind.get(Row4::i2).x(), invBind.get(Row4::i2).y(), invBind.get(Row4::i2).z(), invBind.get(Row4::i2).w());
					Trace::out("invBind 3: %f, %f, %f, %f\n\n", invBind.get(Row4::i3).x(), invBind.get(Row4::i3).y(), invBind.get(Row4::i3).z(), invBind.get(Row4::i3).w());
				}
			}

			runModel.triCount = runModel.vbo_index.count / 3;
			runModel.vertCount = runModel.vbo_vert.count;
			runModel.mode = meshDataConverter::GetMode(gltfModel.meshes[0].primitives[0].mode);
			assert(runModel.mode == meshData::RENDER_MODE::MODE_TRIANGLES);

			GLTF::InsertBoundingSphereData(runModel);

			meshData_proto mA_proto;
			runModel.Serialize(mA_proto);
		}

		WriteAndVerifyRuntimeModel(azulRunModel, pName);

		delete[] poBuff;
		delete[] poJSON;
	}
}