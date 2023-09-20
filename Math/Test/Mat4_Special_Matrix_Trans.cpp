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

TEST(Mat4_Special_Matrix_Trans, TestConfig::ALL)
{
#if Mat4_Special_Matrix_Trans

	Mat4 A(Mat4::Trans::XYZ, 2.0f, 3.0f, 4.0f);

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
	CHECK(A[m12] == 2.0f);
	CHECK(A[m13] == 3.0f);
	CHECK(A[m14] == 4.0f);
	//CHECK(A[m15] == 1.0f);
	CHECK(A.privHasHint() == true);

#endif
} TEST_END

// --- End of File ---
