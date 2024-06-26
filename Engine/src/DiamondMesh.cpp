#include "DiamondMesh.h"
#include "MathEngine.h"

namespace Azul
{
	constexpr float diamondHeight = 3.f;
	constexpr float diamondWidth = 2.f;
	constexpr float diamondCenter = 0.f;
	constexpr float dmdHalfH = diamondHeight / 2.f;
	constexpr float dmdHalfW = diamondWidth / 2.f;

	VertexPos DiamondMesh::g_Vertices_pos[] =
	{
		Vec3f(-dmdHalfW, diamondCenter, dmdHalfW),
		Vec3f(-dmdHalfW, diamondCenter, -dmdHalfW),
		Vec3f(dmdHalfW, diamondCenter, dmdHalfW),
		Vec3f(dmdHalfW, diamondCenter, -dmdHalfW),
		Vec3f(diamondCenter, +dmdHalfH, diamondCenter),
		Vec3f(diamondCenter, -dmdHalfH, diamondCenter),
	};

	VertexColor DiamondMesh::g_Vertices_color[] =
	{
		 Vec3(1.0f, 0.0f, 0.0f), // 0
		 Vec3(1.0f, 1.0f, 0.0f), // 1
		 Vec3(0.0f, 1.0f, 1.0f), // 2
		 Vec3(0.0f, 0.0f, 1.0f), // 3
		 Vec3(1.0f, 0.0f, 1.0f), // 4
		 Vec3(0.0f, 1.0f, 0.0f)  // 5
	};

	/*

			[0]-----[2]
			 | \   / |
			 | [4/5] |
			 | /   \ |
			[1]-----[3]

	*/

	uint32_t DiamondMesh::g_Indicies[] =
	{
		1, 4, 3,
		3, 4, 2,
		2, 4, 0,
		0, 4, 1,
		1, 3, 5,
		3, 2, 5,
		2, 0, 5,
		0, 1, 5
	};

	VertexTexCoord g_DiamondVertices_texCoord[] =
	{
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),
	};

	VertexNorm g_DiamondVertices_norm[] =
	{
		Vec3f(0.0f,  0.0f,  1.0f),
		Vec3f(-0.4f, -0.4f, -0.8f),
		Vec3f(0.4f, -0.4f, -0.8f),
		Vec3f(0.4f,  0.4f, -0.8f),
		Vec3f(-0.4f,  0.4f, -0.8f)
	};

	DiamondMesh::DiamondMesh()
		: Mesh(NUM_VERTS, NUM_INDICES)
	{
		pVertexBufferPosition = CreateVertexBuffer(sizeof(g_Vertices_pos), g_Vertices_pos);
		pVertexBufferColor = CreateVertexBuffer(sizeof(g_Vertices_color), g_Vertices_color);
		pVertexBufferTextureCoordinates = CreateVertexBuffer(sizeof(g_DiamondVertices_texCoord), g_DiamondVertices_texCoord);
		pVertexBufferNormal = CreateVertexBuffer(sizeof(g_DiamondVertices_norm), g_DiamondVertices_norm);
		pIndexBufferTriangles = CreateIndexBuffer(sizeof(g_Indicies), g_Indicies);
		pConstantBufferLightColor = CreateConstantBuffer(sizeof(Vec3));
		pConstantBufferLightPosition = CreateConstantBuffer(sizeof(Vec3));

		HackSetBoundingSphereData(g_Vertices_pos);
		HackSetBoundingBoxData(g_Vertices_pos);
	}

	DiamondMesh::~DiamondMesh()
	{
	}
}