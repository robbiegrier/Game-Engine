#ifndef _ComputeRSVBuffer
#define _ComputeRSVBuffer

#include <d3d11.h>

namespace Azul
{
	class ComputeRSVBuffer
	{
	public:
		ComputeRSVBuffer() = delete;
		ComputeRSVBuffer(size_t count, size_t structSize);
		ComputeRSVBuffer(const ComputeRSVBuffer& r) = delete;
		ComputeRSVBuffer& operator=(const ComputeRSVBuffer&) = delete;
		virtual ~ComputeRSVBuffer();

		void Create(size_t count, size_t structSize);
		void Transfer(void* pBuff);
		void Bind(size_t slot);

		size_t count;
		size_t structSize;
		ID3D11Buffer* poComputeRVSBuffer;
		ID3D11ShaderResourceView* poShaderResourceView;
	};
}

#endif
