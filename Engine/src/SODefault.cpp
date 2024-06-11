#include "SODefault.h"
#include "Mesh.h"
#include "Engine.h"
#include "CameraManager.h"
#include "GraphicsObject.h"

// User generated headers...
#include "Default.Px.h"
#include "Default.Vx.h"

namespace Azul
{
	SODefault::SODefault()
	{
		this->CreateVertexShader(g_Default_VxShader, sizeof(g_Default_VxShader));

		// Create the input layout for the vertex shader.
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Norm, offsetof(VertexNorm,Norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateInputLayout(g_Default_VxShader, sizeof(g_Default_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));
		CreatePixelShader(g_Default_PxShader, sizeof(g_Default_PxShader));

		HRESULT hr;
		D3D11_BUFFER_DESC buffDescLightscape;
		buffDescLightscape.Usage = D3D11_USAGE_DEFAULT;
		buffDescLightscape.ByteWidth = sizeof(CBLightscape);
		buffDescLightscape.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescLightscape.CPUAccessFlags = 0;
		buffDescLightscape.MiscFlags = 0;
		buffDescLightscape.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescLightscape, nullptr, &pConstBuffLightscape);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC buffDescObject;
		buffDescObject.Usage = D3D11_USAGE_DEFAULT;
		buffDescObject.ByteWidth = sizeof(CBObject);
		buffDescObject.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescObject.CPUAccessFlags = 0;
		buffDescObject.MiscFlags = 0;
		buffDescObject.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescObject, nullptr, &pConstBuffObject);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC buffDescFog;
		buffDescFog.Usage = D3D11_USAGE_DEFAULT;
		buffDescFog.ByteWidth = sizeof(CBFog);
		buffDescFog.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffDescFog.CPUAccessFlags = 0;
		buffDescFog.MiscFlags = 0;
		buffDescFog.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&buffDescFog, nullptr, &pConstBuffFog);
		assert(SUCCEEDED(hr));
	}

	SODefault::~SODefault()
	{
		SafeRelease(pConstBuffObject);
		SafeRelease(pConstBuffLightscape);
		SafeRelease(pConstBuffFog);
	}

	void SODefault::SetFogParameters(float start, float range, const Vec4& col)
	{
		fog.FogStart = start;
		fog.FogRange = range;
		fog.FogColor = col;
	}

	void SODefault::SetCurrentObject(GraphicsObject* pObject)
	{
		Vec3 testAmb = Vec3(0.4f, 0.4f, 0.4f);
		Vec3 testDiff = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 testSpec = Vec3(0.5f, 0.5f, 1.0f);

		CBObject object;
		object.inverse = pObject->GetWorld().getInv();/*
		object.inverse.set(Row4::i0, object.inverse.get(Row4::i0).getNorm());
		object.inverse.set(Row4::i1, object.inverse.get(Row4::i1).getNorm());
		object.inverse.set(Row4::i2, object.inverse.get(Row4::i2).getNorm());*/
		object.material.ambient = Vec4(testAmb, 1.0f);
		object.material.diffuse = Vec4(testDiff, 1.0f);
		object.material.specular = Vec4(testSpec, 1.0f);

		Engine::GetContext()->UpdateSubresource(pConstBuffObject, 0, nullptr, &object, 0, 0);
	}

	void SODefault::OnOpen(GraphicsObject* pObject)
	{
		// Per Frame Lightscape data (can be optimized out of per-object rendering)
		CBLightscape lightscape;
		lightscape.numPointLights = LightSystem::MAX_LIGHTS;
		memcpy(lightscape.pointLights, LightSystem::GetPointLights(), sizeof(PointLight) * LightSystem::MAX_LIGHTS);
		lightscape.directionalLight = LightSystem::GetDirectionalLight();
		lightscape.eyePositionWorld = Vec4(GetCamera()->GetLocation(), 1.0f);
		Engine::GetContext()->UpdateSubresource(pConstBuffLightscape, 0, nullptr, &lightscape, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Lightscape, 1, &pConstBuffLightscape);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Lightscape, 1, &pConstBuffLightscape);

		Engine::GetContext()->UpdateSubresource(pConstBuffFog, 0, nullptr, &fog, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Fog, 1, &pConstBuffFog);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Fog, 1, &pConstBuffFog);

		// Per object data
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Object, 1, &pConstBuffObject);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Object, 1, &pConstBuffObject);
		SetCurrentObject(pObject);
	}
}