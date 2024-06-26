#include "MeshProto.h"
#include "MathEngine.h"
#include "Engine.h"
#include "File.h"
#include "EngineUtils.h"

namespace Azul
{
	MeshProto::MeshProto()
		: Mesh(0, 0)
	{}

	MeshProto::MeshProto(const char* const pMeshFileName)
		: Mesh(0, 0)
	{
		azulModel_proto aB_proto;
		aB_proto.ParseFromString(EngineUtils::FileToString(pMeshFileName));

		azulModel aB;
		aB.Deserialize(aB_proto);

		meshData& mB = aB.meshes[0];

		SetFromMeshData(mB);

		for (unsigned int i = 1; i < 48; i++)
		{
			if (!aB.meshes[i].IsEmpty())
			{
				MeshProto* pSubMesh = new MeshProto();
				meshData& meshDataSubMesh = aB.meshes[i];
				pSubMesh->SetFromMeshData(meshDataSubMesh);
				AttachSubMesh(pSubMesh);

				Trace::out("Attached submesh for %s [%d]\n", pMeshFileName, (int)i);
			}
		}

		Trace::out("Mesh %s loaded from converter v%d\n", pMeshFileName, aB.converterVersion);
	}

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

		pVertexBufferPosition = CreateVertexBuffer(mB.vbo_vert.dataSize, mB.vbo_vert.poData);
		pVertexBufferTextureCoordinates = CreateVertexBuffer(mB.vbo_uv.dataSize, mB.vbo_uv.poData);
		pVertexBufferNormal = CreateVertexBuffer(mB.vbo_norm.dataSize, mB.vbo_norm.poData);
		pIndexBufferTriangles = CreateIndexBuffer(mB.vbo_index.dataSize, mB.vbo_index.poData);
		pVertexBufferColor = nullptr;
		pConstantBufferLightColor = CreateConstantBuffer(sizeof(Vec3));
		pConstantBufferLightPosition = CreateConstantBuffer(sizeof(Vec3));

		if (mB.vbo_joints.dataSize != 0)
		{
			pVertexBufferJoints = CreateVertexBuffer(mB.vbo_joints.dataSize, mB.vbo_joints.poData);
			pVertexBufferWeights = CreateVertexBuffer(mB.vbo_weights.dataSize, mB.vbo_weights.poData);
			pConstantBufferBoneWorld = CreateConstantBuffer(sizeof(Mat4) * Mesh::BoneCountMax);

			Mat4* pMat = (Mat4*)mB.vbo_invBind.poData;
			for (size_t i = 0; i < mB.vbo_invBind.count; i++)
			{
				pInverseBindArray[i].set(pMat[i]);
			}
		}

		boundingSphereRadius = mB.boundingSphereRadius;
		pBoundingSphereCenter->set(mB.boundingSphereCenter[0], mB.boundingSphereCenter[1], mB.boundingSphereCenter[2]);

		pAABBMax->set(mB.aabbMax[0], mB.aabbMax[1], mB.aabbMax[2]);
		pAABBMin->set(mB.aabbMin[0], mB.aabbMin[1], mB.aabbMin[2]);
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