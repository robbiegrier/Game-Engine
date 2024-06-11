Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

Texture2D<float> heightfield : register(t1);

cbuffer TerrainParameters : register(b8)
{
    float gridCellSize;
    float halfWorldScale;
    float heightScale;
    float pad81;
};

uint2 GetCellAt2D(float3 worldPos)
{
    float3 terrainZeroZeroLocation = float3(-halfWorldScale, 0.f, -halfWorldScale);
    float3 zeroZeroToLocation = worldPos - terrainZeroZeroLocation;

    const float scaledX = zeroZeroToLocation.x / gridCellSize;
    const float scaledZ = zeroZeroToLocation.z / gridCellSize;

    return uint2(scaledX, scaledZ);
}

float3 GetWorldPos3D(uint2 coord)
{
    float3 output = float3(-halfWorldScale, 0.f, -halfWorldScale);
    output += float3(coord.x, 0.f, coord.y) * gridCellSize;
    output.y = heightfield[coord] * heightScale;
    return output;
}

bool GetBarycentricData(out float beta, out float gamma, out float3 outA, out float3 outB, out float3 outC, float3 P)
{
    uint2 coord;
    float3 B, C;
    bool output;
     
    coord = GetCellAt2D(P);

		// bottom of diagonal
    float3 A = GetWorldPos3D(coord);

		// point minus A
    float3 v2 = P - A;

		// get correct triangle from cell
    if (-v2.x > v2.z)
    {
			// lower CCW
        B = GetWorldPos3D(uint2(coord.x, coord.y + 1)); // forward
        C = GetWorldPos3D(uint2(coord.x + 1, coord.y + 1)); // up/forward

        output = true; // returns true for lower
    }
    else
    {
			// upper CCW
        B = GetWorldPos3D(uint2(coord.x + 1, coord.y + 1)); // up/forward
        C = GetWorldPos3D(uint2(coord.x + 1, coord.y)); // up

        output = false; // returns false for upper
    }

    float3 v0 = B - A;
    float3 v1 = C - B;

    v0.y = 0.f;
    v1.y = 0.f;
    v2.y = 0.f;

    const float a = dot(v0, v0);
    const float b = dot(v1, v0);
    const float c = dot(v1, v1);
    const float d = dot(v2, v0);
    const float e = dot(v2, v1);

    beta = (d * c - b * e) / (a * c - b * b);
    gamma = (a * e - d * b) / (a * c - b * b);
    outA = A;
    outB = B;
    outC = C;

    return output;
}

float ComputeBarycentricHeight(float3 P)
{
    float BETA, GAMMA, output;
    float3 A, B, C;
    uint2 coord;
    
    coord = GetCellAt2D(P);

    GetBarycentricData(BETA, GAMMA, A, B, C, P);
    output = A.y + BETA * (B.y - A.y) + GAMMA * (C.y - B.y);

    return output;
}

struct VertexShaderOutput
{
    float2 tex : TEXCOORD;
    float4 position : SV_POSITION;
    float4 positionModelSpace : POSITION;
};

float4 main(VertexShaderOutput inData) : SV_TARGET
{
    float4 outputColor = mainTexture.Sample(aSampler, inData.tex);
    
    float heightWorldSpace = inData.positionModelSpace.y;
    
    float heightTerrain = ComputeBarycentricHeight(inData.positionModelSpace.xyz);
    //GetWorldPos3D(GetCellAt2D(inData.positionModelSpace.xyz)).y;
    
    float dist = abs(heightWorldSpace - heightTerrain);
    outputColor.rgb = float3(1.f, 1.f, 1.f);
    outputColor.w = lerp(1.f, 0.0f, dist * 1.5f);
    //outputColor.w = 1.f - inData.positionModelSpace.y;
    return outputColor;
}
