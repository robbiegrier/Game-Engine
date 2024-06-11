#pragma pack_matrix( row_major )

#define TERRAIN_BRUSH_THREADS 32

// constants
cbuffer AA0 : register(b0)
{
    uint2 offset;
    float force;
    float radius;
    float4 center;
    float gridCellSize;
    float halfWorldScale;
    float pad1;
    float pad2;
}

float2 GetWorldPos2D(uint2 coord)
{
    float2 output = float2(-halfWorldScale, -halfWorldScale);
    output += float2(coord) * gridCellSize;
    return output;
}

uint2 GetCoordinates(uint3 DTid)
{
    return uint2(offset.x + DTid.x, offset.y + DTid.y);
}

float GetDistanceToBrushCenter(uint2 coord)
{
    float2 worldPos2D = GetWorldPos2D(coord);
    return distance(worldPos2D, center.xz);
}
