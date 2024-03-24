#pragma pack_matrix( row_major )

cbuffer AA0 : register(b0)
{
    matrix projectionMatrix;
}

cbuffer AA1 : register(b1)
{
    matrix viewMatrix;
}

cbuffer AA2 : register(b2)
{
    matrix worldMatrix;
}

cbuffer AA3 : register(b3)
{
    matrix uvMatrix;
}

struct AppData_pos
{
    float3 position : POSITION;
};

struct AppData_tex
{
    float2 tex : TEXCOORD;
};

struct VertexShaderOutput
{
    float2 tex : TEXCOORD;
    float4 position : SV_POSITION;
    matrix uvMat : MONKEY;
};

VertexShaderOutput main(AppData_pos inPos, AppData_tex inTex)
{
    VertexShaderOutput outValue;

    matrix Mat = mul(mul(worldMatrix, viewMatrix), projectionMatrix);

    outValue.position = mul(float4(inPos.position, 1.0f), Mat);
    outValue.tex = inTex.tex;
    outValue.uvMat = uvMatrix;

    return outValue;
}
