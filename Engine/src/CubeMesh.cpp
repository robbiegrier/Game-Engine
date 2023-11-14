#include "CubeMesh.h"
#include "MathEngine.h"

namespace Azul
{
	VertexPos CubeMesh::g_Vertices_pos[] =
	{
		// Triangle 0
		Vec3f(-0.25f,  0.25f, -0.25f),
		Vec3f(-0.25f, -0.25f, -0.25f),
		Vec3f(0.25f, -0.25f,  -0.25f),

		// Triangle 1
		Vec3f(0.25f,  -0.25f, -0.25f),
		Vec3f(0.25f,   0.25f, -0.25f),
		Vec3f(-0.25f,  0.25f, -0.25f),

		// Triangle 2
		Vec3f(0.25f, -0.25f, -0.25f),
		Vec3f(0.25f, -0.25f,  0.25f),
		Vec3f(0.25f,  0.25f, -0.25f),

		// Triangle 3
		Vec3f(0.25f, -0.25f,  0.25f),
		Vec3f(0.25f,  0.25f,  0.25f),
		Vec3f(0.25f,  0.25f, -0.25f),

		// Triangle 4
		Vec3f(0.25f,  -0.25f,  0.25f),
		Vec3f(-0.25f,  -0.25f,  0.25f),
		Vec3f(0.25f,   0.25f,  0.25f),

		// Triangle 5
		Vec3f(-0.25f, -0.25f,  0.25f),
		Vec3f(-0.25f,  0.25f,  0.25f),
		Vec3f(0.25f,   0.25f,  0.25f),

		// Triangle 6
		Vec3f(-0.25f, -0.25f,  0.25f),
		Vec3f(-0.25f, -0.25f, -0.25f),
		Vec3f(-0.25f,  0.25f,  0.25f),

		// Triangle 7
		Vec3f(-0.25f, -0.25f, -0.25f),
		Vec3f(-0.25f,  0.25f, -0.25f),
		Vec3f(-0.25f,  0.25f,  0.25f),

		// Triangle 8
		Vec3f(-0.25f, -0.25f,  0.25f),
		Vec3f(0.25f, -0.25f,   0.25f),
		Vec3f(0.25f, -0.25f,  -0.25f),

		// Triangle 9
		Vec3f(0.25f, -0.25f,  -0.25f),
		Vec3f(-0.25f, -0.25f, -0.25f),
		Vec3f(-0.25f, -0.25f,  0.25f),

		// Triangle 10
		Vec3f(-0.25f,  0.25f, -0.25f),
		Vec3f(0.25f,  0.25f,  -0.25f),
		Vec3f(0.25f,  0.25f,   0.25f),

		// Triangle 11
		Vec3f(0.25f,  0.25f,   0.25f),
		Vec3f(-0.25f,  0.25f,  0.25f),
		Vec3f(-0.25f,  0.25f, -0.25f)
	};

	VertexColor CubeMesh::g_Vertices_color[] =
	{
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,
		 Vec3(0.0f, 0.3f, 0.0f) ,

		 Vec3(0.0f, 0.3f, 0.0f),
		 Vec3(0.0f, 0.3f, 0.0f),
		 Vec3(0.0f, 0.3f, 0.0f),
	};

	uint32_t CubeMesh::g_Indicies[] =
	{
		1, 0,2 ,
		4, 3,5 ,
		7, 6,8 ,
		10,9,11 ,
		13,12,14 ,
		16,15,17 ,
		19,18,20 ,
		22,21,23 ,
		25,24,26 ,
		28,27,29 ,
		31,30,32 ,
		34,33,35
	};

	VertexTexCoord g_CubeVertices_texCoord[] =
	{
		// Triangle 0
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),

		// Triangle 1
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),

		// Triangle 2
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 0.0f),

		// Triangle 3
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),

		// Triangle 4
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 5
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 6
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 7
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 8
		Vec2f(0.0f, 0.0f),
		Vec2f(1.0f, 0.0f),
		Vec2f(1.0f, 1.0f),

		// Triangle 9
		Vec2f(1.0f, 1.0f),
		Vec2f(0.0f, 1.0f),
		Vec2f(0.0f, 0.0f),

		// Triangle 10
		Vec2f(0.0f, 1.0f),
		Vec2f(1.0f, 1.0f),
		Vec2f(1.0f, 0.0f),

		// Triangle 11
		Vec2f(1.0f, 0.0f),
		Vec2f(0.0f, 0.0f),
		Vec2f(0.0f, 1.0f)
	};

	VertexNorm g_CubeVertices_norm[] =
	{
		// Triangle 0
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f, -0.6f, -0.6f),
		Vec3f(0.6f,  -0.6f, -0.6f),

		// Triangle 1
		Vec3f(0.6f, -0.6f, -0.6f),
		Vec3f(0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),

		// Triangle 2
		Vec3f(0.6f, -0.6f, -0.6f),
		Vec3f(0.6f, -0.6f,  0.6f),
		Vec3f(0.6f,  0.6f, -0.6f),

		// Triangle 3
		Vec3f(0.6f, -0.6f,  0.6f),
		Vec3f(0.6f,  0.6f,  0.6f),
		Vec3f(0.6f,  0.6f, -0.6f),

		// Triangle 4
		Vec3f(0.6f, -0.6f, 0.6f),
		Vec3f(-0.6f, -0.6f, 0.6f),
		Vec3f(0.6f,  0.6f, 0.6f),

		// Triangle 5
		Vec3f(-0.6f, -0.6f, 0.6f),
		Vec3f(-0.6f,  0.6f, 0.6f),
		Vec3f(0.6f,  0.6f, 0.6f),

		// Triangle 6
		Vec3f(-0.6f, -0.6f,  0.6f),
		Vec3f(-0.6f, -0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f,  0.6f),

		// Triangle 7
		Vec3f(-0.6f, -0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f, -0.6f),
		Vec3f(-0.6f,  0.6f,  0.6f),

		// Triangle 8
		Vec3f(-0.6f, -0.6f,  0.6f),
		Vec3f(0.6f, -0.6f,  0.6f),
		Vec3f(0.6f, -0.6f, -0.6f),

		// Triangle 9
		Vec3f(0.6f, -0.6f, -0.6f),
		Vec3f(-0.6f, -0.6f, -0.6f),
		Vec3f(-0.6f, -0.6f,  0.6f),

		// Triangle 10
		Vec3f(-0.6f, 0.6f, -0.6f),
		Vec3f(0.6f, 0.6f, -0.6f),
		Vec3f(0.6f, 0.6f,  0.6f),

		// Triangle 11
		Vec3f(0.6f, 0.6f,  0.6f),
		Vec3f(-0.6f, 0.6f,  0.6f),
		Vec3f(-0.6f, 0.6f, -0.6f)
	};

	CubeMesh::CubeMesh()
		: Mesh(NUM_VERTS, NUM_INDICES)
	{
		poVertexBuffer_pos = CreateVertexBuffer(sizeof(g_Vertices_pos), g_Vertices_pos);
		poVertexBuffer_color = CreateVertexBuffer(sizeof(g_Vertices_color), g_Vertices_color);
		poVertexBuffer_texCoord = CreateVertexBuffer(sizeof(g_CubeVertices_texCoord), g_CubeVertices_texCoord);
		poVertexBuffer_norm = CreateVertexBuffer(sizeof(g_CubeVertices_norm), g_CubeVertices_norm);
		poIndexBuffer = CreateIndexBuffer(sizeof(g_Indicies), g_Indicies);
		poConstantBuff_lightColor = CreateConstantBuffer(sizeof(Vec3));
		poConstantBuff_lightPos = CreateConstantBuffer(sizeof(Vec3));

		HackSetBoundingSphereData(g_Vertices_pos);
	}

	CubeMesh::~CubeMesh()
	{
		// remove anything dynamic here
	}
}