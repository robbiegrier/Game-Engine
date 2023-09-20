#ifndef _DiamondMesh
#define _DiamondMesh

#include "Mesh.h"

namespace Azul
{
	class DiamondMesh : public Mesh
	{
	public:
		DiamondMesh();
		DiamondMesh(const DiamondMesh&) = delete;
		DiamondMesh& operator = (DiamondMesh&) = delete;
		virtual ~DiamondMesh();

	private:
		static constexpr unsigned int NUM_VERTS = 6;
		static constexpr unsigned int NUM_POLYS = 8;
		static constexpr unsigned int NUM_INDICES = NUM_POLYS * 3;

		static VertexPos g_Vertices_pos[NUM_VERTS];
		static VertexColor g_Vertices_color[NUM_VERTS];
		static uint32_t g_Indicies[NUM_INDICES];
	};
}

#endif