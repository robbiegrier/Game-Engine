#include "SOSprite.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "Engine.h"
#include "CameraManager.h"
#include "GOSprite.h"

// User generated headers...
#include "Sprite.Px.h"
#include "Sprite.Vx.h"

namespace Azul
{
	SOSprite::SOSprite()
	{
		CreateVertexShader(g_Sprite_VxShader, sizeof(g_Sprite_VxShader));

		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateInputLayout(g_Sprite_VxShader, sizeof(g_Sprite_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));
		CreatePixelShader(g_Sprite_PxShader, sizeof(g_Sprite_PxShader));

		HRESULT hr;

		CD3D11_BLEND_DESC bsd;
		ZeroMemory(&bsd, sizeof(CD3D11_BLEND_DESC));
		bsd.RenderTarget[0].BlendEnable = TRUE;
		bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		hr = Engine::GetDevice()->CreateBlendState(&bsd, &pBlendStateSprite);
		assert(SUCCEEDED(hr));
	}

	SOSprite::~SOSprite()
	{
		SafeRelease(pBlendStateSprite);
	}

	void SOSprite::OnOpen(GraphicsObject* pObject)
	{
		Engine::GetContext()->OMSetBlendState(pBlendStateSprite, blendFactor, sampleMask);

		GOSprite* pGOSprite = (GOSprite*)pObject;

		Trans TransToOriginLowerLeft((float)-GetCamera()->GetScreenWidth() / 2.0f, (float)-GetCamera()->GetScreenHeight() / 2.0f, 0.0f);
		Mat4 tmpMatrix = pGOSprite->GetWorld() * TransToOriginLowerLeft;

		CBObjectUniversal object;
		object.world = tmpMatrix;
		Engine::GetContext()->UpdateSubresource(pConstBuffObjectUniversal, 0, nullptr, &object, 0, 0);
		Engine::GetContext()->VSSetConstantBuffers((uint32_t)ConstantBufferSlot::World, 1u, &pConstBuffObjectUniversal);
	}

	void SOSprite::OnClose()
	{
		Engine::GetContext()->OMSetBlendState(nullptr, blendFactor, sampleMask);
	}

	Camera* SOSprite::GetCamera() const
	{
		return CameraManager::GetCurrentCamera2D();
	}
}