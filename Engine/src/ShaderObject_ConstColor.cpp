//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "ShaderObject_ConstColor.h"
#include "Mesh.h"

// User generated headers... 
#include "ConstColor.Px.h"
#include "ConstColor.Vx.h"

ShaderObject_ConstColor::~ShaderObject_ConstColor()
{

}

ShaderObject_ConstColor::ShaderObject_ConstColor(ShaderObject_ConstColor::Name _name)
	: ShaderObject( _name)
{
	this->CreateVertexShader(g_ConstColor_VxShader, sizeof(g_ConstColor_VxShader));

	// Create the input layout for the vertex shader.
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, (uint32_t)VertexSlot::Position, offsetof(VertexPos,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	this->CreateInputLayout(g_ConstColor_VxShader, sizeof(g_ConstColor_VxShader), vertexLayoutDesc, sizeof(vertexLayoutDesc));

	this->CreatePixelShader(g_ConstColor_PxShader, sizeof(g_ConstColor_PxShader));

}




// --- End of File ---
