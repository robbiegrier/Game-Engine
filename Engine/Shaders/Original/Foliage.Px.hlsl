#include "Default.hlsli"

Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

struct VS_OUTPUT_FOLIAGE
{
    float4 position : SV_POSITION;
    float4 positionModelSpace : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float4 norm : NORMAL;
    float4 tint : TINT;
    matrix runtimeInverse : RTINVERSE;
};

float4 main(VS_OUTPUT_FOLIAGE input) : SV_TARGET
{
    matrix inv = input.runtimeInverse;
    float4 msEyePos = mul(eyePositionWorld, inv);
    float distToEye = length(msEyePos - input.positionModelSpace);
    

    float4 litTextureSample = Light(input.position, input.positionModelSpace, input.norm, inv) *
        mainTexture.Sample(aSampler, input.textureCoordinate);
    

    float FogPercent = saturate((distToEye - FogStart) / FogRange);
    //litTextureSample = lerp(litTextureSample, FogColor, FogPercent);
    
    litTextureSample.xyz = lerp(litTextureSample.xyz, input.tint.xyz, .5f);
    
    return litTextureSample;
}

