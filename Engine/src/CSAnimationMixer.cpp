#include "CSAnimationMixer.h"
#include "BoneTransform.h"
#include "Engine.h"
#include "AnimationMixer.h"
#include "Clip.h"

namespace Azul
{
	CSAnimationMixer::CSAnimationMixer()
		: frameBufferA{ BONE_COUNT_MAX, sizeof(BoneTransform) },
		frameBufferB{ BONE_COUNT_MAX, sizeof(BoneTransform) },
		pFrameBufferResult{ new ComputeUAVBuffer{ BONE_COUNT_MAX, sizeof(BoneTransform) } }
	{
		HRESULT hr;
		hr = Engine::GetDevice()->CreateComputeShader(g_AnimationMixer_ComputeShader, sizeof(g_AnimationMixer_ComputeShader), nullptr, &pComputeShader);
		assert(SUCCEEDED(hr));
		assert(pComputeShader);

		D3D11_BUFFER_DESC bufferDesc{ 0 };
		bufferDesc.ByteWidth = BONE_COUNT_MAX * sizeof(BoneTransform);
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bufferDesc.Usage = D3D11_USAGE_STAGING;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = sizeof(BoneTransform);
		hr = Engine::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &pComputeOutBuffer);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC buffDescConstBuffMixer{ 0 };
		buffDescConstBuffMixer.Usage = D3D11_USAGE_DEFAULT;
		buffDescConstBuffMixer.ByteWidth = sizeof(CBMixer);
		buffDescConstBuffMixer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescConstBuffMixer.CPUAccessFlags = 0;
		buffDescConstBuffMixer.MiscFlags = 0;
		buffDescConstBuffMixer.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescConstBuffMixer, nullptr, &pConstBuffMixer);
		assert(SUCCEEDED(hr));
	}

	CSAnimationMixer::~CSAnimationMixer()
	{
		SafeRelease(pComputeShader);
		SafeRelease(pComputeOutBuffer);
		SafeRelease(pConstBuffMixer);
		delete pFrameBufferResult;
	}

	void CSAnimationMixer::Open(BoneTransform* pInBoneA, BoneTransform* pInBoneB, float inTime)
	{
		Engine::GetContext()->CSSetShader(pComputeShader, nullptr, 0u);

		CBMixer constants;
		constants.time = Vec4(inTime, inTime, inTime, inTime);
		Engine::GetContext()->UpdateSubresource(pConstBuffMixer, 0u, nullptr, &constants, 0u, 0u);
		Engine::GetContext()->CSSetConstantBuffers(0u, 1u, &pConstBuffMixer);

		frameBufferA.Bind(0u);
		frameBufferB.Bind(1u);
		pFrameBufferResult->Bind(0u);

		frameBufferA.Transfer(pInBoneA);
		frameBufferB.Transfer(pInBoneB);
	}

	void CSAnimationMixer::Run()
	{
		Engine::GetContext()->Dispatch(1u, 1u, 1u);
	}

	void CSAnimationMixer::AwaitResults(BoneTransform* pResultOut)
	{
		constexpr static int arrSize = BONE_COUNT_MAX * sizeof(BoneTransform);

		Engine::GetContext()->CopyResource(pComputeOutBuffer, pFrameBufferResult->poComputeUAVBuffer);

		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
		hr = Engine::GetContext()->Map(pComputeOutBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
		assert(SUCCEEDED(hr));
		assert(mappedResource.pData);

		memcpy_s(pResultOut, arrSize, mappedResource.pData, arrSize);
	}

	void CSAnimationMixer::Close()
	{
		Engine::GetContext()->Unmap(pComputeOutBuffer, 0);
	}
}