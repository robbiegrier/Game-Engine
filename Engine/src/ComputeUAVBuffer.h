#ifndef _ComputeUAVBuffer
#define _ComputeUAVBuffer

#include <d3d11.h>

namespace Azul
{
	class ComputeUAVBuffer
	{
	public:
		ComputeUAVBuffer() = delete;
		ComputeUAVBuffer(size_t count, size_t structSize);
		ComputeUAVBuffer(const ComputeUAVBuffer& r) = delete;
		ComputeUAVBuffer& operator=(const ComputeUAVBuffer&) = delete;
		virtual ~ComputeUAVBuffer();

		void Create(size_t count, size_t structSize);
		void Transfer(void* pBuff);
		void Bind(size_t slot);

		size_t count;
		size_t structSize;
		ID3D11Buffer* poComputeUAVBuffer;
		ID3D11ShaderResourceView* poComputeSRVBuffer;
		ID3D11UnorderedAccessView* poUnorderedAccessView;
	};
}

#endif
