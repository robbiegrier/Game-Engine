#include "Default.hlsli"

Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 litTextureSample = Light(input.position, input.positionModelSpace, input.norm, inverse) *
        mainTexture.Sample(aSampler, input.textureCoordinate);
    
    float4 msEyePos = mul(eyePositionWorld, inverse);
    float distToEye = length(msEyePos - input.positionModelSpace);
    float FogPercent = saturate((distToEye - FogStart) / FogRange);
    
    float scale = length(inverse._m00_m01_m02);

    litTextureSample = lerp(litTextureSample, FogColor, FogPercent / scale);
    return litTextureSample;
}

