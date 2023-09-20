#ifndef _PlaneMeshModel
#define _PlaneMeshModel

#include "Mesh.h"

namespace Azul
{
	class PlaneMesh : public Mesh
	{
	public:
		PlaneMesh();
		PlaneMesh(const PlaneMesh&) = delete;
		PlaneMesh& operator = (PlaneMesh&) = delete;
		virtual ~PlaneMesh();

	private:
		static constexpr unsigned int NUM_VERTS = 4;
		static constexpr unsigned int NUM_POLYS = 2;
		static constexpr unsigned int NUM_INDICES = NUM_POLYS * 3;

		static VertexPos g_Vertices_pos[NUM_VERTS];
		static VertexColor g_Vertices_color[NUM_VERTS];
		static uint32_t g_Indicies[NUM_INDICES];
	};
}

#endif
