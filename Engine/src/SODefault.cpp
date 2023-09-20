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
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(Data_LightParams);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferLightParams);
		assert(SUCCEEDED(hr));

		D3D11_BUFFER_DESC bd2;
		bd2.Usage = D3D11_USAGE_DEFAULT;
		bd2.ByteWidth = sizeof(Data_WorldAndMaterial);
		bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd2.CPUAccessFlags = 0;
		bd2.MiscFlags = 0;
		bd2.StructureByteStride = 0;
		hr = Engine::GetDevice()->CreateBuffer(&bd2, nullptr, &mpBuffWordAndMaterial);
		assert(SUCCEEDED(hr));
	}

	SODefault::~SODefault()
	{
		SafeRelease(mpBuffWordAndMaterial);
		SafeRelease(mpBufferLightParams);
	}

	void SODefault::SendWorldAndMaterial(const Mat4& world, const Vec3& amb, const Vec3& dif, const Vec3& sp)
	{
		Data_WorldAndMaterial wm;
		wm.World = world;
		wm.WorldInv = world.getInv();
		wm.Mat.Ambient = Vec4(amb, 1.0f);
		wm.Mat.Diffuse = Vec4(dif, 1.0f);
		wm.Mat.Specular = Vec4(sp, 1.0f);

		Engine::GetContext()->UpdateSubresource(mpBuffWordAndMaterial, 0, nullptr, &wm, 0, 0);
	}

	void SODefault::SetPointLightParameters(const Vec3& pos, float r, const Vec3& att, const Vec3& amb, const Vec3& dif, const Vec3& sp)
	{
		PointLightData.Light.Ambient = Vec4(amb, 0.0f);
		PointLightData.Light.Diffuse = Vec4(dif, 0.0f);
		PointLightData.Light.Specular = Vec4(sp, 0.0f);
		PointLightData.Position = Vec4(pos, 1.0f);
		PointLightData.Attenuation = Vec4(att, 0.0f);
		PointLightData.Range = r;
	}

	void SODefault::OnOpen(GraphicsObject* pObject)
	{
		pObject->GetModel()->ActivateModel();
		pObject->GetModel()->TransferConstantBuffer(CameraManager::GetCurrentCamera(), pObject->GetWorld());

		SendWorldAndMaterial(pObject->GetWorld(), Vec3(0.4, 0.4, 0.4), Vec3(1.0, 1.0, 1), Vec3(0.5, 0.5, 1));

		Vec3 eyePos;
		CameraManager::GetCurrentCamera()->GetLocation(eyePos);

		Data_LightParams dl;
		dl.PntLight = PointLightData;
		dl.EyePosWorld = Vec4(eyePos, 1.0f);
		Engine::GetContext()->UpdateSubresource(mpBufferLightParams, 0, nullptr, &dl, 0, 0);

		Engine::GetContext()->VSSetConstantBuffers(5, 1, &mpBufferLightParams);
		Engine::GetContext()->VSSetConstantBuffers(6, 1, &mpBuffWordAndMaterial);

		Engine::GetContext()->PSSetConstantBuffers(5, 1, &mpBufferLightParams);
		Engine::GetContext()->PSSetConstantBuffers(6, 1, &mpBuffWordAndMaterial);
	}
}