#pragma pack_matrix( row_major )

static const int MaxLights = 15;

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // Hack: w holds the specular power
};

struct PointLight
{
    Material LightInt;
    float4 Position;
    float4 Attenuation;
    float Range;
};

struct DirectionalLight
{
    Material LightInt;
    float4 Direction;
};

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

cbuffer LightParameters : register(b5)
{
    int numPointLights;
    PointLight pointlights[MaxLights];
    DirectionalLight directionalLight;
    float4 eyePositionWorld;
};

cbuffer InstanceData : register(b6)
{
    float4x4 inverse;
    Material material;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 positionModelSpace : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float4 norm : NORMAL;
};