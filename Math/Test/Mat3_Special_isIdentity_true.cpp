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

TEST(Mat3_Special_isIdentity_true, TestConfig::ALL)
{
#if Mat3_Special_isIdentity_true

	Vec3 V0(1.0f, 0.0f, 0.0f);
	Vec3 V1(0.0f, 1.0f, 0.0f);
	Vec3 V2(0.0f, 0.0f, 1.0f);


	Mat3 A(V0, V1, V2);

	CHECK(A[m0] == 1.0f);
	CHECK(A[m1] == 0.0f);
	CHECK(A[m2] == 0.0f);
	
	CHECK(A[m4] == 0.0f);
	CHECK(A[m5] == 1.0f);
	CHECK(A[m6] == 0.0f);

	CHECK(A[m8] == 0.0f);
	CHECK(A[m9] == 0.0f);
	CHECK(A[m10] == 1.0f);

	const bool value = A.isIdentity();

	CHECK(value == true);

#endif
} TEST_END


// ---  End of File ---
