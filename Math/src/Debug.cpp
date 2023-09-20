//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------

// --------------------------------
// ---      DO NOT MODIFY       ---
// --------------------------------

#include "MathEngine.h"
#include "Debug.h"

namespace Azul
{
	void Debug::Print(const char* pName, const Vec4& r)
	{
		float vx = fabs(r[x]) < MATH_PRINT_TOLERANCE ? 0.0f : r[x];
		float vy = fabs(r[y]) < MATH_PRINT_TOLERANCE ? 0.0f : r[y];
		float vz = fabs(r[z]) < MATH_PRINT_TOLERANCE ? 0.0f : r[z];
		float vw = fabs(r[w]) < MATH_PRINT_TOLERANCE ? 0.0f : r[w];
		Trace::out("%s:(Vec4) \n", pName);
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", vx, vy, vz, vw);
		Trace::out("\n");
	}

	void Debug::Print(const char* pName, const Vec3& r)
	{
		float vx = fabs(r[x]) < MATH_PRINT_TOLERANCE ? 0.0f : r[x];
		float vy = fabs(r[y]) < MATH_PRINT_TOLERANCE ? 0.0f : r[y];
		float vz = fabs(r[z]) < MATH_PRINT_TOLERANCE ? 0.0f : r[z];
		Trace::out("%s:(Vec3) \n", pName);
		Trace::out(" %9.4g %9.4g %9.4g\n", vx, vy, vz);
		Trace::out("\n");
	}

	void Debug::Print(const char* pName, const Mat4& r)
	{
		float x0 = fabs(r[m0]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m0];
		float x1 = fabs(r[m1]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m1];
		float x2 = fabs(r[m2]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m2];
		float x3 = fabs(r[m3]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m3];
		float x4 = fabs(r[m4]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m4];
		float x5 = fabs(r[m5]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m5];
		float x6 = fabs(r[m6]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m6];
		float x7 = fabs(r[m7]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m7];
		float x8 = fabs(r[m8]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m8];
		float x9 = fabs(r[m9]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m9];
		float x10 = fabs(r[m10]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m10];
		float x11 = fabs(r[m11]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m11];
		float x12 = fabs(r[m12]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m12];
		float x13 = fabs(r[m13]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m13];
		float x14 = fabs(r[m14]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m14];
		float x15 = fabs(r[m15]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m15];
		Trace::out("%s:(Mat4) \n", pName);
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", x0, x1, x2, x3);
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", x4, x5, x6, x7);
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", x8, x9, x10, x11);
		Trace::out(" %9.4g %9.4g %9.4g %9.4g \n", x12, x13, x14, x15);
		Trace::out("\n");
	}

	void Debug::Print(const char* pName, const Mat3& r)
	{
		float x0 = fabs(r[m0]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m0];
		float x1 = fabs(r[m1]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m1];
		float x2 = fabs(r[m2]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m2];
		float x4 = fabs(r[m4]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m4];
		float x5 = fabs(r[m5]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m5];
		float x6 = fabs(r[m6]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m6];
		float x8 = fabs(r[m8]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m8];
		float x9 = fabs(r[m9]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m9];
		float x10 = fabs(r[m10]) < MATH_PRINT_TOLERANCE ? 0.0f : r[m10];
		Trace::out("%s:(Mat3) \n", pName);
		Trace::out(" %9.4g %9.4g %9.4g  \n", x0, x1, x2);
		Trace::out(" %9.4g %9.4g %9.4g  \n", x4, x5, x6);
		Trace::out(" %9.4g %9.4g %9.4g  \n", x8, x9, x10);
		Trace::out("\n");
	}
}

//--- End of File ---