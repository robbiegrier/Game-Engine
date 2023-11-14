//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_ColorByVertex.h"
#include "Mesh.h"

// User generated headers... 
#include "ColorByVertex.Px.h"
#include "ColorByVertex.Vx.h"

ShaderObject_ColorByVertex::~ShaderObject_ColorByVertex()
{

}

ShaderObject_ColorByVertex::ShaderObject_ColorByVertex(ShaderObject_ColorByVertex::Name _name)
	: ShaderObject( _name)
{
	this->CreateVertexShader(g_ColorByVertex_VxShader, sizeof(g_ColorByVertex_VxShader));

	// Create the input layout for the vertex shader.
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Color, offsetof(VertexColor,Color),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	this->CreateInputLayout(g_ColorByVertex_VxShader, sizeof(g_ColorByVertex_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));

	this->CreatePixelShader(g_ColorByVertex_PxShader, sizeof(g_ColorByVertex_PxShader));

}




// --- End of File ---
