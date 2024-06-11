#include "QTNode.h"

namespace Azul
{
	bool QTNode::RayIntersection(QTHit& outHit, const Vec3& rayOrigin, const Vec3& rayDir)
	{
		Vec3 boxMin = GetAABBMin();
		Vec3 boxMax = GetAABBMax();

		float tmin, tmax, tymin, tymax, tzmin, tzmax;
		outHit.hit = false;

		if (rayDir.x() >= 0) {
			tmin = (boxMin.x() - rayOrigin.x()) / rayDir.x();
			tmax = (boxMax.x() - rayOrigin.x()) / rayDir.x();
		}
		else {
			tmin = (boxMax.x() - rayOrigin.x()) / rayDir.x();
			tmax = (boxMin.x() - rayOrigin.x()) / rayDir.x();
		}
		if (rayDir.y() >= 0) {
			tymin = (boxMin.y() - rayOrigin.y()) / rayDir.y();
			tymax = (boxMax.y() - rayOrigin.y()) / rayDir.y();
		}
		else {
			tymin = (boxMax.y() - rayOrigin.y()) / rayDir.y();
			tymax = (boxMin.y() - rayOrigin.y()) / rayDir.y();
		}
		if ((tmin > tymax) || (tymin > tmax))
			return false;
		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;
		if (rayDir.z() >= 0) {
			tzmin = (boxMin.z() - rayOrigin.z()) / rayDir.z();
			tzmax = (boxMax.z() - rayOrigin.z()) / rayDir.z();
		}
		else {
			tzmin = (boxMax.z() - rayOrigin.z()) / rayDir.z();
			tzmax = (boxMin.z() - rayOrigin.z()) / rayDir.z();
		}
		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		//return ((tmin < t1) && (tmax > t0));
		//return false;

		outHit.entryDistance = tmin;
		outHit.exitDistance = tmax;
		outHit.nodeIndex = nodeIndex;
		outHit.hit = true;
		return true;
	}

	//https://github.com/erich666/jgt-code/blob/master/Volume_02/Number_1/Moller1997a/raytri.c
	bool RayVsTriangle(QTTriangleHit& outHit, const Vec3& rayOrigin, const Vec3& rayDir, const Vec3& vert0, const Vec3& vert1, const Vec3& vert2)
	{
		Vec3 edge1, edge2, tvec, pvec, qvec;
		float det, inv_det;

		outHit.hit = false;
		outHit.verts[0] = vert0;
		outHit.verts[1] = vert1;
		outHit.verts[2] = vert2;

		/* find vectors for two edges sharing vert0 */
		edge1 = vert1 - vert0;
		edge2 = vert2 - vert0;

		/* begin calculating determinant - also used to calculate U parameter */
		pvec = rayDir.cross(edge2);

		/* if determinant is near zero, ray lies in plane of triangle */
		det = edge1.dot(pvec);

		/* calculate distance from vert0 to ray origin */
		tvec = rayOrigin - vert0;
		inv_det = 1.0f / det;

		qvec = tvec.cross(edge1);

		if (det > MATH_TOLERANCE)
		{
			outHit.u = tvec.dot(pvec);
			if (outHit.u < 0.0f || outHit.u > det)
				return false;

			/* calculate V parameter and test bounds */
			outHit.v = rayDir.dot(qvec);
			if (outHit.v < 0.0f || outHit.u + outHit.v > det)
				return false;
		}
		else if (det < -MATH_TOLERANCE)
		{
			/* calculate U parameter and test bounds */
			outHit.u = tvec.dot(pvec);
			if (outHit.u > 0.0f || outHit.u < det)
				return false;

			/* calculate V parameter and test bounds */
			outHit.v = rayDir.dot(qvec);
			if (outHit.v > 0.0f || outHit.u + outHit.v < det)
				return false;
		}
		else return false;  /* ray is parallell to the plane of the triangle */

		outHit.distance = edge2.dot(qvec) * inv_det;
		outHit.u *= inv_det;
		outHit.v *= inv_det;
		outHit.hit = true;

		return true;
	}

	bool QTNode::FrustumIntersection(const Frustum& frustum)
	{
		constexpr static float FORCE_MIN = std::numeric_limits<float>().min();
		constexpr static float FORCE_MAX = std::numeric_limits<float>().max();

		for (uint i = 0u; i < Frustum::SIDES; i++)
		{
			// planes have unit-length normal, offset = -dot(normal, point on plane)
			const Plane& plane = frustum.GetPlane(i);
			const bool nx = plane.normal.x() < 0.f;
			const bool ny = plane.normal.y() < 0.f;
			const bool nz = plane.normal.z() < 0.f;

			const float dot = (-plane.normal.x() * (nx ? area.max.x : area.min.x)) +
				(-plane.normal.y() * (ny ? FORCE_MAX : FORCE_MIN)) +
				(-plane.normal.z() * (nz ? area.max.z : area.min.z));

			if (dot < plane.offset)
			{
				return false;
			}
		}

		return true;
	}
}