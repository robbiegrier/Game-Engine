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

TEST(Mat4_SET_RotX, TestConfig::ALL)
{
#if Mat4_SET_RotX

	Vec4 V0(17.0f, 2.0f, 3.0f, 4.0f);
	Vec4 V1(5.0f, 6.0f, 7.0f, 8.0f);
	Vec4 V2(9.0f, 10.0f, 11.0f, 12.0f);
	Vec4 V3(13.0f, 14.0f, 15.0f, 16.0f);

	Mat4 Rx(V0, V1, V2, V3);

	CHECK(Rx[m0] == 17.0f);
	CHECK(Rx[m1] == 2.0f);
	CHECK(Rx[m2] == 3.0f);
	CHECK(Rx[m3] == 4.0f);
	CHECK(Rx[m4] == 5.0f);
	CHECK(Rx[m5] == 6.0f);
	CHECK(Rx[m6] == 7.0f);
	CHECK(Rx[m7] == 8.0f);
	CHECK(Rx[m8] == 9.0f);
	CHECK(Rx[m9] == 10.0f);
	CHECK(Rx[m10] == 11.0f);
	CHECK(Rx[m11] == 12.0f);
	CHECK(Rx[m12] == 13.0f);
	CHECK(Rx[m13] == 14.0f);
	CHECK(Rx[m14] == 15.0f);
	CHECK(Rx[m15] == 16.0f);

	// Rot_X Type Constructor:
	Rx.set(Mat4::Rot1::X, 1.0471975512f);

	CHECK(Rx[m0] == 1.0f);
	CHECK(Rx[m1] == 0.0f);
	CHECK(Rx[m2] == 0.0f);
	CHECK(Rx[m3] == 0.0f);
	CHECK(Rx[m4] == 0.0f);
	CHECK(eq(Rx[m5], 0.5f, MATH_TOLERANCE));
	CHECK(eq(Rx[m6], 0.866f, MATH_TOLERANCE));
	CHECK(Rx[m7] == 0.0f);
	CHECK(Rx[m8] == 0.0f);
	CHECK(eq(Rx[m9], -0.866f, MATH_TOLERANCE));
	CHECK(eq(Rx[m10], 0.5f, MATH_TOLERANCE));
	CHECK(Rx[m11] == 0.0f);
	CHECK(Rx[m12] == 0.0f);
	CHECK(Rx[m13] == 0.0f);
	CHECK(Rx[m14] == 0.0f);
	//CHECK(Rx[m15] == 1.0f);
	CHECK(Rx.privHasHint() == true);

#endif
} TEST_END

// --- End of File ---
