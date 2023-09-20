//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// ------------------------------------------
// tx - t is texture buuffer, x - slot
// ------------------------------------------
Texture2D mainTexture : register(t0);

// ------------------------------------------
// sx - s is sampler, x - slot
// ------------------------------------------
SamplerState aSampler : register(s0);

// -------------------------------------
//  Pixel Shader Input Sematics
//        TEXCOORD - uv tex coord
// -------------------------------------
struct PixelShaderInput
{
	float2 tex : TEXCOORD;
};

// --------------------------------------------------------
//  Pixel Shader Main()
//      SV_TARGET - System Value Target
//                  output stored in a render target 0  (only one target)
// --------------------------------------------------------
float4 FlatTexture_Main(PixelShaderInput inData) : SV_TARGET
{
	return mainTexture.Sample(aSampler, inData.tex);
}

// --- End of File ---