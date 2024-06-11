#include "ComputeShaderBrush.hlsli"

RWTexture2D<float> textureData : register(u0);

void AddHeight(uint2 coord, float deltaHeight)
{
    textureData[coord] += deltaHeight;
    textureData[coord] = clamp(textureData[coord], 0.f, 1.f);
}