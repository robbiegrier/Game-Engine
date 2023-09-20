//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

// -------------------------------------
//  Pixel Shader Input Sematics
//        COLOR - vertex color
// -------------------------------------
struct PixelShaderInput
{
	float4 color : COLOR;
};

// --------------------------------------------------------
//  Pixel Shader Main()
//      SV_TARGET - System Value Target
//                  output stored in a render target 0  (only one target)
// --------------------------------------------------------
float4 ColorByVertex_Main(PixelShaderInput inData ) : SV_TARGET
{
	return inData.color;
}

// --- End of File ---
