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

TEST(Mat4_SET_RotZ, TestConfig::ALL)
{
#if Mat4_SET_RotZ

	Vec4 V0(17.0f, 2.0f, 3.0f, 4.0f);
	Vec4 V1(5.0f, 6.0f, 7.0f, 8.0f);
	Vec4 V2(9.0f, 10.0f, 11.0f, 12.0f);
	Vec4 V3(13.0f, 14.0f, 15.0f, 16.0f);

	Mat4 Rz(V0, V1, V2, V3);

	CHECK(Rz[m0] == 17.0f);
	CHECK(Rz[m1] == 2.0f);
	CHECK(Rz[m2] == 3.0f);
	CHECK(Rz[m3] == 4.0f);
	CHECK(Rz[m4] == 5.0f);
	CHECK(Rz[m5] == 6.0f);
	CHECK(Rz[m6] == 7.0f);
	CHECK(Rz[m7] == 8.0f);
	CHECK(Rz[m8] == 9.0f);
	CHECK(Rz[m9] == 10.0f);
	CHECK(Rz[m10] == 11.0f);
	CHECK(Rz[m11] == 12.0f);
	CHECK(Rz[m12] == 13.0f);
	CHECK(Rz[m13] == 14.0f);
	CHECK(Rz[m14] == 15.0f);
	CHECK(Rz[m15] == 16.0f);

	Rz.set(Mat4::Rot1::Z, 1.0471975512f);

	CHECK(eq(Rz[m0], 0.5f, MATH_TOLERANCE));
	CHECK(eq(Rz[m1], 0.866f, MATH_TOLERANCE));
	CHECK(Rz[m2] == 0.0f);
	CHECK(Rz[m3] == 0.0f);
	CHECK(eq(Rz[m4], -0.866f, MATH_TOLERANCE));
	CHECK(eq(Rz[m5], 0.5f, MATH_TOLERANCE));
	CHECK(Rz[m6] == 0.0f);
	CHECK(Rz[m7] == 0.0f);
	CHECK(Rz[m8] == 0.0f);
	CHECK(Rz[m9] == 0.0f);
	CHECK(Rz[m10] == 1.0f);
	CHECK(Rz[m11] == 0.0f);
	CHECK(Rz[m12] == 0.0f);
	CHECK(Rz[m13] == 0.0f);
	CHECK(Rz[m14] == 0.0f);
//	CHECK(Rz[m15] == 1.0f);
	CHECK(Rz.privHasHint() == true);

#endif
} TEST_END

// --- End of File ---
