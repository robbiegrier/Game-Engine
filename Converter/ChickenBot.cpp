#include "GLTF.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "json.hpp"
#include "File.h"
#include "MathEngine.h"
#include "ChickenBot.h"
#include "MeshLayout.h"
#include "BoundingSphere.h"
#include "azulModel.h"

using namespace Azul;
using namespace tinygltf;
using json = nlohmann::json;

void CreateChickenBotModels()
{
	bool status;
	tinygltf::Model gltfModel;

	const char* pGLB_Name = "walk_mesh";
	const char* pGLB_extension = ".glb";
	const char* pProto_extension = ".proto.azul";

	size_t FileNameSize = strlen(pGLB_Name) + strlen(pGLB_extension) + 1;

	char* poFileName = new char[FileNameSize]();
	strcat_s(poFileName, FileNameSize, pGLB_Name);
	strcat_s(poFileName, FileNameSize, pGLB_extension);

	size_t ProtoFileNameSize = strlen(pGLB_Name) + strlen(pProto_extension) + 1;

	char* poProtoFileName = new char[ProtoFileNameSize]();
	strcat_s(poProtoFileName, ProtoFileNameSize, pGLB_Name);
	strcat_s(poProtoFileName, ProtoFileNameSize, pProto_extension);

	status = GLTF::LoadBinary(gltfModel, poFileName);
	assert(status);

	char* poBuff = nullptr;
	unsigned int BuffSize(0);

	status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, poFileName);
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

	for (size_t jointIndex = 0; jointIndex < gltfModel.skins[0].joints.size(); jointIndex++)
	{
		azulModel azModel;
		meshData& runModel = azModel.meshes[0];

		BoneMesh Bone0;
		size_t JointTarget = jointIndex;
		{
			size_t inverseMatrixAccessorIndex = 17;
			size_t weightAccessorIndex = 16;
			size_t jointAccessorIndex = 15;
			size_t indexAccessorIndex = 11;
			size_t posAccessorIndex = 12;
			size_t normsAccessorIndex = 13;
			size_t texCoordAccessorIndex = 14;

			unsigned char* pBuff = (unsigned char*)&gltfModel.buffers[0].data[0];

			auto JointAccessor = gltfModel.accessors[jointAccessorIndex];
			auto JointBuffView = gltfModel.bufferViews[(size_t)JointAccessor.bufferView];

			unsigned char* pJointBuff = pBuff + JointBuffView.byteOffset;
			assert(JointAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT);
			assert(JointAccessor.type == TINYGLTF_TYPE_VEC4);

			auto WeightAccessor = gltfModel.accessors[weightAccessorIndex];
			auto WeightBuffView = gltfModel.bufferViews[(size_t)WeightAccessor.bufferView];

			unsigned char* pWeightBuff = pBuff + WeightBuffView.byteOffset;
			assert(WeightAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			assert(WeightAccessor.type == TINYGLTF_TYPE_VEC4);

			auto InverseAccessor = gltfModel.accessors[inverseMatrixAccessorIndex];
			auto InverseBuffView = gltfModel.bufferViews[(size_t)InverseAccessor.bufferView];

			unsigned char* pInverseMatrixBuff = pBuff + InverseBuffView.byteOffset;
			assert(InverseAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			assert(InverseAccessor.type == TINYGLTF_TYPE_MAT4);
			assert(WeightAccessor.count == JointAccessor.count);

			auto IndexAccessor = gltfModel.accessors[indexAccessorIndex];
			auto IndexBuffView = gltfModel.bufferViews[(size_t)IndexAccessor.bufferView];

			unsigned char* pIndexBuff = pBuff + IndexBuffView.byteOffset;
			assert(IndexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT);
			assert(IndexAccessor.type == TINYGLTF_TYPE_SCALAR);

			auto PosAccessor = gltfModel.accessors[posAccessorIndex];
			auto PosBuffView = gltfModel.bufferViews[(size_t)PosAccessor.bufferView];

			unsigned char* pPosBuff = pBuff + PosBuffView.byteOffset;
			assert(PosAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			assert(PosAccessor.type == TINYGLTF_TYPE_VEC3);

			auto NormsAccessor = gltfModel.accessors[normsAccessorIndex];
			auto NormsBuffView = gltfModel.bufferViews[(size_t)NormsAccessor.bufferView];

			unsigned char* pNormsBuff = pBuff + NormsBuffView.byteOffset;
			assert(NormsAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			assert(NormsAccessor.type == TINYGLTF_TYPE_VEC3);

			auto TexCoordAccessor = gltfModel.accessors[texCoordAccessorIndex];
			auto TexCoordBuffView = gltfModel.bufferViews[(size_t)TexCoordAccessor.bufferView];

			unsigned char* pTexCoordBuff = pBuff + TexCoordBuffView.byteOffset;
			assert(TexCoordAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
			assert(TexCoordAccessor.type == TINYGLTF_TYPE_VEC2);

			size_t ElementCount;
			ElementCount = IndexAccessor.count / 3;

			std::vector<Vec3ui> indexVector;
			Vec3si* pisVect3 = (Vec3si*)pIndexBuff;

			for (size_t i = 0; i < ElementCount; i++)
			{
				indexVector.push_back(Vec3ui(pisVect3->x, pisVect3->y, pisVect3->z));
				pisVect3++;
			}

			std::vector<skin_weight_joint_index> WeightJoint;
			WeightJoint.reserve(WeightAccessor.count);

			AZUL_UNUSED_VAR(pInverseMatrixBuff);

			Vec4si* pJointVect4 = (Vec4si*)pJointBuff;
			Vec4f* pWeightVect4 = (Vec4f*)pWeightBuff;
			for (size_t i = 0; i < WeightAccessor.count; i++)
			{
				WeightJoint.push_back(skin_weight_joint_index());
				WeightJoint[i].index = i;
				WeightJoint[i].jointIndex = Vec4si(pJointVect4->x,
					pJointVect4->y,
					pJointVect4->z,
					pJointVect4->w);
				WeightJoint[i].skinWeight = Vec4f(pWeightVect4->x,
					pWeightVect4->y,
					pWeightVect4->z,
					pWeightVect4->w);
				pJointVect4++;
				pWeightVect4++;
			}

			for (size_t i = 0; i < WeightJoint.size(); i++)
			{
				assert(WeightJoint[i].jointIndex.y == 0);
				assert(WeightJoint[i].jointIndex.z == 0);
				assert(WeightJoint[i].jointIndex.w == 0);

				assert(WeightJoint[i].skinWeight.y == 0.0f);
				assert(WeightJoint[i].skinWeight.z == 0.0f);
				assert(WeightJoint[i].skinWeight.w == 0.0f);
			}

			size_t NodeIndex = (size_t)gltfModel.skins[0].joints[JointTarget];

			Bone0.vertCount = 0;
			Bone0.jointIndex = JointTarget;
			Bone0.boneName = gltfModel.nodes[NodeIndex].name;

			Mat4* pInverseMatrix = (Mat4*)pInverseMatrixBuff;
			Bone0.inverseMatrix = pInverseMatrix[JointTarget];
			Bone0.corrected = false;

			Vec3f* pPos = (Vec3f*)pPosBuff;
			Vec3f* pNorms = (Vec3f*)pNormsBuff;
			Vec2f* pTexCoord = (Vec2f*)pTexCoordBuff;
			size_t count(0);
			size_t localIndex(0);

			for (size_t i = 0; i < WeightJoint.size(); i++)
			{
				if (WeightJoint[i].jointIndex.x == JointTarget)
				{
					assert(WeightJoint[i].skinWeight.x == 1.0f);
					size_t JointIndex = WeightJoint[i].jointIndex.x;

					Bone0.l2gIndex.push_back(LocalToGlobalIndex(localIndex, i, JointIndex));
					Bone0.pos.push_back(pPos[i]);
					Bone0.norms.push_back(pNorms[i]);
					Bone0.text_coord.push_back(pTexCoord[i]);

					count++;
					localIndex++;
				}
			}
			Bone0.vertCount = count;

			int cc = 0;
			for (size_t i = 0; i < indexVector.size(); i++)
			{
				int f0 = -1;
				int f1 = -1;
				int f2 = -1;

				for (size_t j = 0; j < Bone0.l2gIndex.size(); j++)
				{
					if (indexVector[i].x == Bone0.l2gIndex[j].globalIndex)
					{
						f0 = (int)Bone0.l2gIndex[j].localIndex;
						break;
					}
				}
				for (size_t j = 0; j < Bone0.l2gIndex.size(); j++)
				{
					if (indexVector[i].y == Bone0.l2gIndex[j].globalIndex)
					{
						f1 = (int)Bone0.l2gIndex[j].localIndex;
						break;
					}
				}
				for (size_t j = 0; j < Bone0.l2gIndex.size(); j++)
				{
					if (indexVector[i].z == Bone0.l2gIndex[j].globalIndex)
					{
						f2 = (int)Bone0.l2gIndex[j].localIndex;
						break;
					}
				}

				if (f0 == -1 && f1 == -1 && f2 == -1)
				{
					continue;
				}
				else
				{
					//Trace::out("%d: (%d) %d %d %d  %d %d %d\n",
					//	cc,
					//	i,
					//	indexVector[i].v0,
					//	indexVector[i].v1,
					//	indexVector[i].v2,
					//	f0, f1, f2);
					assert(f0 >= 0);
					assert(f1 >= 0);
					assert(f2 >= 0);
					Bone0.index.push_back(Vec3ui((unsigned int)f0, (unsigned int)f1, (unsigned int)f2));
					cc++;
				}
			}

			for (size_t i = 0; i < Bone0.pos.size(); i++)
			{
				Vec3  v(Bone0.pos[i].x, Bone0.pos[i].y, Bone0.pos[i].z);
				Mat4  M(Bone0.inverseMatrix);
				Vec3  vOut;

				vOut = Vec4(v, 1) * M;

				Bone0.pos[i].x = vOut[x];
				Bone0.pos[i].y = vOut[y];
				Bone0.pos[i].z = vOut[z];
			}

			for (size_t i = 0; i < Bone0.norms.size(); i++)
			{
				Vec3   v(Bone0.norms[i].x, Bone0.norms[i].y, Bone0.norms[i].z);
				Mat4  M(Bone0.inverseMatrix);
				Vec3    vOut;

				vOut = Vec4(v, 1) * M;
				vOut.norm();

				Bone0.norms[i].x = vOut[x];
				Bone0.norms[i].y = vOut[y];
				Bone0.norms[i].z = vOut[z];
			}

			Bone0.corrected = true;
		}

		pGLB_Name = Bone0.boneName.c_str();
		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, pGLB_Name, gltfModel.nodes[1].name.length());

		GLTF::SetVBO_BONE(gltfModel,
			"POSITION",
			runModel.vbo_vert,
			(char*)&Bone0.pos[0],
			Bone0.pos.size() * sizeof(Vec3f),
			Bone0.pos.size());
		runModel.vbo_vert.Print("vbo_vert");

		GLTF::SetVBO_BONE(gltfModel,
			"NORMAL",
			runModel.vbo_norm,
			(char*)&Bone0.norms[0],
			Bone0.norms.size() * sizeof(Vec3f),
			Bone0.norms.size());
		runModel.vbo_norm.Print("vbo_norm");

		GLTF::SetVBO_BONE(gltfModel,
			"TEXCOORD_0",
			runModel.vbo_uv,
			(char*)&Bone0.text_coord[0],
			Bone0.text_coord.size() * sizeof(Vec2f),
			Bone0.text_coord.size());
		runModel.vbo_uv.Print("vbo_uv");

		runModel.vbo_index.componentType = vboData::VBO_COMPONENT::UNSIGNED_INT;
		GLTF::SetVBO_BONE_index(gltfModel,
			runModel.vbo_index,
			(char*)&Bone0.index[0],
			Bone0.index.size() * sizeof(Vec3ui),
			Bone0.index.size() * 3);
		runModel.vbo_index.Print("vbo_index");

		runModel.triCount = runModel.vbo_index.count / 3;
		runModel.vertCount = runModel.vbo_vert.count;
		runModel.mode = meshDataConverter::GetMode(gltfModel.meshes[0].primitives[0].mode);
		assert(runModel.mode == meshData::RENDER_MODE::MODE_TRIANGLES);

		GLTF::SetTexture(gltfModel, 0, runModel.text_color, pBinaryBuff);

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

		Trace::out("--------------\n");
		Trace::out("------1-------\n");
		Trace::out("--------------\n");

		runModel.Print("mA");
		azulModel_proto mA_proto;
		azModel.Serialize(mA_proto);

		Trace::out("\n");
		Trace::out("message size: %d \n", mA_proto.ByteSizeLong());
		Trace::out("\n");

		File::Handle fh;
		File::Error err;

		// Create output name
		std::string BaseName(pGLB_Name);
		std::string OutputFileName = BaseName + ".proto.azul";

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
		assert(err == File::Error::SUCCESS);

		std::string strOut;
		mA_proto.SerializeToString(&strOut);

		File::Write(fh, strOut.data(), strOut.length());
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		err = File::Open(fh, OutputFileName.c_str(), File::Mode::READ);
		assert(err == File::Error::SUCCESS);

		err = File::Seek(fh, File::Position::END, 0);
		assert(err == File::Error::SUCCESS);

		DWORD FileLength;
		err = File::Tell(fh, FileLength);
		assert(err == File::Error::SUCCESS);

		char* poNewBuff = new char[FileLength]();
		assert(poNewBuff);

		err = File::Seek(fh, File::Position::BEGIN, 0);
		assert(err == File::Error::SUCCESS);

		err = File::Read(fh, poNewBuff, FileLength);
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		Trace::out("--------------\n");
		Trace::out("------2-------\n");
		Trace::out("--------------\n");

		std::string strIn(poNewBuff, FileLength);
		azulModel_proto mB_proto;

		mB_proto.ParseFromString(strIn);

		azulModel mB;
		mB.Deserialize(mB_proto);
		mB.Print("mB");

		delete[] poNewBuff;
	}

	delete[] poProtoFileName;
	delete[] poFileName;
	delete[] poJSON;
	delete[] poBuff;
}