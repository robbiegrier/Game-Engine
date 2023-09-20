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

TEST(Mat4_Special_isIdentity_true, TestConfig::ALL)
{
#if Mat4_Special_isIdentity_true

	Vec4 V0(1.0f, 0.0f, 0.0f, 0.0f);
	Vec4 V1(0.0f, 1.0f, 0.0f, 0.0f);
	Vec4 V2(0.0f, 0.0f, 1.0f, 0.0f);
	Vec4 V3(0.0f, 0.0f, 0.0f, 1.0f);

	Mat4 A(V0, V1, V2, V3);

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 0.0f);
	CHECK(A[m2] == 0.0f);
	CHECK(A[m3] == 0.0f);
	CHECK(A[m4] == 0.0f);
	CHECK(A[m5] == 1.0f);
	CHECK(A[m6] == 0.0f);
	CHECK(A[m7] == 0.0f);
	CHECK(A[m8] == 0.0f);
	CHECK(A[m9] == 0.0f);
	CHECK(A[m10] == 1.0f);
	CHECK(A[m11] == 0.0f);
	CHECK(A[m12] == 0.0f);
	CHECK(A[m13] == 0.0f);
	CHECK(A[m14] == 0.0f);
	CHECK(A[m15] == 1.0f);

	const bool value = A.isIdentity();

	CHECK(value == true);

#endif
} TEST_END


// ---  End of File ---
