#include "SOFlatTexture.h"
#include "Mesh.h"

// User generated headers...
#include "FlatTexture.Px.h"
#include "FlatTexture.Vx.h"

namespace Azul
{
	SOFlatTexture::SOFlatTexture()
	{
		this->CreateVertexShader(g_FlatTexture_VxShader, sizeof(g_FlatTexture_VxShader));

		// Create the input layout for the vertex shader.
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD"   , 0, DXGI_FORMAT_R32G32_FLOAT, (uint32_t)VertexSlot::TexCoord, offsetof(VertexTexCoord,TexCoord),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		this->CreateInputLayout(g_FlatTexture_VxShader, sizeof(g_FlatTexture_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));

		this->CreatePixelShader(g_FlatTexture_PxShader, sizeof(g_FlatTexture_PxShader));
	}
}