#include "TerrainBrushBase.hlsli"

cbuffer NoiseArgs : register(b1)
{
    float noiseScale;
    float pad10;
    float pad11;
    float pad12;
}

Texture2D<float> noiseMatrix : register(t0);
SamplerState noiseSampler : register(s0);

[numthreads(TERRAIN_BRUSH_THREADS, TERRAIN_BRUSH_THREADS, 1u)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint2 coord = GetCoordinates(DTid);
    float dist = GetDistanceToBrushCenter(coord);
    float2 worldPos2d = GetWorldPos2D(coord);
    
    float2 worldPosBrushOrigin = GetWorldPos2D(GetCoordinates(uint3(0, 0, 0)));
    float2 worldPosRelative2d = worldPos2d - worldPosBrushOrigin;
    
    if (dist < radius)
    {
        float maxPos = radius * 2;
        float2 normPos = worldPosRelative2d / maxPos;
        
        float perlin = noiseMatrix.SampleLevel(noiseSampler, normPos, 0);
        
        float deltaHeight = lerp((force * radius) * perlin, 0.f, dist / (radius));
        
        textureData[coord] += deltaHeight;
        textureData[coord] = clamp(textureData[coord], 0.f, 1.f);
    }
}