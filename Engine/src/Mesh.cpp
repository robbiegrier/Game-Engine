#include "Engine.h"
#include "Mesh.h"
#include "BoundingSphere.h"

namespace Azul
{
	Mesh::Mesh(unsigned int _numVerts, unsigned int _numIndices)
		:
		numVerts{ _numVerts }, numIndices{ _numIndices },
		pVertexBufferPosition{ nullptr }, pVertexBufferColor{ nullptr }, pVertexBufferNormal{ nullptr },
		pVertexBufferTextureCoordinates{ nullptr }, pIndexBufferTriangles{ nullptr }, pConstantBufferLightColor{ nullptr },
		pConstantBufferLightPosition{ nullptr }, pConstantBufferUVMatrix{ nullptr }, pVertexBufferJoints{ nullptr },
		pVertexBufferWeights{ nullptr }, pConstantBufferBoneWorld{ nullptr },
		pBoundingSphereCenter{ new Vec3() }, pAABBMax{ new Vec3() }, pAABBMin{ new Vec3() }, pInverseBindArray{ new Mat4[BoneCountMax]() }
	{
		pConstantBufferUVMatrix = CreateConstantBuffer(sizeof(Mat4));

		for (size_t i = 0; i < BoneCountMax; i++)
		{
			pInverseBindArray[i].set(Special::Identity);
		}
	}

	Mesh::~Mesh()
	{
		SafeRelease(pIndexBufferTriangles);

		SafeRelease(pVertexBufferPosition);
		SafeRelease(pVertexBufferColor);
		SafeRelease(pVertexBufferNormal);
		SafeRelease(pVertexBufferTextureCoordinates);
		SafeRelease(pVertexBufferJoints);
		SafeRelease(pVertexBufferWeights);

		SafeRelease(pConstantBufferLightColor);
		SafeRelease(pConstantBufferLightPosition);
		SafeRelease(pConstantBufferUVMatrix);
		SafeRelease(pConstantBufferBoneWorld);

		delete pBoundingSphereCenter;
		delete pAABBMax;
		delete pAABBMin;
		delete[] pInverseBindArray;
	}

	void Mesh::TransferConstantBufferBoneWorldArray(Mat4* pBoneWorldArray)
	{
		assert(pBoneWorldArray);
		Engine::GetContext()->UpdateSubresource(pConstantBufferBoneWorld, 0, nullptr, pBoneWorldArray, 8, 0);
	}

	void Mesh::TransferUVCorrection(Mat4* pUVMatrix)
	{
		assert(pUVMatrix);
		Engine::GetContext()->UpdateSubresource(pConstantBufferUVMatrix, 0, nullptr, pUVMatrix, 0, 0);
	}

	void Mesh::RenderIndexBuffer()
	{
		ActivateModel();
		Engine::GetContext()->DrawIndexed(numIndices, 0u, 0u);
	}

	void Mesh::ActivateModel()
	{
		if (pVertexBufferPosition)
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::Position, 1u, &pVertexBufferPosition, &vertexStride_pos, &offset);
		if (pVertexBufferColor)
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::Color, 1u, &pVertexBufferColor, &vertexStride_color, &offset);
		if (pVertexBufferNormal)
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::Norm, 1, &pVertexBufferNormal, &vertexStride_norm, &offset);
		if (pVertexBufferTextureCoordinates)
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::TexCoord, 1, &pVertexBufferTextureCoordinates, &vertexStride_texCoord, &offset);
		if (pVertexBufferWeights)
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::Weights, 1, &pVertexBufferWeights, &vertexStride_weights, &offset);
		if (pVertexBufferJoints)
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::Joints, 1, &pVertexBufferJoints, &vertexStride_joints, &offset);

		if (pConstantBufferUVMatrix)
			Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::UV_Correction, 1, &pConstantBufferUVMatrix);
		if (pConstantBufferLightColor)
			Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::Color, 1, &pConstantBufferLightColor);
		//if (pConstantBufferLightPosition)
			//Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::LightPos, 1, &pConstantBufferLightPosition);
		if (pConstantBufferBoneWorld)
			Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::vsBoneWorld, 1, &pConstantBufferBoneWorld);

		Engine::GetContext()->IASetIndexBuffer(pIndexBufferTriangles, DXGI_FORMAT_R32_UINT, 0u);
		Engine::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void Mesh::AttachSubMesh(Mesh* pInSubMesh)
	{
		subMeshes.Add(pInSubMesh);
	}

	void Mesh::Wash()
	{
	}

	bool Mesh::Compare(DLink* pTargetNode)
	{
		Mesh* pOther = (Mesh*)pTargetNode;
		return pOther->GetName() == GetName();
	}

	void Mesh::Dump()
	{
		Trace::out("%s\n", NameToString());
	}

	void Mesh::SetName(Name inName)
	{
		name = inName;
	}

	Mesh::Name Mesh::GetName() const
	{
		return name;
	}

	float Mesh::GetBoundingSphereRadius() const
	{
		return boundingSphereRadius;
	}

	const Vec3& Mesh::GetBoundSphereCenter() const
	{
		return *pBoundingSphereCenter;
	}

	const Vec3& Mesh::GetAABBMax() const
	{
		return *pAABBMax;
	}

	const Vec3& Mesh::GetAABBMin() const
	{
		return *pAABBMin;
	}

	ID3D11Buffer* Mesh::CreateVertexBuffer(unsigned int numBytes, void* pData)
	{
		assert(pData);

		D3D11_BUFFER_DESC vertexBufferDesc{ 0 };
		D3D11_SUBRESOURCE_DATA resourceData{ 0 };

		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = numBytes;
		vertexBufferDesc.CPUAccessFlags = 0u;
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		resourceData.pSysMem = pData;

		ID3D11Buffer* pVertexBuffer;
		HRESULT hr = Engine::GetDevice()->CreateBuffer(&vertexBufferDesc, &resourceData, &pVertexBuffer);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		return pVertexBuffer;
	}

	ID3D11Buffer* Mesh::CreateIndexBuffer(unsigned int numBytes, void* pData)
	{
		assert(pData);

		D3D11_BUFFER_DESC indexBufferDesc{ 0 };
		D3D11_SUBRESOURCE_DATA resourceData{ 0 };

		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.ByteWidth = numBytes;
		indexBufferDesc.CPUAccessFlags = 0u;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		resourceData.pSysMem = pData;

		ID3D11Buffer* pIndexBuffer;
		HRESULT hr = Engine::GetDevice()->CreateBuffer(&indexBufferDesc, &resourceData, &pIndexBuffer);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		return pIndexBuffer;
	}

	ID3D11Buffer* Mesh::CreateConstantBuffer(unsigned int numBytes)
	{
		D3D11_BUFFER_DESC constantBufferDesc{ 0 };

		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.ByteWidth = numBytes;
		constantBufferDesc.CPUAccessFlags = 0u;
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		ID3D11Buffer* pConstantBuffer;
		HRESULT hr = Engine::GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &pConstantBuffer);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		return pConstantBuffer;
	}

	void Mesh::HackSetBoundingSphereData(VertexPos* pData)
	{
		Sphere sphere;
		Vec3* pVerts = new Vec3[(unsigned int)this->numVerts];
		for (unsigned int i = 0; i < this->numVerts; i++)
		{
			pVerts[i].set(pData[i].Position.x, pData[i].Position.y, pData[i].Position.z);
		}
		RitterSphere(sphere, pVerts, this->numVerts);
		boundingSphereRadius = sphere.rad;
		pBoundingSphereCenter->set(sphere.cntr[x], sphere.cntr[y], sphere.cntr[z]);

		delete[] pVerts;
	}

	void Mesh::HackSetBoundingBoxData(VertexPos* pData)
	{
		Vec3f& first = *(Vec3f*)pData;
		float minx = first.x;
		float miny = first.y;
		float minz = first.z;
		float maxx = first.x;
		float maxy = first.y;
		float maxz = first.z;

		for (int i = 1; i < (int)numVerts; i++)
		{
			Vec3f* pVec3f = (Vec3f*)pData;
			const Vec3f& w_vert = pVec3f[i];

			if (w_vert.x > maxx)	maxx = w_vert.x;
			else if (w_vert.x < minx)	minx = w_vert.x;
			if (w_vert.y > maxy)	maxy = w_vert.y;
			else if (w_vert.y < miny)	miny = w_vert.y;
			if (w_vert.z > maxz)	maxz = w_vert.z;
			else if (w_vert.z < minz)	minz = w_vert.z;
		}

		pAABBMax->set(maxx, maxy, maxz);
		pAABBMin->set(minx, miny, minz);
	}

	const char* Mesh::NameToString()
	{
		switch (name)
		{
		case Name::Sphere:
			return "Sphere";
		case Name::Cube:
			return "Cube";
		case Name::Pyramid:
			return "Pyramid";
		case Name::Diamond:
			return "Diamond";
		case Name::Cross:
			return "Cross";
		case Name::Plane:
			return "Plane";
		case Name::Crate:
			return "Crate";
		case Name::Frigate:
			return "Frigate";
		case Name::Bracket:
			return "Bracket";
		case Name::Fish:
			return "Fish";
		case Name::DogHouse:
			return "DogHouse";
		case Name::Duck:
			return "Duck";
		case Name::R2D2:
			return "R2D2";
		case Name::Corset:
			return "Corset";
		case Name::AntiqueCameraTripod:
			return "AntiqueCameraTripod";
		case Name::AntiqueCamera:
			return "AntiqueCamera";
		case Name::Dog:
			return "Dog";
		case Name::Dog1:
			return "Dog1";
		case Name::WesternTownHouse:
		case Name::WesternTownHouse1:
			return "WesternTownHouse";
		case Name::DesertRock0:
		case Name::DesertRock1:
		case Name::DesertRock2:
		case Name::DesertRock3:
		case Name::DesertRock4:
		case Name::DesertRock5:
		case Name::DesertRock6:
		case Name::DesertRock7:
		case Name::DesertRock8:
		case Name::DesertRock9:
		case Name::DesertRock10:
		case Name::DesertRock11:
		case Name::DesertRock12:
		case Name::DesertRock13:
			return "DesertRock";
		case Name::UnitSphere:
			return "UnitSphere";
		case Name::UnitIcoSphere:
			return "UnitIcoSphere";
		case Name::BoneTransform:
		case Name::Bone_001:
		case Name::Bone_L:
		case Name::Bone_L_001:
		case Name::Bone_L_002:
		case Name::Bone_R:
		case Name::Bone_R_001:
		case Name::Bone_R_002:
			return "Bone";
		case Name::Sprite:
			return "Sprite (Unit Square)";
		case Name::DryGrass0:
			return "DryGrass0";
		case Name::DryGrass1:
			return "DryGrass1";
		case Name::DryGrass2:
			return "DryGrass2";
		case Name::DryGrass3:
			return "DryGrass3";
		case Name::Null:
			return "Null";
		default:
			return "None";
		}
	}
}