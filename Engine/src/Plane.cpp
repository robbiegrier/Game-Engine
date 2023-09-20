#include "Plane.h"
#include "MathEngine.h"

namespace Azul
{
	constexpr float planeScale = 100.f;

	VertexPos PlaneMesh::g_Vertices_pos[] =
	{
		Vec3(-planeScale,  0.f, -planeScale),
		Vec3(-planeScale, 0.f, planeScale),
		Vec3(planeScale, 0.f, -planeScale),
		Vec3(planeScale,  0.f, planeScale)
	};

	VertexColor PlaneMesh::g_Vertices_color[] =
	{
		 Vec3(1.0f, 1.0f, 1.0f),
		 Vec3(1.0f, 1.0f, 1.0f),
		 Vec3(1.0f, 1.0f, 1.0f),
		 Vec3(1.0f, 1.0f, 1.0f)
	};

	uint32_t PlaneMesh::g_Indicies[] =
	{
		1, 2, 0,
		1, 3, 2
	};

	VertexTexCoord g_PlaneVertices_texCoord[] =
	{
		Vec2f(0.0f, 0.0f),
		Vec2f(planeScale, 0.0f),
		Vec2f(0.0f, planeScale),
		Vec2f(planeScale, planeScale)
	};

	VertexNorm g_PlaneVertices_norm[] =
	{
		Vec3f(0.f, 1.f, 0.f),
		Vec3f(0.f, 1.f, 0.f),
		Vec3f(0.f, 1.f, 0.f),
		Vec3f(0.f, 1.f, 0.f)
	};

	PlaneMesh::PlaneMesh()
		: Mesh(NUM_VERTS, NUM_INDICES)
	{
		poVertexBuffer_pos = CreateVertexBuffer(sizeof(g_Vertices_pos), g_Vertices_pos);
		poVertexBuffer_color = CreateVertexBuffer(sizeof(g_Vertices_color), g_Vertices_color);
		poVertexBuffer_texCoord = CreateVertexBuffer(sizeof(g_PlaneVertices_texCoord), g_PlaneVertices_texCoord);
		poVertexBuffer_norm = CreateVertexBuffer(sizeof(g_PlaneVertices_norm), g_PlaneVertices_norm);

		poIndexBuffer = CreateIndexBuffer(sizeof(g_Indicies), g_Indicies);

		poConstantBuff_Projection = CreateConstantBuffer(sizeof(Mat4));
		poConstantBuff_View = CreateConstantBuffer(sizeof(Mat4));
		poConstantBuff_World = CreateConstantBuffer(sizeof(Mat4));

		poConstantBuff_lightColor = CreateConstantBuffer(sizeof(Vec3));
		poConstantBuff_lightPos = CreateConstantBuffer(sizeof(Vec3));
	}

	PlaneMesh::~PlaneMesh()
	{
	}
}