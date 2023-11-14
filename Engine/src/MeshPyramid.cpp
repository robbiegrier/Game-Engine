//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MeshPyramid.h"
#include "MathEngine.h"
#include "Engine.h"
#include "DirectXDeviceMan.h"


using namespace Azul;

VertexPos g_PyramidVertices_pos[] =
{
	Vec3f(0,  0, 2),  //0
	Vec3f(-1, -1, 0),  //1
	Vec3f(1, -1, 0),  //2
	Vec3f(1,  1, 0),  //3
	Vec3f(-1,  1, 0)   //4
};

VertexColor g_PyramidVertices_color[] =
{
	 Vec3(0.0f, 0.0f, 1.0f) , // 0
	 Vec3(0.0f, 1.0f, 0.0f) , // 1
	 Vec3(1.0f, 1.0f, 0.0f) , // 2
	 Vec3(1.0f, 0.0f, 0.0f) , // 3
	 Vec3(1.0f, 0.0f, 1.0f)   // 4
};


uint32_t g_PyramidIndices[] =
{
	1, 0, 4,
	2, 0, 1,
	3, 0, 2,
	4, 0, 3,
	4, 2, 1,
	4, 3, 2
};

VertexTexCoord g_PyramidVertices_texCoord[] =
{
	Vec2f(0.5f, 0.5f),
	Vec2f(0.0f, 1.0f),
	Vec2f(1.0f, 1.0f),
	Vec2f(1.0f, 0.0f),
	Vec2f(0.0f, 0.0f)
};

VertexNorm g_PyramidVertices_norm[] =
{
	Vec3f(0.0f,  0.0f,  1.0f),
	Vec3f(-0.4f, -0.4f, -0.8f),
	Vec3f(0.4f, -0.4f, -0.8f),
	Vec3f(0.4f,  0.4f, -0.8f),
	Vec3f(-0.4f,  0.4f, -0.8f)

};

unsigned int xxx = sizeof(g_PyramidVertices_pos);
unsigned int NumPyramidVerts = sizeof(g_PyramidVertices_pos) / sizeof(g_PyramidVertices_pos[0]);
unsigned int NumPyramidIndices = sizeof(g_PyramidIndices) / sizeof(g_PyramidIndices[0]);


MeshPyramid::MeshPyramid()
	: Mesh(NumPyramidVerts, NumPyramidIndices)
{
	// Create an initialize the vertex buffer.
	poVertexBuffer_pos = CreateVertexBuffer(sizeof(g_PyramidVertices_pos), g_PyramidVertices_pos);
	poVertexBuffer_color = CreateVertexBuffer(sizeof(g_PyramidVertices_color), g_PyramidVertices_color);
	poVertexBuffer_texCoord = CreateVertexBuffer(sizeof(g_PyramidVertices_texCoord), g_PyramidVertices_texCoord);
	poVertexBuffer_norm = CreateVertexBuffer(sizeof(g_PyramidVertices_norm), g_PyramidVertices_norm);

	// Create and initialize the index buffer.
	poIndexBuffer = CreateIndexBuffer(sizeof(g_PyramidIndices), g_PyramidIndices);

	// Create the constant buffers for the variables defined in the vertex shader.
	poConstantBuff_Projection = CreateConstantBuffer(sizeof(Mat4));
	poConstantBuff_View = CreateConstantBuffer(sizeof(Mat4));
	poConstantBuff_World = CreateConstantBuffer(sizeof(Mat4));

	// Additional material owned by meshPyramid
	poConstantBuff_lightColor = CreateConstantBuffer(sizeof(Vec3));
	poConstantBuff_lightPos = CreateConstantBuffer(sizeof(Vec3));
}


// --- End of File ---
