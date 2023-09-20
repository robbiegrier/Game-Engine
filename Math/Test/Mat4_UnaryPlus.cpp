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

TEST(Mat4_UnaryPlus, TestConfig::ALL)
{
#if Mat4_UnaryPlus

	Vec4 V0(30.0f, 29.0f, 28.0f, 27.0f);
	Vec4 V1(26.0f, 25.0f, 24.0f, 23.0f);
	Vec4 V2(22.0f, 21.0f, 20.0f, 19.0f);
	Vec4 V3(18.0f, 17.0f, 16.0f, 15.0f);

	Mat4 A(V0, V1, V2, V3);

	CHECK(A[m0] == 30.0f);
	CHECK(A[m1] == 29.0f);
	CHECK(A[m2] == 28.0f);
	CHECK(A[m3] == 27.0f);
	CHECK(A[m4] == 26.0f);
	CHECK(A[m5] == 25.0f);
	CHECK(A[m6] == 24.0f);
	CHECK(A[m7] == 23.0f);
	CHECK(A[m8] == 22.0f);
	CHECK(A[m9] == 21.0f);
	CHECK(A[m10] == 20.0f);
	CHECK(A[m11] == 19.0f);
	CHECK(A[m12] == 18.0f);
	CHECK(A[m13] == 17.0f);
	CHECK(A[m14] == 16.0f);
	CHECK(A[m15] == 15.0f);

	+A;

	CHECK(A[m0] == 30.0f);
	CHECK(A[m1] == 29.0f);
	CHECK(A[m2] == 28.0f);
	CHECK(A[m3] == 27.0f);
	CHECK(A[m4] == 26.0f);
	CHECK(A[m5] == 25.0f);
	CHECK(A[m6] == 24.0f);
	CHECK(A[m7] == 23.0f);
	CHECK(A[m8] == 22.0f);
	CHECK(A[m9] == 21.0f);
	CHECK(A[m10] == 20.0f);
	CHECK(A[m11] == 19.0f);
	CHECK(A[m12] == 18.0f);
	CHECK(A[m13] == 17.0f);
	CHECK(A[m14] == 16.0f);
	CHECK(A[m15] == 15.0f);

#endif
} TEST_END

// ---  End of File ---
