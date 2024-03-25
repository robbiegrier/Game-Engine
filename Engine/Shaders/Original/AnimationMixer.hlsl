#pragma pack_matrix( row_major )

#define BONE_COUNT_MAX 128

float3 Lerp(float3 a, float3 b, float t)
{
    return a + ((b - a) * t);
}

float4 Slerp(float4 a, float4 b, float t)
{
    float4 output = float4(0.f, 0.f, 0.f, 0.f);
    float tmpCos = dot(a, b);
    
    if (abs(tmpCos) >= 1.f)
    {
        output = a;
    }
    else
    {
        float4 tmpb = b;

        if (tmpCos < 0.f)
        {
            tmpCos = -tmpCos;
            tmpb = -tmpb;
        }

        const float theta = acos(tmpCos);
        const float sinTheta = sin(theta);
    
        if (abs(sinTheta) >= 0.000001f)
        {
            float4 aPrime = a * (sin((1.f - t) * theta) / sinTheta);
            float4 bPrime = tmpb * (sin(t * theta) / sinTheta);
            output = aPrime + bPrime;
        }
    }
    
    return output;
}

struct BoneTransform
{
    float4 translation;
    float4 rotation;
    float4 scale;
};

// constants
cbuffer AA0 : register(b0)
{
    float4 time;
}

// inputs
StructuredBuffer<BoneTransform> frameBucketA : register(t0);
StructuredBuffer<BoneTransform> frameBucketB : register(t1);

// outputs
RWStructuredBuffer<BoneTransform> boneResultArray : register(u0);

[numthreads(BONE_COUNT_MAX, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    boneResultArray[DTid.x].translation.xyz = Lerp(frameBucketA[DTid.x].translation.xyz, frameBucketB[DTid.x].translation.xyz, time.x);
    boneResultArray[DTid.x].rotation = Slerp(frameBucketA[DTid.x].rotation, frameBucketB[DTid.x].rotation, time.x);
    boneResultArray[DTid.x].scale.xyz = Lerp(frameBucketA[DTid.x].scale.xyz, frameBucketB[DTid.x].scale.xyz, time.x);
}