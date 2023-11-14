//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_Null.h"
#include "Mesh.h"

// User generated headers... 
#include "Null.Px.h"
#include "Null.Vx.h"


ShaderObject_Null::~ShaderObject_Null()
{

}

ShaderObject_Null::ShaderObject_Null(ShaderObject_Null::Name _name)
	: ShaderObject( _name)
{
	this->CreateVertexShader(g_Null_VxShader, sizeof(g_Null_VxShader));

	// Create the input layout for the vertex shader.
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Color, offsetof(VertexColor,Color),  D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	this->CreateInputLayout(g_Null_VxShader, sizeof(g_Null_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));

	this->CreatePixelShader(g_Null_PxShader, sizeof(g_Null_PxShader));

}




// --- End of File ---
