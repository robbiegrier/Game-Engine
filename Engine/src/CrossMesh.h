#ifndef _CrossMesh
#define _CrossMesh

#include "Mesh.h"

namespace Azul
{
	class CrossMesh : public Mesh
	{
	public:
		CrossMesh();
		CrossMesh(const CrossMesh&) = delete;
		CrossMesh& operator = (CrossMesh&) = delete;
		virtual ~CrossMesh();

	private:
		static constexpr unsigned int NUM_VERTS = 24;
		static constexpr unsigned int NUM_POLYS = 44;
		static constexpr unsigned int NUM_INDICES = NUM_POLYS * 3;

		static VertexPos g_Vertices_pos[NUM_VERTS];
		static VertexColor g_Vertices_color[NUM_VERTS];
		static uint32_t g_Indicies[NUM_INDICES];
	};
}

#endif