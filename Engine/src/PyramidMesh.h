#ifndef _PyramidMesh
#define _PyramidMesh

#include "Mesh.h"

namespace Azul
{
	class PyramidMesh : public Mesh
	{
	public:
		PyramidMesh();
		PyramidMesh(const PyramidMesh&) = delete;
		PyramidMesh& operator = (PyramidMesh&) = delete;
		virtual ~PyramidMesh();

	private:
		static constexpr unsigned int NUM_VERTS = 5;
		static constexpr unsigned int NUM_POLYS = 6;
		static constexpr unsigned int NUM_INDICES = NUM_POLYS * 3;

		static VertexPos g_Vertices_pos[NUM_VERTS];
		static VertexColor g_Vertices_color[NUM_VERTS];
		static uint32_t g_Indicies[NUM_INDICES];
	};
}

#endif