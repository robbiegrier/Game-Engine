#ifndef _CubeModel
#define _CubeModel

#include "Mesh.h"

namespace Azul
{
	class CubeMesh : public Mesh
	{
	public:
		CubeMesh();
		CubeMesh(const CubeMesh&) = delete;
		CubeMesh& operator = (CubeMesh&) = delete;
		virtual ~CubeMesh();

	private:
		static constexpr unsigned int NUM_VERTS = 3 * 12;
		static constexpr unsigned int NUM_POLYS = 12;
		static constexpr unsigned int NUM_INDICES = NUM_POLYS * 3;

		static VertexPos g_Vertices_pos[NUM_VERTS];
		static VertexColor g_Vertices_color[NUM_VERTS];
		static uint32_t g_Indicies[NUM_INDICES];
	};
}

#endif
