#include "TextureObject.h"
#include "Engine.h"

#include <Windows.h>
#include <string>
#include <d3d11.h>
#include "DirectXTex.h"

namespace Azul
{
	TextureObject::TextureObject()
		:name(TextureObject::Name::None), TexResourceSlot(0), SamplerSlot(0), poTextureRV(nullptr), poSampler(nullptr)
	{
	}

	TextureObject::~TextureObject()
	{
		SafeRelease(poTextureRV);
		SafeRelease(poSampler);
	}

	TextureObject::TextureObject(TextureObject::Name _name, LPCWSTR filepath, D3D11_FILTER filter)
		:name(_name), TexResourceSlot(0), SamplerSlot(0),
		poTextureRV(nullptr), poSampler(nullptr)
	{
		DirectX::ScratchImage testTexture;
		HRESULT hr = LoadFromTGAFile(filepath, nullptr, testTexture);
		assert(SUCCEEDED(hr));

		CreateShaderResourceView(
			Engine::GetDevice(), 
			testTexture.GetImage(0, 0, 0), 
			testTexture.GetImageCount(), 
			testTexture.GetMetadata(), 
			&poTextureRV
		);

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = filter;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0.f;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = Engine::GetDevice()->CreateSamplerState(&sampDesc, &poSampler);
		assert(SUCCEEDED(hr));
	}

	void TextureObject::ActivateTexture()
	{
		Engine::GetContext()->PSSetShaderResources(TexResourceSlot, 1, &poTextureRV);
		Engine::GetContext()->PSSetSamplers(SamplerSlot, 1, &poSampler);
	}

	const char* TextureObject::NameToString()
	{
		switch (name)
		{
		case Name::Brick:
			return "Brick";
		case Name::Duckweed:
			return "Duckweed";
		case Name::Rocks:
			return "Rocks";
		case Name::Stone:
			return "Stone";
		case Name::NullTexture:
			return "NullTexture";
		default:
			return "NO STRING FOR TEXTURE";
		}
	}

	void TextureObject::SetName(TextureObject::Name _name)
	{
		name = _name;
	}

	void TextureObject::Wash()
	{
		name = Name::None;
	}

	bool TextureObject::Compare(DLink* pTargetNode)
	{
		TextureObject* pOther = (TextureObject*)pTargetNode;
		return pOther->GetName() == GetName();
	}

	void TextureObject::Dump()
	{
		Trace::out("%s\n", NameToString());
	}

	int TextureObject::GetRefCount() const
	{
		return refCount;
	}

	void TextureObject::IncrementRefCount()
	{
		refCount++;
	}

	void TextureObject::DecrementRefCount()
	{
		refCount--;
	}

	TextureObject::Name TextureObject::GetName() const
	{
		return name;
	}
}