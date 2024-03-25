#ifndef _CSAnimationMixer
#define _CSAnimationMixer

#include "ComputeRSVBuffer.h"
#include "ComputeUAVBuffer.h"
#include "MathEngine.h"

namespace Azul
{
	struct BoneTransform;

	class CSAnimationMixer
	{
	public:
		CSAnimationMixer();
		CSAnimationMixer(const CSAnimationMixer&) = delete;
		CSAnimationMixer& operator=(const CSAnimationMixer&) = delete;
		~CSAnimationMixer();

		void Open(BoneTransform* pInBoneA, BoneTransform* pInBoneB, float inTime);
		void Run();
		void AwaitResults(BoneTransform* pResultOut);
		void Close();

		ID3D11ComputeShader* pComputeShader;
		ID3D11Buffer* pComputeOutBuffer = nullptr;

		struct CBMixer
		{
			Vec4 time;
		};
		ID3D11Buffer* pConstBuffMixer;

		ComputeRSVBuffer frameBufferA;
		ComputeRSVBuffer frameBufferB;
		ComputeUAVBuffer* pFrameBufferResult;

		BoneTransform* pBoneCacheA;
		BoneTransform* pBoneCacheB;
		float timeCache;
	};
}

#endif