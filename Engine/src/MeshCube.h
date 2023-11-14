//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_CUBE_H
#define MESH_CUBE_H

#include "Mesh.h"

class MeshCube : public Mesh
{
public:
	MeshCube();
	MeshCube(const MeshCube &) = delete;
	MeshCube &operator = (MeshCube &) = delete;
	virtual ~MeshCube();


	// Data

};

#endif

// --- End of File ---
