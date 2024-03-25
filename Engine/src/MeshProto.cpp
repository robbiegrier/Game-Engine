#include "MeshProto.h"
#include "MathEngine.h"
#include "Engine.h"
#include "File.h"
#include "EngineUtils.h"

namespace Azul
{
	MeshProto::MeshProto(const char* const pMeshFileName, unsigned int index)
		: Mesh(0, 0)
	{
		azulModel_proto aB_proto;
		aB_proto.ParseFromString(EngineUtils::FileToString(pMeshFileName));

		azulModel aB;
		aB.Deserialize(aB_proto);

		meshData& mB = aB.meshes[index];

		SetFromMeshData(mB);

		Trace::out("Mesh %s loaded from converter v%d\n", mB.pMeshName, aB.converterVersion);
	}

	MeshProto::MeshProto(const meshData& mB)
		: Mesh(0, 0)
	{
		SetFromMeshData(mB);
	}

	MeshProto::~MeshProto()
	{
	}

	void MeshProto::SetFromMeshData(const meshData& mB)
	{
		numVerts = mB.vbo_vert.count;
		numIndices = mB.triCount * 3u;

		VerifyImportedData(mB);

		poVertexBuffer_pos = CreateVertexBuffer(mB.vbo_vert.dataSize, mB.vbo_vert.poData);
		poVertexBuffer_texCoord = CreateVertexBuffer(mB.vbo_uv.dataSize, mB.vbo_uv.poData);
		poVertexBuffer_norm = CreateVertexBuffer(mB.vbo_norm.dataSize, mB.vbo_norm.poData);
		poIndexBuffer = CreateIndexBuffer(mB.vbo_index.dataSize, mB.vbo_index.poData);
		poVertexBuffer_color = nullptr;
		poConstantBuff_lightColor = CreateConstantBuffer(sizeof(Vec3));
		poConstantBuff_lightPos = CreateConstantBuffer(sizeof(Vec3));

		if (mB.vbo_joints.dataSize != 0)
		{
			poVertexBuffer_joints = CreateVertexBuffer(mB.vbo_joints.dataSize, mB.vbo_joints.poData);
			poVertexBuffer_weights = CreateVertexBuffer(mB.vbo_weights.dataSize, mB.vbo_weights.poData);
			poConstantBuff_boneWorld = CreateConstantBuffer(sizeof(Mat4) * Mesh::BoneCountMax);

			Mat4* pMat = (Mat4*)mB.vbo_invBind.poData;
			for (size_t i = 0; i < mB.vbo_invBind.count; i++)
			{
				poInvBindArray[i].set(pMat[i]);
			}
		}

		boundingSphereRadius = mB.boundingSphereRadius;
		pBoundingSphereCenter->set(mB.boundingSphereCenter[0], mB.boundingSphereCenter[1], mB.boundingSphereCenter[2]);
	}

	void MeshProto::VerifyImportedData(const meshData& mB) const
	{
		static_cast<void>(mB); // unused in release

		assert(mB.vbo_vert.poData);
		assert(mB.vbo_vert.dataSize > 0);
		assert(mB.vbo_vert.vboType == vboData::VBO_TYPE::VEC3);
		assert(mB.vbo_vert.componentType == vboData::VBO_COMPONENT::FLOAT);

		assert(mB.vbo_uv.poData);
		assert(mB.vbo_uv.dataSize > 0);
		assert(mB.vbo_uv.vboType == vboData::VBO_TYPE::VEC2);
		assert(mB.vbo_uv.componentType == vboData::VBO_COMPONENT::FLOAT);

		assert(mB.vbo_norm.poData);
		assert(mB.vbo_norm.dataSize > 0);
		assert(mB.vbo_norm.vboType == vboData::VBO_TYPE::VEC3);
		assert(mB.vbo_norm.componentType == vboData::VBO_COMPONENT::FLOAT);

		assert(mB.vbo_index.vboType == vboData::VBO_TYPE::SCALAR);
		assert(mB.vbo_index.componentType != vboData::VBO_COMPONENT::DOUBLE
			&& mB.vbo_index.componentType != vboData::VBO_COMPONENT::FLOAT);
		assert(mB.vbo_index.dataSize > 0);
		assert(mB.vbo_index.poData);

		assert(numIndices > 0);
		assert(numVerts > 0);
	}
}