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

TEST(Mat3_GetTranspose, TestConfig::ALL)
{
#if Mat3_GetTranspose

	Vec3 v0(1.0f, 2.0f, 3.0f);
	Vec3 v1(5.0f, 6.0f, 7.0f);
	Vec3 v2(9.0f, 10.0f, 11.0f);

	Mat3 A(v0, v1, v2);

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	
	CHECK(A[m8] == 9.0f);
	CHECK(A[m9] == 10.0f);
	CHECK(A[m10] == 11.0f);

	Mat3 B;

	B = A.getT();

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 2.0f);
	CHECK(A[m2] == 3.0f);
	
	CHECK(A[m4] == 5.0f);
	CHECK(A[m5] == 6.0f);
	CHECK(A[m6] == 7.0f);
	
	CHECK(A[m8] == 9.0f);
	CHECK(A[m9] == 10.0f);
	CHECK(A[m10] == 11.0f);

	CHECK(B[m0] == 1.0f);
	CHECK(B[m1] == 5.0f);
	CHECK(B[m2] == 9.0f);
	
	CHECK(B[m4] == 2.0f);
	CHECK(B[m5] == 6.0f);
	CHECK(B[m6] == 10.0f);

	CHECK(B[m8] == 3.0f);
	CHECK(B[m9] == 7.0f);
	CHECK(B[m10] == 11.0f);

#endif
} TEST_END

// --- End of File ---
