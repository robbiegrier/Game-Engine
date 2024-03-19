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

VS_OUTPUT main(AppData_pos inPos, AppData_tex inTex, AppData_norm inNorm)
{
    VS_OUTPUT output;
    
    output.positionModelSpace = float4(inPos.position, 1.0f); // We pass along the raw model space position 
    output.norm = float4(inNorm.norm, 0.0f); // and the face normal

    output.position = mul(float4(inPos.position, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.textureCoordinate = inTex.tex;

    return output;
}
