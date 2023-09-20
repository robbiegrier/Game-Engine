//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#pragma pack_matrix( row_major )

// ------------------------------------------
// bx - b is constant buffer, x - slot
// ------------------------------------------

cbuffer AA0: register(b0)  // slot 0  (ConstantBufferSlot::Projection)
{
	matrix projectionMatrix;
}

cbuffer AA1 : register(b1) // slot 1 (ConstantBufferSlot::View)
{
	matrix viewMatrix;
}

cbuffer AA2 : register(b2) // slot 2 (ConstantBufferSlot::World)
{
	matrix worldMatrix;
}

cbuffer AA3 : register( b3 ) // slot 3 (ConstantBufferSlot::Color)
{
	float4 lightColor;
}


// ------------------------------------------------------
// Sematics Vertex Input:
//      POSITION - used in vertex layout description
//      COLOR - used in vertex layout description
// ------------------------------------------------------

struct AppData_pos  //  vertex constant buffer, slot 0 (VertexSlot::Position)
{
	float3 position : POSITION;
};



// ------------------------------------------------------------
// Sematics Vertex Output:
//      COLOR - Input to the Pixel Shader
//      SV_POSIION - System Value goes to the raster stage
// ------------------------------------------------------------
struct VertexShaderOutput
{
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

// ----------------------------------------------------------
//  Vertex Shader Main()
// ----------------------------------------------------------
VertexShaderOutput ConstColor_Main(AppData_pos inPos)
{
	VertexShaderOutput outValue;

	// Mat = World * View * Proj
	matrix Mat = mul(mul(worldMatrix, viewMatrix), projectionMatrix);

	//   position = vec4(inPos.xyz,1) * Mat
	//   color    = vec4(inColor.rgb,1)
	outValue.position = mul(float4(inPos.position, 1.0f), Mat);
	outValue.color = float4( lightColor.xyz, 1.0f);

	return outValue;
}

// --- End of File ---
