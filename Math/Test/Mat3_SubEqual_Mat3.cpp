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

TEST(Mat3_SubEqual_Mat3, TestConfig::ALL)
{
#if Mat3_SubEqual_Mat3

	Vec3 V0(30.0f, 29.0f, 28.0f);
	Vec3 V1(26.0f, 25.0f, 24.0f);
	Vec3 V2(22.0f, 21.0f, 20.0f);

	Mat3 A(V0, V1, V2);

	CHECK(A[m0] == 30.0f);
	CHECK(A[m1] == 29.0f);
	CHECK(A[m2] == 28.0f);

	CHECK(A[m4] == 26.0f);
	CHECK(A[m5] == 25.0f);
	CHECK(A[m6] == 24.0f);

	CHECK(A[m8] == 22.0f);
	CHECK(A[m9] == 21.0f);
	CHECK(A[m10] == 20.0f);

	Vec3 V4(20.0f, 21.0f, 22.0f);
	Vec3 V5(24.0f, 25.0f, 26.0f);
	Vec3 V6(28.0f, 29.0f, 30.0f);
	Vec3 V7(32.0f, 33.0f, 34.0f);

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

	A -= B;

	CHECK(B[m0] == 20.0f);
	CHECK(B[m1] == 21.0f);
	CHECK(B[m2] == 22.0f);
	
	CHECK(B[m4] == 24.0f);
	CHECK(B[m5] == 25.0f);
	CHECK(B[m6] == 26.0f);
	
	CHECK(B[m8] == 28.0f);
	CHECK(B[m9] == 29.0f);
	CHECK(B[m10] == 30.0f);



	CHECK(A[m0] == 10.0f);
	CHECK(A[m1] == 8.0f);
	CHECK(A[m2] == 6.0f);
	
	CHECK(A[m4] == 2.0f);
	CHECK(A[m5] == 0.0f);
	CHECK(A[m6] == -2.0f);
	
	CHECK(A[m8] == -6.0f);
	CHECK(A[m9] == -8.0f);
	CHECK(A[m10] == -10.0f);


#endif
} TEST_END

// ---  End of File ---
