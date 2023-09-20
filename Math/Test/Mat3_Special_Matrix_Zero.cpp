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

TEST(Mat3_Special_Matrix_Zero, TestConfig::ALL)
{
#if Mat3_Special_Matrix_Zero

	Mat3 A(Mat3::Special::Zero);

	CHECK(A[m0] == 0.0f);
	CHECK(A[m1] == 0.0f);
	CHECK(A[m2] == 0.0f);

	CHECK(A[m4] == 0.0f);
	CHECK(A[m5] == 0.0f);
	CHECK(A[m6] == 0.0f);
	
	CHECK(A[m8] == 0.0f);
	CHECK(A[m9] == 0.0f);
	CHECK(A[m10] == 0.0f);

#endif
} TEST_END

// --- End of File ---
