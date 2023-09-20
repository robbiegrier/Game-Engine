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

TEST(Mat3_AddEqual_Mat3, TestConfig::ALL)
{
#if Mat3_AddEqual_Mat3

	Vec3 V0(1.0f, 2.0f, 3.0f);
	Vec3 V1(5.0f, 6.0f, 7.0f);
	Vec3 V2(9.0f, 10.0f, 11.0f);

	Mat3 A(V0, V1, V2);

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);

	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);

	CHECK(A[m8] == 9.0f);
	CHECK(A[m9] == 10.0f);
	CHECK(A[m10] == 11.0f);

	Vec3 V4(20.0f, 21.0f, 22.0f);
	Vec3 V5(24.0f, 25.0f, 26.0f);
	Vec3 V6(28.0f, 29.0f, 30.0f);

	Mat3 B(V4, V5, V6);

	CHECK(B[m0] == 20.0f);
	CHECK(B[m1] == 21.0f);
	CHECK(B[m2] == 22.0f);

	CHECK(B[m4] == 24.0f);
	CHECK(B[m5] == 25.0f);
	CHECK(B[m6] == 26.0f);

	CHECK(B[m8] == 28.0f);
	CHECK(B[m9] == 29.0f);
	CHECK(B[m10] == 30.0f);

	Mat3 C;

	A += B;

	CHECK(B[m0] == 20.0f);
	CHECK(B[m1] == 21.0f);
	CHECK(B[m2] == 22.0f);

	CHECK(B[m4] == 24.0f);
	CHECK(B[m5] == 25.0f);
	CHECK(B[m6] == 26.0f);

	CHECK(B[m8] == 28.0f);
	CHECK(B[m9] == 29.0f);
	CHECK(B[m10] == 30.0f);

	CHECK(A[m0] == 21.0f);
	CHECK(A[m1] == 23.0f);
	CHECK(A[m2] == 25.0f);

	CHECK(A[m4] == 29.0f);
	CHECK(A[m5] == 31.0f);
	CHECK(A[m6] == 33.0f);

	CHECK(A[m8] == 37.0f);
	CHECK(A[m9] == 39.0f);
	CHECK(A[m10] == 41.0f);


#endif
} TEST_END

// --- End of File ---
