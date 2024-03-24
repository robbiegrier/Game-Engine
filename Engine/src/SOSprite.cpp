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
	}

	SOSprite::~SOSprite()
	{
	}

	void SOSprite::OnOpen(GraphicsObject* pObject)
	{
		static_cast<void>(pObject);

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
		Engine::ToggleBlending(false);
	}

	Camera* SOSprite::GetCamera() const
	{
		return CameraManager::GetCurrentCamera2D();
	}
}