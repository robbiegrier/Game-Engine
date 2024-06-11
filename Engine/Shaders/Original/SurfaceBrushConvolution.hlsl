#include "SurfaceBrushBase.hlsli"

static const uint CONVOLUTION_RESOLUTION = 33u;
static const uint CONVOLUTION_SIZE = CONVOLUTION_RESOLUTION * CONVOLUTION_RESOLUTION;
static const uint CONVOLUTION_OFFSET = floor(CONVOLUTION_RESOLUTION / 2u);
static const uint2 CONVOLUTION_OFFSET_COORD = uint2(CONVOLUTION_OFFSET, CONVOLUTION_OFFSET);

RWTexture2D<float> convolutionMatrix : register(u1);

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
        uint2 convOriginTerrainCoord = weightsCoord - CONVOLUTION_OFFSET_COORD;
        
        float4 sum = float4(0.f, 0.f, 0.f, 0.f);
        
        [unroll]
        for (uint ci = 0; ci < CONVOLUTION_RESOLUTION; ci++)
        {
            [unroll]
            for (uint cj = 0; cj < CONVOLUTION_RESOLUTION; cj++)
            {
                uint2 coordConvMatrix = uint2(ci, cj);
                uint2 coordTerrainSurfaceWeights = convOriginTerrainCoord + coordConvMatrix;
                float convolutionWeight = convolutionMatrix[coordConvMatrix];
                float4 surfaceWeights = surfaceLayerWeights[coordTerrainSurfaceWeights];
                
                sum += (surfaceWeights * convolutionWeight);
            }
        }
        
        float4 deltaWeight = sum - surfaceLayerWeights[weightsCoord];
        deltaWeight *= force * 10000.f;
        deltaWeight = lerp(deltaWeight, float4(0.f, 0.f, 0.f, 0.f), dist / radius);
        
        surfaceLayerWeights[weightsCoord] += deltaWeight;
        surfaceLayerWeights[weightsCoord] = clamp(surfaceLayerWeights[weightsCoord], float4(0.f, 0.f, 0.f, 0.f), float4(1.f, 1.f, 1.f, 1.f));
    }
}