#include "ComputeShaderBrush.hlsli"

#define SPLATTING_RESOLUTION 4u

RWTexture2D<float4> surfaceLayerWeights : register(u0);