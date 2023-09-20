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

cbuffer AA3 : register(b3) // slot 3 (ConstantBufferSlot::Color)
{
	float4 lightColor;
}

cbuffer AA4 : register(b4) // slot 4 (ConstantBufferSlot::LightPos)
{
	float4 lightPos;
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

struct AppData_norm  // vertex constant buffer, slot 2 (VertexSlot::Norm)
{
	float3 norm: NORMAL;  // norm location
};

struct AppData_tex  // vertex constant buffer, slot 3 (VertexSlot::TexCoord)
{
	float2 tex: TEXCOORD;  // uv location
};

// ------------------------------------------------------------
// Sematics Vertex Output:
//      COLOR - Input to the Pixel Shader
//      SV_POSIION - System Value goes to the raster stage
// ------------------------------------------------------------
struct VertexShaderOutput
{
	float2 tex : TEXCOORD;
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

// ----------------------------------------------------------
//  Vertex Shader Main()
// ----------------------------------------------------------
VertexShaderOutput LightTexture_Main(AppData_pos inPos, AppData_tex inTex, AppData_norm inNorm)
{
	VertexShaderOutput outValue;

	// Mat = World * View * Proj
	matrix Mat = mul(mul(worldMatrix, viewMatrix), projectionMatrix);

	//   position = vec4(inPos.xyz,1) * Mat
	//   color    = vec4(inColor.rgb,1)
	outValue.position = mul(float4(inPos.position, 1.0f), Mat);

	// Texture
	outValue.tex = inTex.tex;

	// color calculation
	matrix mvMatrix = mul(worldMatrix, viewMatrix);
	float3x3 mNormalMatrix;

	mNormalMatrix[0] = normalize(mvMatrix[0].xyz);
	mNormalMatrix[1] = normalize(mvMatrix[1].xyz);
	mNormalMatrix[2] = normalize(mvMatrix[2].xyz);
	float3 vA = mul(inNorm.norm.xyz, mNormalMatrix);
	float3 vNorm = normalize(vA);

	// Get vLightDir
	float4 ecPosition;
	float3 ecPosition3;
	ecPosition = mul(float4(inPos.position, 1.0f), mvMatrix);
	ecPosition3 = ecPosition.xyz / ecPosition.w;
	float3 vLightDir = normalize(lightPos.xyz - ecPosition3);

	// Get vFragColor
	float fDot = max(0.0, dot(vNorm, vLightDir));
	outValue.color.xyz = lightColor.xyz * fDot;
	outValue.color.w = 1.0f;

	return outValue;
}

// --- End of File ---