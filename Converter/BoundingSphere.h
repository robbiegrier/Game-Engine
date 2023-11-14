#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include "MathEngine.h"

namespace Azul
{
	struct Sphere
	{
		Vec3 cntr;
		float rad;
		float pad[3];
	};

	void RitterSphere(Sphere& s, Vec3* pt, int numPts);
}

#endif
