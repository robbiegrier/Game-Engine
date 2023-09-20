#include "SOLightTexture.h"
#include "Mesh.h"

// User generated headers...
#include "LightTexture.Px.h"
#include "LightTexture.Vx.h"

namespace Azul
{
	SOLightTexture::SOLightTexture()
		: ShaderObject()
	{
		this->CreateVertexShader(g_LightTexture_VxShader, sizeof(g_LightTexture_VxShader));

		// Create the input layout for the vertex shader.
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Norm, offsetof(VertexNorm,Norm),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		this->CreateInputLayout(g_LightTexture_VxShader, sizeof(g_LightTexture_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));

		this->CreatePixelShader(g_LightTexture_PxShader, sizeof(g_LightTexture_PxShader));
	}
}