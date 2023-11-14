//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_PYRAMID_H
#define MESH_PYRAMID_H

#include "Mesh.h"

class MeshPyramid : public Mesh
{
public:
	MeshPyramid();
	MeshPyramid(const MeshPyramid &) = delete;
	MeshPyramid &operator = (MeshPyramid &) = delete;
	virtual ~MeshPyramid() = default;


	// Data

};

#endif

// --- End of File ---
