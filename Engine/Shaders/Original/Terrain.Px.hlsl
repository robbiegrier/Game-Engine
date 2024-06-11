#include "Terrain.hlsli"

SamplerState aSampler : register(s0);
SamplerState normSampler : register(s1);

// N textures of single floats to represent the weight of each texture surface at that coordinate
Texture2D<float4> surfaceLayerWeights : register(t2);

// N textures of real image data for surfaces. surfaceLayerWeights[i].xy gives the weight of surfaceLayerTextures[i] at (x, y)
Texture2D surfaceLayerTextures[NUM_SURFACES] : register(t6);

Texture2D surfaceLayerNormals[NUM_SURFACES] : register(t10);

float4 GetAverageTexture(float2 worldPos, float2 textureCoordinate)
{
    float4 averageTexture = float4(0.f, 0.f, 0.f, 0.f);
    float resolution = ((halfWorldScale * 2.f) / gridCellSize);
    float2 terrainZeroZeroLocation = float2(-halfWorldScale, -halfWorldScale);
    float2 zeroZeroToLocation = worldPos - terrainZeroZeroLocation;
    float2 normWorldPos = zeroZeroToLocation / resolution;
    float4 weightVector = surfaceLayerWeights.Sample(aSampler, normWorldPos);

    [unroll]
    for (uint i = 0; i < NUM_SURFACES; i++)
    {
        float weight = weightVector[i];
        
        if (weight > 0.001f)
        {
            float4 surfaceLayerSample = surfaceLayerTextures[i].Sample(aSampler, textureCoordinate);
            weight = clamp(weight, 0.f, 1.f);
            averageTexture = averageTexture + (surfaceLayerSample * weight);
        }
    }
    
    return averageTexture;
}

float4 GetAverageNormal(float2 worldPos, float2 textureCoordinate)
{
    float4 average = float4(0.f, 0.f, 0.f, 0.f);
    float resolution = ((halfWorldScale * 2.f) / gridCellSize);
    float2 terrainZeroZeroLocation = float2(-halfWorldScale, -halfWorldScale);
    float2 zeroZeroToLocation = worldPos - terrainZeroZeroLocation;
    float2 normWorldPos = zeroZeroToLocation / resolution;
    float4 weightVector = surfaceLayerWeights.Sample(aSampler, normWorldPos);

    [unroll]
    for (uint i = 0; i < NUM_SURFACES; i++)
    {
        float weight = weightVector[i];
        
        if (weight > 0.001f)
        {
            float4 surfaceLayerSample = surfaceLayerNormals[i].Sample(normSampler, textureCoordinate);
            surfaceLayerSample = (surfaceLayerSample * 2.f) - 1.f;
            weight = clamp(weight, 0.f, 1.f);
            average = average + (surfaceLayerSample * weight);
        }
    }
    
    return average;
}

float4 main(VS_OUTPUT input) : SV_TARGET
{   
    float4 msEyePos = mul(eyePositionWorld, inverse);
    float distToEye = length(msEyePos - input.positionModelSpace);
    
    float3 normalSampled = input.norm.xyz;
    
    float bumpDist = 250.f;
    if (distToEye < bumpDist)
    {
        float3 rawSample = GetAverageNormal(input.positionModelSpace.xz, input.textureCoordinate).xyz;
        rawSample.xyz = -rawSample.xyz * 1.1f;
        rawSample.z = 0;
        normalSampled.xyz += lerp(float3(0, 0, 0), rawSample, 1.f - (distToEye / bumpDist));
        normalSampled.xyz = normalize(normalSampled.xyz);
    }

    float4 averageTexture = GetAverageTexture(input.positionModelSpace.xz, input.textureCoordinate);
    float4 litSample = Light(input.position, input.positionModelSpace, float4(normalSampled, 0.f), inverse);
    float4 litTextureSample = litSample * averageTexture;

    float FogPercent = saturate((distToEye - FogStart) / FogRange);
    litTextureSample = lerp(litTextureSample, FogColor, FogPercent );
    
    float distMouse = distance(input.positionModelSpace.xz, raycastPos.xz);
    if (distMouse < brushRadius)
    {
        float4 highlightColor = litSample * float4(0.5f, 0.7f, 0.9f, 1.f);
        litTextureSample = lerp(highlightColor, litTextureSample, distMouse / brushRadius);
    }
    
    return litTextureSample;
}
