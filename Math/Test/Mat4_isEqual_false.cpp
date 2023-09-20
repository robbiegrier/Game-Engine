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

TEST(Mat4_isEqual_false, TestConfig::ALL)
{
#if Mat4_isEqual_false

	Vec4 V0(1.0f, 2.0f, 3.0f, 4.0f);
	Vec4 V1(5.0f, 6.0f, 7.0f, 8.0f);
	Vec4 V2(9.0f, 10.0f, 11.0f, 12.0f);
	Vec4 V3(13.0f, 14.0f, 15.0f, 16.0f);

	Mat4 A(V0, V1, V2, V3);

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	CHECK(A[m3] == 4.0f);
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	CHECK(A[m7] == 8.0f);
	CHECK(A[m8] == 9.0f);
	CHECK(A[m9] == 10.0f);
	CHECK(A[m10] == 11.0f);
	CHECK(A[m11] == 12.0f);
	CHECK(A[m12] == 13.0f);
	CHECK(A[m13] == 14.0f);
	CHECK(A[m14] == 15.0f);
	CHECK(A[m15] == 16.0f);

	Vec4 V4(20.0f, 21.0f, 22.0f, 23.0f);
	Vec4 V5(24.0f, 25.0f, 26.0f, 27.0f);
	Vec4 V6(28.0f, 29.0f, 30.0f, 31.0f);
	Vec4 V7(32.0f, 33.0f, 34.0f, 35.0f);

	Mat4 B(V4, V5, V6, V7);

	CHECK(B[m0] == 20.0f);
	CHECK(B[m1] == 21.0f);
	CHECK(B[m2] == 22.0f);
	CHECK(B[m3] == 23.0f);
	CHECK(B[m4] == 24.0f);
	CHECK(B[m5] == 25.0f);
	CHECK(B[m6] == 26.0f);
	CHECK(B[m7] == 27.0f);
	CHECK(B[m8] == 28.0f);
	CHECK(B[m9] == 29.0f);
	CHECK(B[m10] == 30.0f);
	CHECK(B[m11] == 31.0f);
	CHECK(B[m12] == 32.0f);
	CHECK(B[m13] == 33.0f);
	CHECK(B[m14] == 34.0f);
	CHECK(B[m15] == 35.0f);

	bool value = A.isEqual(B);

	CHECK(value == false);

#endif
} TEST_END

// ---  End of File ---------------
