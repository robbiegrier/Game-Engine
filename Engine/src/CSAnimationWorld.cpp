#include "CSAnimationWorld.h"
#include "BoneTransform.h"
#include "Engine.h"
#include "AnimationWorld.h"
#include "Clip.h"

namespace Azul
{
	CSAnimationWorld::CSAnimationWorld()
		: table{ BONE_COUNT_MAX * HIERARCHY_DEPTH_MAX, sizeof(int) },
		worldResult{ BONE_COUNT_MAX, sizeof(Mat4) }
	{
		HRESULT hr;
		hr = Engine::GetDevice()->CreateComputeShader(g_AnimationWorld_ComputeShader, sizeof(g_AnimationWorld_ComputeShader), nullptr, &pComputeShader);
		assert(SUCCEEDED(hr));
		assert(pComputeShader);

		D3D11_BUFFER_DESC bufferDesc{ 0 };
		bufferDesc.ByteWidth = BONE_COUNT_MAX * sizeof(Mat4);
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		bufferDesc.Usage = D3D11_USAGE_STAGING;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = sizeof(Mat4);
		hr = Engine::GetDevice()->CreateBuffer(&bufferDesc, nullptr, &pComputeOutBuffer);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC buffDescConstBuff{ 0 };
		buffDescConstBuff.Usage = D3D11_USAGE_DEFAULT;
		buffDescConstBuff.ByteWidth = sizeof(CBAnimWorld);
		buffDescConstBuff.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescConstBuff.CPUAccessFlags = 0;
		buffDescConstBuff.MiscFlags = 0;
		buffDescConstBuff.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescConstBuff, nullptr, &pConstBuffAnimWorld);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC buffDescConstBuffInvBind{ 0 };
		buffDescConstBuffInvBind.Usage = D3D11_USAGE_DEFAULT;
		buffDescConstBuffInvBind.ByteWidth = sizeof(Mat4) * BONE_COUNT_MAX;
		buffDescConstBuffInvBind.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescConstBuffInvBind.CPUAccessFlags = 0;
		buffDescConstBuffInvBind.MiscFlags = 0;
		buffDescConstBuffInvBind.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescConstBuffInvBind, nullptr, &pConstBuffInvBindArray);
		assert(SUCCEEDED(hr));
	}

	CSAnimationWorld::~CSAnimationWorld()
	{
		SafeRelease(pComputeShader);
		SafeRelease(pComputeOutBuffer);
		SafeRelease(pConstBuffAnimWorld);
		SafeRelease(pConstBuffInvBindArray);
	}

	void CSAnimationWorld::Open(int* pTable, int hierarchyDepth, const Mat4& pivotWorld, Mat4* pInvBindArray)
	{
		assert(pBoneLocal);
		Engine::GetContext()->CSSetShader(pComputeShader, nullptr, 0u);

		CBAnimWorld constants;
		constants.hierarchyDepth = hierarchyDepth;
		constants.pivotWorld = pivotWorld;
		Engine::GetContext()->UpdateSubresource(pConstBuffAnimWorld, 0u, nullptr, &constants, 0u, 0u);
		Engine::GetContext()->CSSetConstantBuffers(0u, 1u, &pConstBuffAnimWorld);

		Engine::GetContext()->UpdateSubresource(pConstBuffInvBindArray, 0u, nullptr, pInvBindArray, 0u, 0u);
		Engine::GetContext()->CSSetConstantBuffers(1u, 1u, &pConstBuffInvBindArray);

		pBoneLocal->Bind(0u);
		table.Bind(1u);
		worldResult.Bind(1u);

		table.Transfer(pTable);
	}

	void CSAnimationWorld::Run()
	{
		Engine::GetContext()->Dispatch(1u, 1u, 1u);
	}

	void CSAnimationWorld::AwaitResults(Mat4* pResultOut)
	{
		constexpr static int arrSize = BONE_COUNT_MAX * sizeof(Mat4);

		Engine::GetContext()->CopyResource(pComputeOutBuffer, worldResult.poComputeUAVBuffer);

		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
		hr = Engine::GetContext()->Map(pComputeOutBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
		assert(SUCCEEDED(hr));
		assert(mappedResource.pData);

		memcpy_s(pResultOut, arrSize, mappedResource.pData, arrSize);
	}

	void CSAnimationWorld::Close()
	{
		Engine::GetContext()->Unmap(pComputeOutBuffer, 0);
	}

	void CSAnimationWorld::AttachTo(const CSAnimationMixer& computeShaderMixer)
	{
		pBoneLocal = computeShaderMixer.pFrameBufferResult;
	}
}