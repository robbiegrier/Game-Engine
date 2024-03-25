#include "SkinLightTexture.hlsli"

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
	float2 tex: TEXCOORD;  // uv location
};

struct AppData_weights  // vertex constant buffer, slot 4 (VertexSlot::Weights)
{
    float4 w : BLENDWEIGHT; // weight location
};

struct AppData_joints  // vertex constant buffer, slot 5 (VertexSlot::Joints)
{
    uint4 j : BLENDINDICES; // joints location
};

VS_OUTPUT main(AppData_pos inPos, AppData_norm inNorm, AppData_tex inTex, AppData_weights inWeight, AppData_joints inJoint)
{
    VS_OUTPUT outValue;
	
    matrix skinWorld = mul(BoneWorld[inJoint.j.x], inWeight.w.x);
    skinWorld += mul(BoneWorld[inJoint.j.y], inWeight.w.y);
    skinWorld += mul(BoneWorld[inJoint.j.z], inWeight.w.z);
    skinWorld += mul(BoneWorld[inJoint.j.w], inWeight.w.w);

    matrix transform = mul(mul(skinWorld, viewMatrix), projectionMatrix);
    outValue.position = mul(float4(inPos.position, 1.0f), transform);
    outValue.textureCoordinate = inTex.tex;
    
    outValue.norm = float4(inNorm.norm, 0.0f);
    outValue.positionModelSpace = float4(inPos.position, 1.0f);

	return outValue;
}
