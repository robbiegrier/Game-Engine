Texture2D mainTexture : register(t0);
SamplerState aSampler : register(s0);

struct PixelShaderInput
{
    float2 tex : TEXCOORD;
    float4 position : SV_POSITION;
    matrix uvMat : MONKEY;
};

float4 main(PixelShaderInput inData) : SV_TARGET
{
	// correct the UV coordinates, originally its (0,0 - 1,1)
	// scale and translate to the correct sub image on the texture

    float4 uv_vec;
    uv_vec.xy = inData.tex.xy;
    uv_vec.z = 0.0f;
    uv_vec.w = 1.0f;

    float4 uv_corrected = mul(inData.uvMat, uv_vec);

    return mainTexture.Sample(aSampler, uv_corrected.xy);
    
	return mainTexture.Sample(aSampler, inData.tex);
    
}
