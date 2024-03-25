#include "Phong.hlsli"

#define BONE_COUNT_MAX 128

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

cbuffer AA4 : register(b8) // slot 8 (ConstantBufferSlot::vsBoneWorld)
{
    matrix BoneWorld[BONE_COUNT_MAX];
}

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 positionModelSpace : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float4 norm : NORMAL;
};