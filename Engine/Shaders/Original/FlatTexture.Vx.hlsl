#pragma pack_matrix( row_major )

cbuffer AA0: register(b0)  // slot 0  (ConstantBufferSlot::Projection)
{
	matrix projectionMatrix;
}

cbuffer AA1 : register(b1) // slot 1 (ConstantBufferSlot::View)
{
	matrix viewMatrix;
}

cbuffer AA2 : register(b2) // slot 2 (ConstantBufferSlot::World)
{
	matrix worldMatrix;
}

struct AppData_pos  //  vertex constant buffer, slot 0 (VertexSlot::Position)
{
	float3 position : POSITION;
};

struct AppData_tex  // vertex constant buffer, slot 3 (VertexSlot::TexCoord)
{
	float2 tex: TEXCOORD;  // uv location
};

struct VertexShaderOutput
{
	float2 tex : TEXCOORD;
	float4 position : SV_POSITION;
};

VertexShaderOutput FlatTexture_Main(AppData_pos inPos, AppData_tex inTex)
{
	VertexShaderOutput outValue;

	matrix Mat = mul(mul(worldMatrix, viewMatrix), projectionMatrix);

	outValue.position = mul(float4(inPos.position, 1.0f), Mat);
	outValue.tex = inTex.tex;

	return outValue;
}
