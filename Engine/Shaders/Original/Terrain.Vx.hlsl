#include "Terrain.hlsli"

struct AppData_pos  //  vertex constant buffer, slot 0 (VertexSlot::Position)
{
    float3 position : POSITION;
};

struct AppData_norm  // vertex constant buffer, slot 2 (VertexSlot::Norm)
{
    float3 norm : NORMAL; // norm location
};

struct AppData_tex  // vertex constant buffer, slot 3 (VertexSlot::TexCoord)
{
    float2 tex : TEXCOORD; // uv location
};

float4 GetRealTimeNormal(uint2 coordCenter)
{
    const float3 current = GetWorldPos3D(coordCenter);
    const float3 back = GetWorldPos3D(uint2(coordCenter.x - 1, coordCenter.y));
    const float3 forward = GetWorldPos3D(uint2(coordCenter.x + 1, coordCenter.y));
    const float3 up = GetWorldPos3D(uint2(coordCenter.x, coordCenter.y + 1));
    const float3 down = GetWorldPos3D(uint2(coordCenter.x, coordCenter.y - 1));
    const float3 upForward = GetWorldPos3D(uint2(coordCenter.x + 1, coordCenter.y + 1));
    const float3 downBack = GetWorldPos3D(uint2(coordCenter.x - 1, coordCenter.y - 1));

    const float3 a = normalize(cross((back - current), (current - up)));
    const float3 b = normalize(cross((downBack - current), (current - back)));
    const float3 c = normalize(cross((down - current), (current - downBack)));
    const float3 d = normalize(cross((forward - current), (current - down)));
    const float3 e = normalize(cross((upForward - current), (current - forward)));
    const float3 f = normalize(cross((up - current), (current - upForward)));

    const float3 add = (a + b + c + d + e + f);
    const float3 average = normalize(add * (1.0f / 6.0f));
    return float4(-average, 0.f);
}

VS_OUTPUT main(AppData_pos inPos, AppData_tex inTex, AppData_norm inNorm)
{
    VS_OUTPUT output;
    
    uint2 coord = GetCellAt2D(inPos.position);
    float heightValue = heightfield[coord] * heightScale;
    float3 truePos = float3(inPos.position.x, heightValue, inPos.position.z);
    
    output.norm = GetRealTimeNormal(coord);
    
    output.positionModelSpace = float4(truePos, 1.0f);
    
    output.position = mul(float4(truePos, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.textureCoordinate = inTex.tex;
    
    return output;
}