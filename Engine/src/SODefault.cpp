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
	}

	SODefault::~SODefault()
	{
		SafeRelease(pConstBuffObject);
		SafeRelease(pConstBuffLightscape);
	}

	void SODefault::SetPointLightParameters(const Vec3& pos, float r, const Vec3& att, const Vec3& amb, const Vec3& dif, const Vec3& sp)
	{
		pointLight.light.ambient = Vec4(amb, 0.0f);
		pointLight.light.diffuse = Vec4(dif, 0.0f);
		pointLight.light.specular = Vec4(sp, 0.0f);
		pointLight.position = Vec4(pos, 1.0f);
		pointLight.attenuation = Vec4(att, 0.0f);
		pointLight.range = r;
	}

	void SODefault::SetCurrentObject(GraphicsObject* pObject)
	{
		Vec3 testAmb = Vec3(0.4f, 0.4f, 0.4f);
		Vec3 testDiff = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 testSpec = Vec3(0.5f, 0.5f, 1.0f);

		CBObject object;
		object.world = pObject->GetWorld();
		object.inverse = pObject->GetWorld().getInv();
		object.material.ambient = Vec4(testAmb, 1.0f);
		object.material.diffuse = Vec4(testDiff, 1.0f);
		object.material.specular = Vec4(testSpec, 1.0f);

		Engine::GetContext()->UpdateSubresource(pConstBuffObject, 0, nullptr, &object, 0, 0);
	}

	void SODefault::OnOpen(GraphicsObject* pObject)
	{
		// Per Frame Lightscape data (can be optimized out of per-object rendering)
		CBLightscape lightscape;
		lightscape.pointLight = pointLight;
		lightscape.eyePositionWorld = Vec4(CameraManager::GetCurrentCamera()->GetLocation(), 1.0f);
		Engine::GetContext()->UpdateSubresource(pConstBuffLightscape, 0, nullptr, &lightscape, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Lightscape, 1, &pConstBuffLightscape);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Lightscape, 1, &pConstBuffLightscape);

		// Per object data
		Engine::GetContext()->VSSetConstantBuffers((UINT)ConstBuffSlot::Object, 1, &pConstBuffObject);
		Engine::GetContext()->PSSetConstantBuffers((UINT)ConstBuffSlot::Object, 1, &pConstBuffObject);
		pObject->GetModel()->ActivateModel();
		SetCurrentObject(pObject);
	}
}