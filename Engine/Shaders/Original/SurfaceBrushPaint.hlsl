#include "SurfaceBrushBase.hlsli"

cbuffer PaintArgs : register(b1)
{
    float4 layerMask;
}

[numthreads(TERRAIN_BRUSH_THREADS, TERRAIN_BRUSH_THREADS, 1u)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint2 coord = GetCoordinates(DTid);
    float dist = GetDistanceToBrushCenter(coord);
    
    if (dist < radius)
    {
        float2 worldPos2d = GetWorldPos2D(coord);
        float2 worldPos2dRelativeOrigin = worldPos2d - GetWorldPos2D(uint2(0, 0));
        float2 worldPos2dNormSplatting = worldPos2dRelativeOrigin / float(SPLATTING_RESOLUTION);
        uint2 weightsCoord = uint2(floor(worldPos2dNormSplatting));
        
        float deltaPaint = lerp(force * 10000.f, 0.f, dist / (radius));
        surfaceLayerWeights[weightsCoord] = lerp(surfaceLayerWeights[weightsCoord], layerMask, deltaPaint);

    }
}