#include "ComputeRSVBuffer.h"
#include "Engine.h"

namespace Azul
{
	ComputeRSVBuffer::ComputeRSVBuffer(size_t inCount, size_t inStructSize)
		: count{ 0 },
		structSize{ 0 },
		poComputeRVSBuffer{ nullptr },
		poShaderResourceView{ nullptr }
	{
		Create(inCount, inStructSize);
	}

	void ComputeRSVBuffer::Create(size_t _count, size_t _structSize)
	{
		count = _count;
		structSize = _structSize;

		assert(count > 0);
		assert(structSize > 0);

		HRESULT hr;
		D3D11_BUFFER_DESC bufferDesc{ 0 };
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.ByteWidth = count * structSize;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = structSize;
		hr = Engine::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &poComputeRVSBuffer);
		assert(SUCCEEDED(hr));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.BufferEx.FirstElement = 0;
		srvDesc.BufferEx.Flags = 0;
		srvDesc.BufferEx.NumElements = _count;
		hr = Engine::GetDevice()->CreateShaderResourceView(poComputeRVSBuffer, &srvDesc, &poShaderResourceView);
		assert(SUCCEEDED(hr));
	}

	void ComputeRSVBuffer::Transfer(void* pBuff)
	{
		Engine::GetContext()->UpdateSubresource(poComputeRVSBuffer, 0, nullptr, pBuff, 0, 0);
	}

	void ComputeRSVBuffer::Bind(size_t slot)
	{
		Engine::GetContext()->CSSetShaderResources(slot, 1, &poShaderResourceView);
	}

	ComputeRSVBuffer::~ComputeRSVBuffer()
	{
		SafeRelease(poComputeRVSBuffer);
		SafeRelease(poShaderResourceView);
	}
}