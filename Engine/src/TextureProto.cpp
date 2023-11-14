#include "TextureProto.h"
#include "File.h"
#include "Engine.h"
#include <Windows.h>
#include <string>
#include <d3d11.h>
#include "DirectXTex.h"
#include "WICTextureLoader.h"

namespace Azul
{
	TextureProto::TextureProto(const char* const pMeshFileName, unsigned int index, D3D11_FILTER filter, DXGI_FORMAT format)
		: TextureObject(), poTex(nullptr)
	{
		DirectX::ScratchImage testTexture;

		azulModel_proto aB_proto;
		aB_proto.ParseFromString(EngineUtils::FileToString(pMeshFileName));

		azulModel aB;
		aB.Deserialize(aB_proto);

		meshData& mB = aB.meshes[index];
		SetFromMeshData(mB, filter, format);
	}

	TextureProto::TextureProto(const meshData& mB, D3D11_FILTER filter, DXGI_FORMAT format)
	{
		SetFromMeshData(mB, filter, format);
	}

	void TextureProto::SetFromMeshData(const meshData& mB, D3D11_FILTER filter, DXGI_FORMAT format)
	{
		assert(mB.text_color.textType == textureData::TEXTURE_TYPE::PNG);

		D3D11_TEXTURE2D_DESC desc;
		desc.Width = mB.text_color.width;
		desc.Height = mB.text_color.height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = format;//GUID_WICPixelFormat32bppRGBA
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = mB.text_color.poData;
		subResource.SysMemPitch = desc.Width * 4;
		subResource.SysMemSlicePitch = 0;

		HRESULT hr = Engine::GetDevice()->CreateTexture2D(&desc, &subResource, &poTex);
		assert(SUCCEEDED(hr));
		assert(poTex != nullptr);

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = Engine::GetDevice()->CreateShaderResourceView(poTex, &SRVDesc, &poTextureRV);
		assert(SUCCEEDED(hr));

		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = filter;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = Engine::GetDevice()->CreateSamplerState(&sampDesc, &poSampler);
		assert(SUCCEEDED(hr));
	}

	TextureProto::~TextureProto()
	{
		SafeRelease(poTex);
	}
}

// --- End of File ---