//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#pragma pack_matrix( row_major )

// ------------------------------------------
// bx - b is constant buffer, x - slot
// ------------------------------------------

cbuffer AA0 : register(b0) // slot 0  (ConstantBufferSlot::Projection)
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

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // Hack: w holds the specular power
};

struct PhongADS
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

struct PointLight
{
    PhongADS LightInt;
    float4 Position;
    float4 Attenuation;
    float Range;
};

cbuffer LightParameters : register(b5)
{
    PointLight pointlight;
    float4 EyePosWorld;
};

cbuffer InstanceData : register(b6)
{
    float4x4 World;
    float4x4 WorldInv;
    Material Mater;
};

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
    float3 norm : NORMAL; // norm location
};

struct AppData_tex  // vertex constant buffer, slot 3 (VertexSlot::TexCoord)
{
    float2 tex : TEXCOORD; // uv location
};

// ------------------------------------------------------------
// Sematics Vertex Output:
//      COLOR - Input to the Pixel Shader
//      SV_POSIION - System Value goes to the raster stage
// ------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 PosMS : POSITION;
    float2 Tex : TEXCOORD;
    float4 Norm : NORMAL;
};

// ----------------------------------------------------------
//  Vertex Shader Main()
// ----------------------------------------------------------
VS_OUTPUT main(AppData_pos inPos, AppData_tex inTex, AppData_norm inNorm)
{
    VS_OUTPUT output;
    output.PosMS = float4(inPos.position, 1.0f); // We pass along the raw model space position 
    output.Norm = float4(inNorm.norm, 0.0f); // and the face normal

    output.Pos = mul(float4(inPos.position, 1.0f), World);
    output.Pos = mul(output.Pos, viewMatrix);
    output.Pos = mul(output.Pos, projectionMatrix);
    output.Tex = inTex.tex;

    return output;
}

// --- End of File ---