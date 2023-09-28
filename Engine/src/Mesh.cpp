#include "Engine.h"
#include "Mesh.h"

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
		poConstantBuff_Projection{ nullptr },
		poConstantBuff_World{ nullptr },
		poConstantBuff_View{ nullptr },
		poConstantBuff_lightColor{ nullptr },
		poConstantBuff_lightPos{ nullptr }
	{
	}

	const char* Mesh::NameToString()
	{
		switch (name)
		{
		case Name::Cube:
			return "Cube";
		case Name::Pyramid:
			return "Pyramid";
		case Name::Null:
			return "Null";
		default:
			return "None";
		}
	}

	Mesh::~Mesh()
	{
		SafeRelease(poVertexBuffer_pos);
		SafeRelease(poVertexBuffer_color);
		SafeRelease(poVertexBuffer_norm);
		SafeRelease(poVertexBuffer_texCoord);
		SafeRelease(poIndexBuffer);
		SafeRelease(poConstantBuff_Projection);
		SafeRelease(poConstantBuff_World);
		SafeRelease(poConstantBuff_View);
		SafeRelease(poConstantBuff_lightColor);
		SafeRelease(poConstantBuff_lightPos);
	}

	void Mesh::RenderIndexBuffer()
	{
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

		Engine::GetContext()->UpdateSubresource(poConstantBuff_View, 0u, nullptr, &pCam->GetViewMatrix(), 0u, 0u);
		Engine::GetContext()->UpdateSubresource(poConstantBuff_Projection, 0u, nullptr, &pCam->GetProjMatrix(), 0u, 0u);
		Engine::GetContext()->UpdateSubresource(poConstantBuff_World, 0u, nullptr, &pWorld, 0u, 0u);
	}

	void Mesh::UpdateWorld(const Mat4& pWorld)
	{
		Engine::GetContext()->UpdateSubresource(poConstantBuff_World, 0u, nullptr, &pWorld, 0u, 0u);
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

		Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::World, 1u, &poConstantBuff_World);

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