#include "Engine.h"
#include "Mesh.h"
#include "BoundingSphere.h"

namespace Azul
{
	Mesh::Mesh(unsigned int _numVerts, unsigned int _numIndices)
		:
		numVerts{ _numVerts },
		numIndices{ _numIndices },
		poVertexBuffer_pos{ nullptr },
		poVertexBuffer_color{ nullptr },
		poVertexBuffer_norm{ nullptr },
		poVertexBuffer_texCoord{ nullptr },
		poIndexBuffer{ nullptr },
		poConstantBuff_lightColor{ nullptr },
		poConstantBuff_lightPos{ nullptr },
		pBoundingSphereCenter{ new Vec3() }
	{
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
		case Name::Bone:
		case Name::Bone_001:
		case Name::Bone_L:
		case Name::Bone_L_001:
		case Name::Bone_L_002:
		case Name::Bone_R:
		case Name::Bone_R_001:
		case Name::Bone_R_002:
			return "Bone";
		case Name::Null:
			return "Null";
		default:
			return "None";
		}
	}

	float Mesh::GetBoundingSphereRadius() const
	{
		return boundingSphereRadius;
	}

	const Vec3& Mesh::GetBoundSphereCenter() const
	{
		return *pBoundingSphereCenter;
	}

	void Mesh::HackSetBoundingSphereData(VertexPos* pData)
	{
		Sphere sphere;
		Vec3* pVerts = new Vec3[(unsigned int)this->numVerts];
		for (unsigned int i = 0; i < this->numVerts; i++)
		{
			pVerts[i].set(pData[i].Position.x,
				pData[i].Position.y,
				pData[i].Position.z);
			//Trace::out("%d:  %f %f %f\n", i, pVerts[i][x], pVerts[i][y], pVerts[i][z]);
		}
		RitterSphere(sphere, pVerts, this->numVerts);
		boundingSphereRadius = sphere.rad;
		pBoundingSphereCenter->set(sphere.cntr[x], sphere.cntr[y], sphere.cntr[z]);

		delete[] pVerts;
	}

	Mesh::~Mesh()
	{
		SafeRelease(poVertexBuffer_pos);
		SafeRelease(poVertexBuffer_color);
		SafeRelease(poVertexBuffer_norm);
		SafeRelease(poVertexBuffer_texCoord);
		SafeRelease(poIndexBuffer);
		SafeRelease(poConstantBuff_lightColor);
		SafeRelease(poConstantBuff_lightPos);
		delete pBoundingSphereCenter;
	}

	void Mesh::RenderIndexBuffer()
	{
		ActivateModel();

		// Set (point to ) Index buffer: Render configuration: Triangles
		Engine::GetContext()->IASetIndexBuffer(poIndexBuffer, DXGI_FORMAT_R32_UINT, 0u);
		Engine::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// RENDER - using index data
		Engine::GetContext()->DrawIndexed(numIndices, 0u, 0u);
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

	void Mesh::TransferConstantBuffer(Camera* pCam, const Mat4& pWorld)
	{
		assert(pCam);
		static_cast<void>(pWorld);
		static_cast<void>(pCam);

		//Engine::GetContext()->UpdateSubresource(poConstantBuff_World, 0u, nullptr, &pWorld, 0u, 0u);
	}

	void Mesh::ActivateModel()
	{
		const UINT vertexStride_pos = sizeof(VertexPos);
		const UINT vertexStride_color = sizeof(VertexColor);
		const UINT offset = 0u;

		if (poVertexBuffer_pos)
		{
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::Position, 1u, &poVertexBuffer_pos, &vertexStride_pos, &offset);
		}
		if (poVertexBuffer_color)
		{
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::Color, 1u, &poVertexBuffer_color, &vertexStride_color, &offset);
		}

		if (poVertexBuffer_norm)
		{
			const UINT vertexStride_norm = sizeof(VertexNorm);
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::Norm, 1, &poVertexBuffer_norm, &vertexStride_norm, &offset);
		}

		if (poVertexBuffer_texCoord)
		{
			const UINT vertexStride_texCoord = sizeof(VertexTexCoord);
			Engine::GetContext()->IASetVertexBuffers((uint32_t)VertexSlot::TexCoord, 1, &poVertexBuffer_texCoord, &vertexStride_texCoord, &offset);
		}

		if (poConstantBuff_lightColor)
		{
			Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::Color, 1, &poConstantBuff_lightColor);
		}

		if (poConstantBuff_lightPos)
		{
			Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::LightPos, 1, &poConstantBuff_lightPos);
		}
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
}