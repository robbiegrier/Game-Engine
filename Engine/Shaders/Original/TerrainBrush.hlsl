#include "TerrainBrushBase.hlsli"

[numthreads(TERRAIN_BRUSH_THREADS, TERRAIN_BRUSH_THREADS, 1u)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint2 coord = GetCoordinates(DTid);
    float dist = GetDistanceToBrushCenter(coord);
    
    if (dist < radius)
    {
        float deltaHeight = lerp(force * radius, 0.f, dist / radius);
        textureData[coord] += deltaHeight;
        textureData[coord] = clamp(textureData[coord], 0.f, 1.f);
    }
}