#pragma pack_matrix( row_major )

#define BONE_COUNT_MAX 128
#define HIERARCHY_MAX_DEPTH 20

struct RowMatrix
{
    matrix m;
};

static const matrix Identity = {
    { 1.f, 0.f, 0.f, 0.f },
    { 0.f, 1.f, 0.f, 0.f },
    { 0.f, 0.f, 1.f, 0.f },
    { 0.f, 0.f, 0.f, 1.f }
};

struct BoneTransform
{
    float4 translation;
    float4 rotation;
    float4 scale;
};

// constants
cbuffer AA0 : register(b0)
{
    matrix pivotWorld;
    int hierarchyDepth;
    int pad0;
    int pad1;
    int pad2;
}

cbuffer AA1 : register(b1)
{
    matrix invBind[BONE_COUNT_MAX];
}

// inputs
RWStructuredBuffer<BoneTransform> boneLocal : register(u0);
StructuredBuffer<int> table : register(t1);

// outputs
RWStructuredBuffer<RowMatrix> worldOut : register(u1);

matrix TransMatrix(float4 trans)
{
    matrix result = Identity;
    result._m30_m31_m32 = trans.xyz;
    return result;

}

matrix ScaleMatrix(float4 scale)
{
    matrix result = Identity;
    result._m00_m11_m22 = scale.xyz;
    return result;
}

matrix RotMatrix(float4 q)
{
    matrix result;

    float x2, y2, z2;

    x2 = 2 * q.x;
    y2 = 2 * q.y;
    z2 = 2 * q.z;

    float xx, xy, xz;
    float yy, yz, zz;
    float wx, wy, wz;

    xx = q.x * x2;
    xy = q.x * y2;
    xz = q.x * z2;

    yy = q.y * y2;
    yz = q.y * z2;
    zz = q.z * z2;

    wx = q.w * x2;
    wy = q.w * y2;
    wz = q.w * z2;

    result[0] = float4(1.0 - (yy + zz), xy + wz, xz - wy, 0);
    result[1] = float4(xy - wz, 1 - (xx + zz), yz + wx, 0);
    result[2] = float4(xz + wy, yz - wx, 1 - (xx + yy), 0);
    result[3] = float4(0, 0, 0, 1);

    return result;
}

matrix Bone2Matrix(BoneTransform bone)
{
    matrix mTrans = TransMatrix(bone.translation);
    matrix mScale = ScaleMatrix(bone.scale);
    matrix mRot = RotMatrix(bone.rotation);

    matrix result;

    result = mul(mul(mScale, mRot), mTrans);

    return result;
}

[numthreads(BONE_COUNT_MAX, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    matrix world = pivotWorld;
    
    for (int i = 0; i < hierarchyDepth; i++)
    {
        int parentIndex = table[(DTid.x * HIERARCHY_MAX_DEPTH) + i];
        
        if (parentIndex != -1)
        {
            world = mul(Bone2Matrix(boneLocal[parentIndex]), world);
        }
    }
    
    world = mul(invBind[DTid.x], world);
    worldOut[DTid.x].m = world;
}

