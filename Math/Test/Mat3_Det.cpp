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

TEST(Mat3_Det, TestConfig::ALL)
{
#if Mat3_Det

	Vec3 v0(1.0f, 2.0f, 3.0f);
	Vec3 v1(5.0f, 6.0f, 7.0f);
	Vec3 v2(-0.8f, -7.0f, -0.6f);

	Mat3 A(v0, v1, v2);

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);

	CHECK(A[m8] == -0.8f);
	CHECK(A[m9] == -7.0f);
	CHECK(A[m10] == -0.60f);

	const float value = A.det();

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);

	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	
	CHECK(A[m8] == -0.8f);
	CHECK(A[m9] == -7.0f);
	CHECK(A[m10] == -0.60f);

	CHECK(eq(value, -50.4f, MATH_TOLERANCE));

#endif
} TEST_END

// --- End of File ---
