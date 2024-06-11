#include "Default.hlsli"

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

struct InstData_world
{
    float x : INSTANCEX;
    float z : INSTANCEZ;
    float scale : INSTANCESCALE;
    float angle : INSTANCEANGLE;
    float4 instanceColor : INSTANCECOLOR;
};

struct VS_OUTPUT_FOLIAGE
{
    float4 position : SV_POSITION;
    float4 positionModelSpace : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float4 norm : NORMAL;
    float4 tint : TINT;
    matrix runtimeInverse : RTINVERSE;
};

Texture2D<float> heightfield : register(t1);

cbuffer TerrainParameters : register(b8)
{
    float gridCellSize;
    float halfWorldScale;
    float heightScale;
    float pad81;
};

cbuffer FoliageParameters : register(b9)
{
    float foliageRenderDistance;
    float foliageScaleFalloffStart;
    float followNormal;
    float pad91;
};

float3 GetWorldPos3D(uint2 coord)
{
    float3 output = float3(-halfWorldScale, 0.f, -halfWorldScale);
    output += float3(coord.x, 0.f, coord.y) * gridCellSize;
    output.y = heightfield[coord] * heightScale;
    return output;
}

uint2 GetCellAt2D(float3 worldPos)
{
    float3 terrainZeroZeroLocation = float3(-halfWorldScale, 0.f, -halfWorldScale);
    float3 zeroZeroToLocation = worldPos - terrainZeroZeroLocation;

    const float scaledX = zeroZeroToLocation.x / gridCellSize;
    const float scaledZ = zeroZeroToLocation.z / gridCellSize;

    return uint2(scaledX, scaledZ);
}

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

row_major matrix MakeRotationMatrixY(float angle)
{
    row_major matrix output;
    output[0] = float4(cos(angle), 0.f, -sin(angle), 0.f);
    output[1] = float4(0.f, 1.f, 0.f, 0.f);
    output[2] = float4(sin(angle), 0.f, cos(angle), 0.f);
    output[3] = float4(0.f, 0.f, 0.f, 1.f);
    return output;
}


VS_OUTPUT_FOLIAGE main(AppData_pos inPos, AppData_tex inTex, AppData_norm inNorm, InstData_world instWorld)
{
    VS_OUTPUT_FOLIAGE output;
    
    row_major matrix makeWorld = Identity;
    makeWorld._41_42_43 = float3(instWorld.x, 0.f, instWorld.z);
    float3 instWorldPos = makeWorld[3].xyz;
    uint2 coord = GetCellAt2D(instWorldPos);
    float terrainHeight = heightfield[coord] * heightScale;
    makeWorld._42 = terrainHeight;

    if (followNormal > 0.f)
    {
        const float3 up = GetRealTimeNormal(coord).xyz;
        const float3 right = cross(float3(1.f, 0.f, 0.f), up);
        const float3 forward = cross(up, right);
        makeWorld[0].xyz = forward;
        makeWorld[1].xyz = up;
        makeWorld[2].xyz = right;
    }
    
    matrix rotY = MakeRotationMatrixY(instWorld.angle);
    makeWorld = mul(rotY, makeWorld);
    
    float scale = instWorld.scale;
    
    float4 msEyePos = mul(eyePositionWorld, inverse);
    float distToEye = length(msEyePos - float4(makeWorld[3].xyz, 1.0f));
    
    if (distToEye > foliageScaleFalloffStart)
    {
        float falloffRange = foliageRenderDistance - foliageScaleFalloffStart;
        falloffRange *= 0.25f;
        scale = lerp(scale, 0.f, (distToEye - foliageScaleFalloffStart) / falloffRange);
    }
    
    makeWorld[0] *= scale;
    makeWorld[1] *= scale;
    makeWorld[2] *= scale;
    
    output.positionModelSpace = float4(inPos.position, 1.0f);
    
    output.norm = float4(inNorm.norm, 0.0f);

    output.position = mul(float4(inPos.position, 1.0f), makeWorld);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.textureCoordinate = inTex.tex;
    
    output.tint = instWorld.instanceColor;
    
    output.runtimeInverse = transpose(rotY);
    output.runtimeInverse._41 = (-makeWorld._41 * rotY._11) + (-makeWorld._42 * rotY._21) + (-makeWorld._43 * rotY._31);
    output.runtimeInverse._42 = (-makeWorld._41 * rotY._12) + (-makeWorld._42 * rotY._22) + (-makeWorld._43 * rotY._32);
    output.runtimeInverse._43 = (-makeWorld._41 * rotY._13) + (-makeWorld._42 * rotY._23) + (-makeWorld._43 * rotY._33);
    
    return output;
}
