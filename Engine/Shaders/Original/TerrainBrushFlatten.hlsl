#include "TerrainBrushBase.hlsli"

cbuffer FlattenArgs : register(b1)
{
    float flattenToHeight;
    float pad10;
    float pad11;
    float pad12;
}

[numthreads(TERRAIN_BRUSH_THREADS, TERRAIN_BRUSH_THREADS, 1u)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint2 coord = GetCoordinates(DTid);
    float dist = GetDistanceToBrushCenter(coord);
    
    if (dist < radius)
    {
        textureData[coord] = lerp(textureData[coord], flattenToHeight, force * 1000.f);
    }
}