#include "PyramidMesh.h"
#include "MathEngine.h"

namespace Azul
{
	constexpr float pyramidHeight = 2.f;
	constexpr float pyramidWidth = 2.f;
	constexpr float pyramidCenter = 0.f;
	constexpr float pyrHalfH = pyramidHeight / 2.f;
	constexpr float pyrHalfW = pyramidWidth / 2.f;

	VertexPos PyramidMesh::g_Vertices_pos[] =
	{
		Vec3f(-pyrHalfW, -pyrHalfH, pyrHalfW),
		Vec3f(-pyrHalfW, -pyrHalfH, -pyrHalfW),
		Vec3f(pyrHalfW, -pyrHalfH, pyrHalfW),
		Vec3f(pyrHalfW, -pyrHalfH, -pyrHalfW),
		Vec3f(pyramidCenter, +pyrHalfH, pyramidCenter),
	};

	VertexColor PyramidMesh::g_Vertices_color[] =
	{
		 Vec3(0.0f, 0.2f, 1.0f) , // 0
		 Vec3(0.0f, 0.4f, 1.0f) , // 1
		 Vec3(0.0f, 0.7f, 1.0f) , // 2
		 Vec3(0.0f, 0.8f, 1.0f) , // 3
		 Vec3(1.0f, 0.5f, 0.0f) , // 4
	};

	/*

			[2]-----[0]
			 | \   / |
			 |	[4]	 |
			 | /   \ |
			[3]-----[1]

	*/

	uint32_t PyramidMesh::g_Indicies[] =
	{
		0, 1, 3,
		2, 0, 3,
		1, 4, 3,
		3, 4, 2,
		2, 4, 0,
		0, 4, 1
	};

	VertexTexCoord g_PyramidVertices_texCoord[] =
	{
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f)
	};

	VertexNorm g_PyramidVertices_norm[] =
	{
		Vec3f(0.0f,  0.0f,  1.0f),
		Vec3f(-0.4f, -0.4f, -0.8f),
		Vec3f(0.4f, -0.4f, -0.8f),
		Vec3f(0.4f,  0.4f, -0.8f),
		Vec3f(-0.4f,  0.4f, -0.8f)
	};

	PyramidMesh::PyramidMesh()
		: Mesh(NUM_VERTS, NUM_INDICES)
	{
		poVertexBuffer_pos = CreateVertexBuffer(sizeof(g_Vertices_pos), g_Vertices_pos);
		poVertexBuffer_color = CreateVertexBuffer(sizeof(g_Vertices_color), g_Vertices_color);
		poVertexBuffer_texCoord = CreateVertexBuffer(sizeof(g_PyramidVertices_texCoord), g_PyramidVertices_texCoord);
		poVertexBuffer_norm = CreateVertexBuffer(sizeof(g_PyramidVertices_norm), g_PyramidVertices_norm);
		poIndexBuffer = CreateIndexBuffer(sizeof(g_Indicies), g_Indicies);
		poConstantBuff_lightColor = CreateConstantBuffer(sizeof(Vec3));
		poConstantBuff_lightPos = CreateConstantBuffer(sizeof(Vec3));

		HackSetBoundingSphereData(g_Vertices_pos);
		HackSetBoundingBoxData(g_Vertices_pos);
	}

	PyramidMesh::~PyramidMesh()
	{
	}
}