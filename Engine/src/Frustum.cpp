#include "Frustum.h"

namespace Azul
{
	Plane::Plane(const Vec3& inNormal, const Vec3& inPoint)
		: normal(inNormal), point(inPoint), offset(-(normal.dot(point)))
	{
	}
}