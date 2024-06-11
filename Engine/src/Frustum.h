#ifndef _Frustum
#define _Frustum

#include "MathEngine.h"

namespace Azul
{
	struct Plane : public Align16
	{
		Plane() = default;
		Plane(const Plane&) = default;
		Plane& operator=(const Plane&) = default;
		~Plane() = default;

		explicit Plane(const Vec3& inNormal, const Vec3& inPoint);

		float GetOffset() const { return offset; }

		Vec3 normal;
		Vec3 point;
		float offset;
	};

	struct Frustum : public Align16
	{
	public:
		Frustum() = default;
		Frustum(const Frustum&) = default;
		Frustum& operator=(const Frustum&) = default;
		~Frustum() = default;

		const Plane& GetPlane(unsigned int i) const { return planes[i]; }

		static constexpr unsigned int SIDES = 6u;
		Plane planes[SIDES];
	};
}

#endif