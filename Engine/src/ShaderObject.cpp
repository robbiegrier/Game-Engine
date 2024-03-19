#include "ShaderObject.h"
#include "Engine.h"
#include "Mesh.h"
#include "CameraManager.h"
#include "GraphicsObject.h"

namespace Azul
{
	const char* ShaderObject::NameToString()
	{
		switch (name)
		{
		case Name::Default:
			return "Lit Texture (Default)";
		case Name::ColorByVertex:
			return "ColorByVertex";
		case Name::FlatTexture:
			return "FlatTexture";
		case Name::LightTexture:
			return "LightTexture";
		case Name::ConstColor:
			return "ConstColor";
		case Name::EditorVisual:
			return "EditorVisual";
		case Name::Null:
			return "Null";
		default:
			return "None";
		}
	}

	ShaderObject::ShaderObject()
		: name(Name::None), pConstBuffProjection{ nullptr }, pConstBuffView{ nullptr }, pConstBuffObjectUniversal{ nullptr }
	{
		pConstBuffProjection = Mesh::CreateConstantBuffer(sizeof(Mat4));
		pConstBuffView = Mesh::CreateConstantBuffer(sizeof(Mat4));
		pConstBuffObjectUniversal = Mesh::CreateConstantBuffer(sizeof(CBObjectUniversal));
	}

	ShaderObject::~ShaderObject()
	{
		SafeRelease(poInputLayout);
		SafeRelease(poVertexShader);
		SafeRelease(poPixelShader);
		SafeRelease(pConstBuffProjection);
		SafeRelease(pConstBuffView);
		SafeRelease(pConstBuffObjectUniversal);
	}

	void ShaderObject::Open(GraphicsObject* pObject)
	{
		Engine::GetContext()->VSSetShader(poVertexShader, nullptr, 0u);
		Engine::GetContext()->IASetInputLayout(poInputLayout);
		Engine::GetContext()->PSSetShader(poPixelShader, nullptr, 0u);

		Engine::GetContext()->UpdateSubresource(pConstBuffView, 0u, nullptr, &CameraManager::GetCurrentCamera()->GetViewMatrix(), 0u, 0u);
		Engine::GetContext()->UpdateSubresource(pConstBuffProjection, 0u, nullptr, &CameraManager::GetCurrentCamera()->GetProjMatrix(), 0u, 0u);
		Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::Projection, 1u, &pConstBuffProjection);
		Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::View, 1u, &pConstBuffView);

		// Object Specific
		CBObjectUniversal object;
		object.world = pObject->GetWorld();
		Engine::GetContext()->UpdateSubresource(pConstBuffObjectUniversal, 0, nullptr, &object, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::World, 1u, &pConstBuffObjectUniversal);

		OnOpen(pObject);
	}

	void ShaderObject::Close()
	{
		OnClose();
	}

	void ShaderObject::SetName(ShaderObject::Name inName)
	{
		name = inName;
	}

	ShaderObject::Name ShaderObject::GetName() const
	{
		return name;
	}

	void ShaderObject::Wash()
	{
	}

	bool ShaderObject::Compare(DLink* pTargetNode)
	{
		ShaderObject* pOther = (ShaderObject*)pTargetNode;
		return pOther->GetName() == GetName();
	}

	void ShaderObject::Dump()
	{
		Trace::out("%s\n", NameToString());
	}

	const char* ShaderObject::GetLatestProfile_VertexShader() const
	{
		switch (Engine::GetDevice()->GetFeatureLevel())
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "vs_5_0";
		case D3D_FEATURE_LEVEL_10_1:
			return "vs_4_1";
		case D3D_FEATURE_LEVEL_10_0:
			return "vs_4_0";
		case D3D_FEATURE_LEVEL_9_3:
			return "vs_4_0_level_9_3";
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_1:
			return "vs_4_0_level_9_1";
		default:
			assert(false);
		}

		return "";
	}

	const char* ShaderObject::GetLatestProfile_PixelShader() const
	{
		switch (Engine::GetDevice()->GetFeatureLevel())
		{
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_11_0:
			return "ps_5_0";
		case D3D_FEATURE_LEVEL_10_1:
			return "ps_4_1";
		case D3D_FEATURE_LEVEL_10_0:
			return "ps_4_0";
		case D3D_FEATURE_LEVEL_9_3:
			return "ps_4_0_level_9_3";
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_1:
			return "ps_4_0_level_9_1";
		}
		return "";
	}

	void ShaderObject::CreateVertexShader(const void* pCompiledVertexShaderObject, size_t codeSize)
	{
		assert(pCompiledVertexShaderObject);

		HRESULT hr = Engine::GetDevice()->CreateVertexShader(pCompiledVertexShaderObject, codeSize, nullptr, &poVertexShader);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		assert(std::string("vs_5_0") == (GetLatestProfile_VertexShader()));
		assert(poVertexShader);
	}

	void ShaderObject::CreatePixelShader(const unsigned char* pCompiledPixelShaderObject, size_t codeSize)
	{
		assert(pCompiledPixelShaderObject);

		HRESULT hr = Engine::GetDevice()->CreatePixelShader(pCompiledPixelShaderObject, codeSize, nullptr, &poPixelShader);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		assert(std::string("ps_5_0") == (GetLatestProfile_PixelShader()));
		assert(poPixelShader);
	}

	void ShaderObject::CreateInputLayout(const unsigned char* pCompiledVertexShaderObject, size_t codeSize, D3D11_INPUT_ELEMENT_DESC* pDesc, unsigned int SizeInBytes)
	{
		assert(pCompiledVertexShaderObject);
		assert(pDesc);

		HRESULT hr = Engine::GetDevice()->CreateInputLayout(pDesc, SizeInBytes / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompiledVertexShaderObject, codeSize, &poInputLayout);
		assert(SUCCEEDED(hr));
		static_cast<void>(hr);

		assert(poInputLayout);
	}
}