#include "SOColorByVertex.h"
#include "Mesh.h"
#include "ColorByVertex.Px.h"
#include "ColorByVertex.Vx.h"

namespace Azul
{
	SOColorByVertex::SOColorByVertex()
	{
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Color, offsetof(VertexColor,Color),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CreateVertexShader(g_ColorByVertex_VxShader, sizeof(g_ColorByVertex_VxShader));
		CreateInputLayout(g_ColorByVertex_VxShader, sizeof(g_ColorByVertex_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));
		CreatePixelShader(g_ColorByVertex_PxShader, sizeof(g_ColorByVertex_PxShader));
	}
}