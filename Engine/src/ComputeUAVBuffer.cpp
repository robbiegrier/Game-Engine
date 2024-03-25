#include "ComputeUAVBuffer.h"
#include "Engine.h"

namespace Azul
{
	ComputeUAVBuffer::ComputeUAVBuffer(size_t inCount, size_t inStructSize)
		: count{ 0 },
		structSize{ 0 },
		poComputeUAVBuffer{ nullptr },
		poComputeSRVBuffer{ nullptr },
		poUnorderedAccessView{ nullptr }
	{
		Create(inCount, inStructSize);
	}

	void ComputeUAVBuffer::Create(size_t _count, size_t _structSize)
	{
		count = _count;
		structSize = _structSize;
		assert(count > 0);
		assert(structSize > 0);

		HRESULT hr;
		D3D11_BUFFER_DESC bufferDesc{ 0 };
		bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		bufferDesc.ByteWidth = count * structSize;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = structSize;
		hr = Engine::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &poComputeUAVBuffer);
		assert(SUCCEEDED(hr));

		D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
		sbUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		sbUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		sbUAVDesc.Buffer.FirstElement = 0;
		sbUAVDesc.Buffer.Flags = 0;
		sbUAVDesc.Buffer.NumElements = _count;
		hr = Engine::GetDevice()->CreateUnorderedAccessView(this->poComputeUAVBuffer, &sbUAVDesc, &this->poUnorderedAccessView);
		assert(SUCCEEDED(hr));
	}

	void ComputeUAVBuffer::Transfer(void* pBuff)
	{
		Engine::GetContext()->UpdateSubresource(poComputeUAVBuffer, 0, nullptr, pBuff, 0, 0);
	}

	void ComputeUAVBuffer::Bind(size_t slot)
	{
		Engine::GetContext()->CSSetUnorderedAccessViews(slot, 1, &poUnorderedAccessView, nullptr);
	}

	ComputeUAVBuffer::~ComputeUAVBuffer()
	{
		SafeRelease(this->poComputeUAVBuffer);
		SafeRelease(this->poComputeSRVBuffer);
		SafeRelease(this->poUnorderedAccessView);
	}
}