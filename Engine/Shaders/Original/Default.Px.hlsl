#include "Default.hlsli"

Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return Light(input.position, input.positionModelSpace, input.norm) *
        mainTexture.Sample(aSampler, input.textureCoordinate);
}

