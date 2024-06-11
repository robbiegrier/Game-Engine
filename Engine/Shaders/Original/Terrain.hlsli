#include "Phong.hlsli"

#define NUM_SURFACES 4u

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

cbuffer TerrainParameters : register(b8)
{
    float gridCellSize;
    float halfWorldScale;
    float heightScale;
    float pad81;
};

cbuffer TerrainBrushArgs : register(b9)
{
    float4 raycastPos;
    float brushRadius;
    float pad90;
    float pad91;
    float pad92;
}

//Texture2D mainTexture : register(t0);

// The height of each vertex represented as a texture of single floats
Texture2D<float> heightfield : register(t1);

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 positionModelSpace : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float4 norm : NORMAL;
};

uint2 GetCellAt2D(float3 worldPos)
{
    float3 terrainZeroZeroLocation = float3(-halfWorldScale, 0.f, -halfWorldScale);
    float3 zeroZeroToLocation = worldPos - terrainZeroZeroLocation;

    const float scaledX = zeroZeroToLocation.x / gridCellSize;
    const float scaledZ = zeroZeroToLocation.z / gridCellSize;

    return uint2(scaledX, scaledZ);
}

float3 GetWorldPos3D(uint2 coord)
{
    float3 output = float3(-halfWorldScale, 0.f, -halfWorldScale);
    output += float3(coord.x, 0.f, coord.y) * gridCellSize;
    output.y = heightfield[coord] * heightScale;
    return output;
}