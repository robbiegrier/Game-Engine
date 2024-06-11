#include "Plane.h"
#include "MathEngine.h"

namespace Azul
{
	constexpr float planeScale = 100.f;
	constexpr float tiling = 0.1f;

	VertexPos PlaneMesh::g_Vertices_pos[] =
	{
		Vec3f(-planeScale,  0.f, -planeScale),
		Vec3f(-planeScale, 0.f, planeScale),
		Vec3f(planeScale, 0.f, -planeScale),
		Vec3f(planeScale,  0.f, planeScale)
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
		Vec2f(planeScale * tiling, 0.0f),
		Vec2f(0.0f, planeScale * tiling),
		Vec2f(planeScale * tiling, planeScale * tiling)
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
		pVertexBufferPosition = CreateVertexBuffer(sizeof(g_Vertices_pos), g_Vertices_pos);
		pVertexBufferColor = CreateVertexBuffer(sizeof(g_Vertices_color), g_Vertices_color);
		pVertexBufferTextureCoordinates = CreateVertexBuffer(sizeof(g_PlaneVertices_texCoord), g_PlaneVertices_texCoord);
		pVertexBufferNormal = CreateVertexBuffer(sizeof(g_PlaneVertices_norm), g_PlaneVertices_norm);
		pIndexBufferTriangles = CreateIndexBuffer(sizeof(g_Indicies), g_Indicies);
		pConstantBufferLightColor = CreateConstantBuffer(sizeof(Vec3));
		pConstantBufferLightPosition = CreateConstantBuffer(sizeof(Vec3));

		HackSetBoundingSphereData(g_Vertices_pos);
		HackSetBoundingBoxData(g_Vertices_pos);
	}

	PlaneMesh::~PlaneMesh()
	{
	}
}