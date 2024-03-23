Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

struct VertexShaderOutput
{
    float2 tex : TEXCOORD;
    float4 position : SV_POSITION;
};

float4 FlatTexture_Main(VertexShaderOutput inData) : SV_TARGET
{
	return mainTexture.Sample(aSampler, inData.tex);
}
