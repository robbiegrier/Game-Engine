//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_SPHERE_H
#define MESH_SPHERE_H

#include "Mesh.h"

namespace Azul
{
	class MeshSphere : public Mesh
	{
	public:
		MeshSphere();
		~MeshSphere();

		// tells the compiler do not create or allow it to be used, c++11
		MeshSphere(const MeshSphere &) = delete;
		MeshSphere &operator=(const MeshSphere &other) = delete;
	
	};
}

#endif

// --- End of File ---
