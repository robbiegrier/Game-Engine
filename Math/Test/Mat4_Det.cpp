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

TEST(Mat4_Det, TestConfig::ALL)
{
#if Mat4_Det

	Vec4 v0(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4 v1(5.0f, 6.0f, 7.0f, 8.0f);
	Vec4 v2(-0.8f, -7.0f, -0.6f, -5.0f);
	Vec4 v3(-4.0f, -3.0f, 2.0f, -0.1f);

	Mat4 A(v0, v1, v2, v3);

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	CHECK(A[m3] == 4.0f);
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	CHECK(A[m7] == 8.0f);
	CHECK(A[m8] == -0.8f);
	CHECK(A[m9] == -7.0f);
	CHECK(A[m10] == -0.60f);
	CHECK(A[m11] == -5.0f);
	CHECK(A[m12] == -4.0f);
	CHECK(A[m13] == -3.0f);
	CHECK(A[m14] == 2.0f);
	CHECK(A[m15] == -0.10f);

	const float value = A.det();

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	CHECK(A[m3] == 4.0f);
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	CHECK(A[m7] == 8.0f);
	CHECK(A[m8] == -0.8f);
	CHECK(A[m9] == -7.0f);
	CHECK(A[m10] == -0.60f);
	CHECK(A[m11] == -5.0f);
	CHECK(A[m12] == -4.0f);
	CHECK(A[m13] == -3.0f);
	CHECK(A[m14] == 2.0f);
	CHECK(A[m15] == -0.10f);

	CHECK(eq(value, 185.04f, MATH_TOLERANCE));

#endif
} TEST_END

// --- End of File ---
