#include "CrossMesh.h"
#include "MathEngine.h"

namespace Azul
{
	VertexPos CrossMesh::g_Vertices_pos[] =
	{
		Vec3f(-1.0f, -1.0f, -1.0f), // 0
		Vec3f(-1.0f,  1.0f, -1.0f), // 1
		Vec3f(1.0f,  1.0f, -1.0f) , // 2
		Vec3f(1.0f, -1.0f, -1.0f) , // 3
		Vec3f(-1.0f, -1.0f,  1.0f), // 4
		Vec3f(-1.0f,  1.0f,  1.0f), // 5
		Vec3f(1.0f,  1.0f,  1.0f) , // 6
		Vec3f(1.0f, -1.0f,  1.0f),  // 7

		// top
		Vec3f(-1.0f,  3.0f, -1.0f), // 8
		Vec3f(1.0f,  3.0f, -1.0f),  // 9
		Vec3f(-1.0f,  3.0f,  1.0f), // 10
		Vec3f(1.0f,  3.0f,  1.0f),  // 11

		// bottom
		Vec3f(-1.0f,  -3.0f, -1.0f),// 12
		Vec3f(1.0f,  -3.0f, -1.0f), // 13
		Vec3f(-1.0f, -3.0f,  1.0f), // 14
		Vec3f(1.0f,  -3.0f,  1.0f), // 15

		// left
		Vec3f(-3.0f, -1.0f, -1.0f), // 16
		Vec3f(-3.0f,  1.0f, -1.0f), // 17
		Vec3f(-3.0f, -1.0f,  1.0f), // 18
		Vec3f(-3.0f,  1.0f,  1.0f), // 19

		// right
		Vec3f(3.0f, -1.0f, -1.0f),  // 20
		Vec3f(3.0f,  1.0f, -1.0f),  // 21
		Vec3f(3.0f, -1.0f,  1.0f),  // 22
		Vec3f(3.0f,  1.0f,  1.0f),  // 23
	};

	VertexColor CrossMesh::g_Vertices_color[] =
	{
		 Vec3(1.0f, 0.0f, 0.0f) , // 0
		 Vec3(1.0f, 0.0f, 0.0f) , // 1
		 Vec3(1.0f, 0.0f, 0.0f) , // 2
		 Vec3(1.0f, 0.0f, 0.0f) , // 3
		 Vec3(1.0f, 0.0f, 0.0f) , // 4
		 Vec3(1.0f, 0.0f, 0.0f) , // 5
		 Vec3(1.0f, 0.0f, 0.0f) , // 6
		 Vec3(1.0f, 0.0f, 0.0f),  // 7
		 Vec3(0.0f, 0.0f, 1.0f) ,// 8
		 Vec3(0.0f, 0.0f, 1.0f) ,// 9
		 Vec3(0.0f, 0.0f, 1.0f) ,// 10
		 Vec3(0.0f, 0.0f, 1.0f) ,// 11
		 Vec3(0.0f, 0.0f, 1.0f) ,// 12
		 Vec3(0.0f, 0.0f, 1.0f) ,// 13
		 Vec3(0.0f, 0.0f, 1.0f) ,// 14
		 Vec3(0.0f, 0.0f, 1.0f),// 15
		 Vec3(0.0f, 1.0f, 0.0f) ,// 16
		 Vec3(0.0f, 1.0f, 0.0f) ,// 17
		 Vec3(0.0f, 1.0f, 0.0f) ,// 18
		 Vec3(0.0f, 1.0f, 0.0f) ,// 19
		 Vec3(0.0f, 1.0f, 0.0f) ,// 20
		 Vec3(0.0f, 1.0f, 0.0f) ,// 21
		 Vec3(0.0f, 1.0f, 0.0f) ,// 22
		 Vec3(0.0f, 1.0f, 0.0f),// 23
	};

	uint32_t CrossMesh::g_Indicies[] =
	{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		4, 7, 6,

		10, 11, 8,
		8, 11, 9,
		1, 9, 2,
		1, 8, 9,

		2, 9, 6,
		6, 9, 11,
		6, 11, 5,
		11, 10, 5,

		10, 8, 5,
		8, 1, 5,
		14, 12, 13,
		14, 13, 15,

		0, 3, 12,
		12, 3, 13,
		13, 3, 7,
		13, 7, 15,

		4, 0, 12,
		4, 12, 14,
		15, 7, 4,
		15, 4, 14,

		19, 16,18,
		19, 17, 16,
		16, 1, 0,
		16, 17, 1,

		17, 5, 1,
		17, 19, 5,
		4, 5, 18,
		5, 19, 18,

		18, 16, 0,
		18, 0, 4,
		3, 21,20,
		3, 2, 21,

		2, 6, 23,
		2, 23, 21,
		23, 7, 22,
		23, 6, 7,

		3, 20, 7,
		20, 22, 7,
		20, 21, 23,
		20, 23, 22
	};

	VertexTexCoord g_CrossVertices_texCoord[] =
	{
		Vec2f(0.666f, 0.333f),
		Vec2f(0.666f, 0.666f),
		Vec2f(0.333f, 0.666f),
		Vec2f(0.333f, 0.333f),

		Vec2f(0.666f, 0.333f),
		Vec2f(0.666f, 0.666f),
		Vec2f(0.333f, 0.666f),
		Vec2f(0.333f, 0.333f),

		Vec2f(0.666f, 1.0f),
		Vec2f(0.333f, 1.0f),

		Vec2f(0.666f, 1.0f),
		Vec2f(0.333f, 1.0f),

		Vec2f(0.666f, 0.0f),
		Vec2f(0.333f, 0.0f),

		Vec2f(0.666f, 0.0f),
		Vec2f(0.333f, 0.0f),

		Vec2f(1.0f, 0.333f),
		Vec2f(1.0f, 0.666f),

		Vec2f(1.0f, 0.333f),
		Vec2f(1.0f, 0.666f),

		Vec2f(0.0f, 0.333f),
		Vec2f(0.0f, 0.666f),

		Vec2f(0.0f, 0.333f),
		Vec2f(0.0f, 0.666f),
	};

	VertexNorm g_CrossVertices_norm[] =
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

	CrossMesh::CrossMesh()
		: Mesh(NUM_VERTS, NUM_INDICES)
	{
		pVertexBufferPosition = CreateVertexBuffer(sizeof(g_Vertices_pos), g_Vertices_pos);
		pVertexBufferColor = CreateVertexBuffer(sizeof(g_Vertices_color), g_Vertices_color);
		pVertexBufferTextureCoordinates = CreateVertexBuffer(sizeof(g_CrossVertices_texCoord), g_CrossVertices_texCoord);
		pVertexBufferNormal = CreateVertexBuffer(sizeof(g_CrossVertices_norm), g_CrossVertices_norm);
		pIndexBufferTriangles = CreateIndexBuffer(sizeof(g_Indicies), g_Indicies);
		pConstantBufferLightColor = CreateConstantBuffer(sizeof(Vec3));
		pConstantBufferLightPosition = CreateConstantBuffer(sizeof(Vec3));

		HackSetBoundingSphereData(g_Vertices_pos);
		HackSetBoundingBoxData(g_Vertices_pos);
	}

	CrossMesh::~CrossMesh()
	{
	}
}