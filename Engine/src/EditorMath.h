#ifndef _EditorMath
#define _EditorMath

#include "MathEngine.h"

namespace Azul
{
	class GameObject;

	// Math utilities for editor operations
	class EditorMath
	{
	public:
		static Vec3 ProjectVector(const Vec3& projected, const Vec3& surface);
		static bool RayVsSphere(const Vec3& rayOrigin, const Vec3& rayDirection, const Vec3& sphereCenter, float sphereRadius);
		static Vec3 ClosestPointBetweenLineAndRay(const Vec3& linePosition, const Vec3& lineDirection, const Vec3& rayOrigin, const Vec3& rayDirection);
		static bool RayVsPlaneIntersection(const Vec3& planePoint, const Vec3& planeNormal, const Vec3& rayOrigin, const Vec3& rayDir);
		static bool LineVsBoxIntersection(const Vec3& boxMin, const Vec3& boxMax, const Vec3& lineStart, const Vec3& lineEnd);
		static bool RayVsOBBIntersection(const Vec3& rayOrigin, const Vec3& rayDir, const Vec3& aabbMin, const Vec3& aabbMax, const Mat4& boxWorld, float& intersectDist, float& error);
		static Vec3 GetRayPointOnPlane(const Vec3& planePoint, const Vec3& planeNormal, const Vec3& rayOrigin, const Vec3& rayDir);
		static Vec3 QuatToEulerAngles(const Quat& q);
		static Quat EulerAnglesToQuat(const Vec3& q);
		static float RadiansToDegrees(float r);
		static float DegreesToRadians(float d);
		static float SquareDistance(const Vec3& a, const Vec3& b);
		static float SquareLen(const Vec3& a);

		static void Print(const Vec3& v, const char* m);
	};
}

#endif