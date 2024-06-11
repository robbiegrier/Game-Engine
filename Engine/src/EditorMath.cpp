#include "EditorMath.h"
#include "GameObject.h"

namespace Azul
{
	bool EditorMath::RayVsSphere(const Vec3& rayOrigin, const Vec3& rayDirection, const Vec3& sphereCenter, float sphereRadius)
	{
		const Vec3 toSphereCenter = sphereCenter - rayOrigin;
		const Vec3 projection = ProjectVector(toSphereCenter, rayDirection);
		const Vec3 closestPoint = rayOrigin + projection;

		const bool isInFront = toSphereCenter.dot(rayDirection) > 0.f;
		const bool rayInSphere = (closestPoint - sphereCenter).len() < sphereRadius;

		return isInFront && rayInSphere;
	}

	Vec3 EditorMath::ProjectVector(const Vec3& projected, const Vec3& surface)
	{
		Vec3 surfaceNorm = surface.getNorm();
		float s = projected.dot(surfaceNorm);
		return s * surfaceNorm;
	}

	Vec3 EditorMath::ClosestPointBetweenLineAndRay(const Vec3& linePosition, const Vec3& lineDirection, const Vec3& rayOrigin, const Vec3& rayDirection)
	{
		Vec3 toLinePos = linePosition - rayOrigin;
		Vec3 directionsCross = lineDirection.cross(rayDirection).getNorm();
		Vec3 rejection = toLinePos - EditorMath::ProjectVector(toLinePos, rayDirection) - EditorMath::ProjectVector(toLinePos, directionsCross);
		float divisor = lineDirection.dot(rejection.getNorm());

		if (fabsf(divisor) < MATH_TOLERANCE)
		{
			divisor = MATH_TOLERANCE;
		}

		float distanceToLine = rejection.len() / divisor;
		Vec3 closestApproach = linePosition - (lineDirection * distanceToLine);
		return closestApproach;
	}

	bool EditorMath::RayVsPlaneIntersection(const Vec3& planePoint, const Vec3& planeNormal, const Vec3& rayOrigin, const Vec3& rayDir)
	{
		float normalDotRayDir = planeNormal.dot(rayDir);

		if (fabsf(normalDotRayDir) > MATH_TOLERANCE)
		{
			Vec3 planePointToRayOrigin = planePoint - rayOrigin;
			float t = planePointToRayOrigin.dot(planeNormal) / normalDotRayDir;
			return t >= 0.f;
		}

		return false;
	}

	int GetIntersection(float fDst1, float fDst2, const Vec3& P1, const Vec3& P2, Vec3& Hit)
	{
		if ((fDst1 * fDst2) >= 0.0f) return 0;
		if (fDst1 == fDst2) return 0;
		Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
		return 1;
	}

	int InBox(const Vec3& Hit, const Vec3& B1, const Vec3& B2, const int Axis)
	{
		if (Axis == 1 && Hit[z] > B1[z] && Hit[z] < B2[z] && Hit[y] > B1[y] && Hit[y] < B2[y]) return 1;
		if (Axis == 2 && Hit[z] > B1[z] && Hit[z] < B2[z] && Hit[x] > B1[x] && Hit[x] < B2[x]) return 1;
		if (Axis == 3 && Hit[x] > B1[x] && Hit[x] < B2[x] && Hit[y] > B1[y] && Hit[y] < B2[y]) return 1;
		return 0;
	}

	bool EditorMath::LineVsBoxIntersection(const Vec3& B1, const Vec3& B2, const Vec3& L1, const Vec3& L2)
	{
		Vec3 Hit;
		if (L2[x] < B1[x] && L1[x] < B1[x]) return false;
		if (L2[x] > B2[x] && L1[x] > B2[x]) return false;
		if (L2[y] < B1[y] && L1[y] < B1[y]) return false;
		if (L2[y] > B2[y] && L1[y] > B2[y]) return false;
		if (L2[z] < B1[z] && L1[z] < B1[z]) return false;
		if (L2[z] > B2[z] && L1[z] > B2[z]) return false;
		if (L1[x] > B1[x] && L1[x] < B2[x] &&
			L1[y] > B1[y] && L1[y] < B2[y] &&
			L1[z] > B1[z] && L1[z] < B2[z])
		{
			Hit = L1;
			return true;
		}
		if ((GetIntersection(L1[x] - B1[x], L2[x] - B1[x], L1, L2, Hit) && InBox(Hit, B1, B2, 1))
			|| (GetIntersection(L1[y] - B1[y], L2[y] - B1[y], L1, L2, Hit) && InBox(Hit, B1, B2, 2))
			|| (GetIntersection(L1[z] - B1[z], L2[z] - B1[z], L1, L2, Hit) && InBox(Hit, B1, B2, 3))
			|| (GetIntersection(L1[x] - B2[x], L2[x] - B2[x], L1, L2, Hit) && InBox(Hit, B1, B2, 1))
			|| (GetIntersection(L1[y] - B2[y], L2[y] - B2[y], L1, L2, Hit) && InBox(Hit, B1, B2, 2))
			|| (GetIntersection(L1[z] - B2[z], L2[z] - B2[z], L1, L2, Hit) && InBox(Hit, B1, B2, 3)))
		{
			return true;
		}

		return false;
	}

	Vec3 EditorMath::GetRayPointOnPlane(const Vec3& planePoint, const Vec3& planeNormal, const Vec3& rayOrigin, const Vec3& rayDir)
	{
		float normalDotRayDir = planeNormal.dot(rayDir);

		if (fabsf(normalDotRayDir) > MATH_TOLERANCE)
		{
			Vec3 planePointToRayOrigin = planePoint - rayOrigin;
			float t = planePointToRayOrigin.dot(planeNormal) / normalDotRayDir;
			return rayOrigin + (rayDir * t);
		}

		return Vec3();
	}

	Vec3 EditorMath::QuatToEulerAngles(const Quat& q)
	{
		Vec3 angles;

		float sinrCosp = 2 * (q[w] * q[x] + q[y] * q[z]);
		float cosrCosp = 1 - 2 * (q[x] * q[x] + q[y] * q[y]);
		angles[x] = std::atan2(sinrCosp, cosrCosp);

		float sinp = std::sqrt(1 + 2 * (q[w] * q[y] - q[x] * q[z]));
		float cosp = std::sqrt(1 - 2 * (q[w] * q[y] - q[x] * q[z]));
		angles[y] = 2 * std::atan2(sinp, cosp) - MATH_PI2;

		float sinyCosp = 2 * (q[w] * q[z] + q[x] * q[y]);
		float cosyCosp = 1 - 2 * (q[y] * q[y] + q[z] * q[z]);
		angles[z] = std::atan2(sinyCosp, cosyCosp);

		return angles;
	}

	Quat EditorMath::EulerAnglesToQuat(const Vec3& v)
	{
		float cr = cos(v[x] * 0.5f);
		float sr = sin(v[x] * 0.5f);
		float cp = cos(v[y] * 0.5f);
		float sp = sin(v[y] * 0.5f);
		float cy = cos(v[z] * 0.5f);
		float sy = sin(v[z] * 0.5f);

		Quat q;
		q[w] = cr * cp * cy + sr * sp * sy;
		q[x] = sr * cp * cy - cr * sp * sy;
		q[y] = cr * sp * cy + sr * cp * sy;
		q[z] = cr * cp * sy - sr * sp * cy;

		return q.getNorm();
	}

	float EditorMath::RadiansToDegrees(float r)
	{
		return r * MATH_180_PI;
	}

	float EditorMath::DegreesToRadians(float d)
	{
		return d * MATH_PI_180;
	}

	float EditorMath::SquareDistance(const Vec3& a, const Vec3& b)
	{
		return SquareLen(b - a);
	}

	float EditorMath::SquareLen(const Vec3& a)
	{
		return (a.x() * a.x()) + (a.y() * a.y()) + (a.z() * a.z());
	}

	void EditorMath::Print(const Vec3& v, const char* m)
	{
		Trace::out("%s: [%.4f %.4f %.4f]\n", m, v[x], v[y], v[z]);
	}

	bool EditorMath::RayVsOBBIntersection(const Vec3& rayOrigin, const Vec3& rayDirection, const Vec3& aabbMinIn, const Vec3& aabbMaxIn, const Mat4& boxWorld, float& intersectDist, float& error)
	{
		float tMin = 0.0f;
		float tMax = 100000.0f;
		error = 0.f;

		Vec3 scaleBoxWorld = Vec3(boxWorld.get(Row4::i0).len(), boxWorld.get(Row4::i1).len(), boxWorld.get(Row4::i2).len());
		Scale scaleMat(scaleBoxWorld);

		Vec3 aabbMin = aabbMinIn * Mat3(scaleMat);
		Vec3 aabbMax = aabbMaxIn * Mat3(scaleMat);

		Vec3 OBBposition_worldspace(boxWorld.get(Row4::i3).x(), boxWorld.get(Row4::i3).y(), boxWorld.get(Row4::i3).z());

		Vec3 delta = OBBposition_worldspace - rayOrigin;

		// Test intersection with the 2 planes perpendicular to the OBB's X axis
		{
			Vec3 xaxis(boxWorld.get(Row4::i0).x(), boxWorld.get(Row4::i0).y(), boxWorld.get(Row4::i0).z());
			xaxis.norm();
			float e = xaxis.dot(delta);
			float f = rayDirection.dot(xaxis);

			if (fabs(f) > 0.001f) { // Standard case
				float t1 = (e + aabbMin.x()) / f; // Intersection with the "left" plane
				float t2 = (e + aabbMax.x()) / f; // Intersection with the "right" plane
				// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

				// We want t1 to represent the nearest intersection,
				// so if it's not the case, invert t1 and t2
				if (t1 > t2) {
					float w = t1; t1 = t2; t2 = w; // swap t1 and t2
				}

				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
				if (t2 < tMax)
					tMax = t2;
				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
				if (t1 > tMin)
					tMin = t1;

				// And here's the trick :
				// If "far" is closer than "near", then there is NO intersection.
				// See the images in the tutorials for the visual explanation.
				if (tMax < tMin)
				{
					error = tMin - tMax;
					return false;
				}
			}
			else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
				if (-e + aabbMin.x() > 0.0f || -e + aabbMax.x() < 0.0f)
					return false;
			}
		}

		{
			Vec3 yaxis(boxWorld.get(Row4::i1).x(), boxWorld.get(Row4::i1).y(), boxWorld.get(Row4::i1).z());
			yaxis.norm();
			float e = yaxis.dot(delta);
			float f = rayDirection.dot(yaxis);

			if (fabs(f) > 0.001f) {
				float t1 = (e + aabbMin.y()) / f;
				float t2 = (e + aabbMax.y()) / f;

				if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

				if (t2 < tMax)
					tMax = t2;
				if (t1 > tMin)
					tMin = t1;
				if (tMin > tMax)
				{
					error = tMin - tMax;
					return false;
				}
			}
			else {
				if (-e + aabbMin.y() > 0.0f || -e + aabbMax.y() < 0.0f)
					return false;
			}
		}

		{
			Vec3 zaxis(boxWorld.get(Row4::i2).x(), boxWorld.get(Row4::i2).y(), boxWorld.get(Row4::i2).z());
			zaxis.norm();
			float e = zaxis.dot(delta);
			float f = rayDirection.dot(zaxis);

			if (fabs(f) > 0.001f) {
				float t1 = (e + aabbMin.z()) / f;
				float t2 = (e + aabbMax.z()) / f;

				if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

				if (t2 < tMax)
					tMax = t2;
				if (t1 > tMin)
					tMin = t1;
				if (tMin > tMax)
				{
					error = tMin - tMax;
					return false;
				}
			}
			else {
				if (-e + aabbMin.z() > 0.0f || -e + aabbMax.z() < 0.0f)
					return false;
			}
		}

		intersectDist = tMin;
		return true;
	}
}