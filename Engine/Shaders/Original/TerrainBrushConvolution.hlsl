#include "TerrainBrushBase.hlsli"


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
        uint2 convOriginTerrainCoord = coord - CONVOLUTION_OFFSET_COORD;
        
        float sum = 0.f;
        
        [unroll]
        for (uint ci = 0; ci < CONVOLUTION_RESOLUTION; ci++)
        {
            [unroll]
            for (uint cj = 0; cj < CONVOLUTION_RESOLUTION; cj++)
            {
                uint2 coordConvMatrix = uint2(ci, cj);
                uint2 coordTerrain = convOriginTerrainCoord + coordConvMatrix;
                
                float weight = convolutionMatrix[coordConvMatrix];
                float terrainHeight = textureData[coordTerrain];

                sum += (terrainHeight * weight);
            }
        }
        
        float deltaHeight = sum - textureData[coord];
        deltaHeight *= force * 10000.f;
        deltaHeight = lerp(deltaHeight, 0.f, dist / radius);
        
        textureData[coord] += deltaHeight;
        textureData[coord] = clamp(textureData[coord], 0.f, 1.f);
    }
}