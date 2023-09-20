//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------  

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
#define eq	Util::isEqual 
using namespace Azul;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Vec4_MultEqual_Mat4, TestConfig::ALL)
{
#if Vec4_MultEqual_Mat4

	Vec4 vA(2.0f, 3.0f, 4.0f, -2.0f);

	CHECK(vA[x] == 2.0f);
	CHECK(vA[y] == 3.0f);
	CHECK(vA[z] == 4.0f);
	CHECK(vA[w] == -2.0f);

	Vec4 V0(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4 V1(7.0f, 6.0f, 5.0f, 3.0f);
	Vec4 V2(-4.0f, -2.0f, -1.0f, -4.0f);
	Vec4 V3(9.0f, -7.0f, -2.0f, 5.0f);

	Mat4 M(V0, V1, V2, V3);

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m3] == 4.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m7] == 3.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);
	CHECK(M[m11] == -4.0f);
	CHECK(M[m12] == 9.0f);
	CHECK(M[m13] == -7.0f);
	CHECK(M[m14] == -2.0f);
	CHECK(M[m15] == 5.0f);

	vA *= M;

	CHECK(M[m0] == 1.0f);
	CHECK(M[m1] == 2.0f);
	CHECK(M[m2] == 3.0f);
	CHECK(M[m3] == 4.0f);
	CHECK(M[m4] == 7.0f);
	CHECK(M[m5] == 6.0f);
	CHECK(M[m6] == 5.0f);
	CHECK(M[m7] == 3.0f);
	CHECK(M[m8] == -4.0f);
	CHECK(M[m9] == -2.0f);
	CHECK(M[m10] == -1.0f);
	CHECK(M[m11] == -4.0f);
	CHECK(M[m12] == 9.0f);
	CHECK(M[m13] == -7.0f);
	CHECK(M[m14] == -2.0f);
	CHECK(M[m15] == 5.0f);

	CHECK(eq(vA[x], -11.0f, MATH_TOLERANCE));
	CHECK(eq(vA[y], 28.0f, MATH_TOLERANCE));
	CHECK(eq(vA[z], 21.0f, MATH_TOLERANCE));
	CHECK(eq(vA[w], -9.0f, MATH_TOLERANCE));

#endif
} TEST_END

// ---  End of File ---------------
